#include <tftp.h>
#include <ctype.h>
#include <string.h> // strlen strncpy memcpy
#include <skbuff.h>
#include <eth.h>
#include <arp.h>
#include <ip.h>
#include <udp.h>
#include <if_5120.h>
#include <utils.h>
#include <buart.h>
#include <param.h>
#include <timer.h>
#include <except.h>
#include <uartdrv.h>
#include <bsp_cfg.h>

static UINT32 remote_ip; // TFTP server ip
static UINT32 gw_ip;	// TFTP server gateway ip
static unsigned short remote_port; // TFTP server port
static unsigned short remote_block; // TFTP server data block number
static int tftpc_read_start;

int tftp_send_ack (int block)
{
	struct tftphdr *tftp_ack;
	struct sk_buff skb;

	skb_headerinit (&skb);
	udp_skb_reserve (&skb);
	tftp_ack = (struct tftphdr *) skb_put (&skb, sizeof (struct tftphdr));
	tftp_ack->th_opcode = htons (TFTP_ACK);
	tftp_ack->th_block = htons (block);

	udp_send (&skb, remote_ip, TFTP, remote_port);

	return 0;
}

int tftp_send_rrq (UINT32 servip, const char *filename)
{
	struct sk_buff skb;
	char *temp;
	unsigned int rrqlen;
	short *opCode;
	int fileNameLen;
	skb_headerinit (&skb);
	udp_skb_reserve (&skb);
	temp = (char *) skb.data;

	// operating code
	opCode = (short *) temp;
	*opCode = htons (TFTP_RRQ);
	temp += 2;

	// remote file name
	fileNameLen = strlen (filename);
	strncpy (temp, filename, fileNameLen);
	temp += fileNameLen;

	// stuff byte
	*temp++ = 0;

	// operating mode "octet"
	strncpy (temp, "octet", 5);
	temp += 5;

	// stuff byte
	*temp = 0;

	// Opcode + filename + one stuff + "octet" + one stuff, the format of rrq.
	rrqlen = 2 + fileNameLen + 1 + strlen ("octet") + 1;
	skb.len = rrqlen;

	remote_ip = servip;
	remote_port = TFTP;
	remote_block = 1;

	udp_send (&skb, remote_ip, TFTP, remote_port);
	return 0;
}

int tftp_rcv_packet (struct sk_buff *skb); // forward declaration

int rcv_imgpkt (UINT8 *buf, int *buf_len)
{
	struct sk_buff skb;
	int status;
	skb_headerinit (&skb);
	if (tftp_rcv_packet (&skb) == 1)
	{
		*buf_len = skb.len;
		memcpy (buf, skb.data, *buf_len);
		if (*buf_len < 512)
			status = TFTP_END;
		else
			status = TFTP_CONTINUE;
	} else
	{
		*buf_len = 0;
		status = TFTP_NO_DATA;
	}
	return status;
}

int tftp_rcv_packet (struct sk_buff *skb)
{
	struct tftphdr *tftp_hdr;
	int data_flag;

	data_flag = 0;
	if (udp_rcv_packet (skb) == 1)
	{
		tftp_hdr = (struct tftphdr *) skb->data;
		if (ntohs (tftp_hdr->th_opcode) == TFTP_DATA)
		{
			if (remote_ip != ip_get_source_ip (skb))
			{
				return data_flag;
			}

			remote_port = udp_get_source_port (skb);

			if (remote_block == ntohs (tftp_hdr->th_block))
			{
				tftpc_read_start = 1;
				skb_pull (skb, sizeof (struct tftphdr));
				tftp_send_ack (remote_block);
				remote_block++;
				data_flag = 1;
			} else if (remote_block > ntohs (tftp_hdr->th_block))
				tftp_send_ack (ntohs (tftp_hdr->th_block));
			else
				tftp_send_ack (remote_block);
		}
	}
	return data_flag;
}

UINT32 tftpc (UINT8 *buf, int buf_size, int mode)
{
	UINT32 ticks, servip, gwip, total_len;
	char servfile[BSP_FILENAME_STR_LEN + 1];
	int transmit_flag, pkt_len = 0, rrqcount = 0;
	UINT8 *working = buf;
	(void)buf_size;

	if (get_tftp_param (&servip, &gwip, servfile, mode) != 0)
	{
		buart_print ("\n\rTFTP Server IP or Filename Error.");
		return 0;
	}

	gw_ip = gwip;
	transmit_flag = TFTP_START;
	total_len = 0;
	if5120turnon ();
	tftp_send_rrq (servip, servfile);
	ticks = UpTime ();
	while (1)
	{
		if (buart_get (0) == 0x1b)//press ESC to stop.
		{
			total_len = 0;
			break;
		}
		if ((!tftpc_read_start) && ((UpTime () - ticks) > 100))
		{
			tftp_send_rrq (servip, servfile);
			ticks = UpTime ();
			rrqcount++;
			if (rrqcount == 5)
				break;
		}
		transmit_flag = rcv_imgpkt (working, &pkt_len);
		if (tftpc_read_start)
		{
			if (transmit_flag == TFTP_CONTINUE)//Get img packet successfully.
			{
				working += pkt_len;
				total_len += pkt_len;
				if ((total_len % (32 * 1024 * 2) == 0))
					buart_put ('.');
				ticks = UpTime ();
			} else if (transmit_flag == TFTP_NO_DATA)//Cannot get img packet.
			{
				if ((UpTime () - ticks) > 2000)//Timeout.
				{
					total_len = 0;
					//buart_print("\n\rTimeOut..");
					break;
				}
			} else if (transmit_flag == TFTP_END)//transmit successfully.
			{
				working += pkt_len;
				total_len += pkt_len;
				break;
			}
		}
	}
	tftpc_read_start = 0;
	if5120shutdown ();
	return total_len;
}

void tftp_init (void)
{
	remote_ip = 0;
	remote_port = TFTP;
	remote_block = 1;
	tftpc_read_start = 0;

	//Init socket buffer
	skb_init ();

	//Init Ethernet protocol.
	eth_init ();

	//Init ARP protocol
	arp_init ();

	//Init IP protocol
	ip_init ();

	//Init UDP protocol
	udp_init ();

	//Init Data Receive Queue
	if (InitEthPktQueue () != 0)
	{
		buart_print ("\n\rInit ethernet receive queue error");
		panic ();
	}

	//Shutdown Switch Until TFTP Download Start.
	if5120shutdown ();
}
