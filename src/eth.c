/*****************************************************************************
;
;    Project : Am5120
;    Creator : David Weng
;    File    : eth.c
;    Abstract:
;
;*****************************************************************************/
#include <ctype.h>
#include <irqlib.h>
#include <memlib.h>
#include <eth.h>
#include <skbuff.h>
#include <if_5120.h>

#define DEF_BRG_CTRL_FLAG			0x00


// Imported Function
extern void panic (void);

#ifdef DBG_OUT_SUPPORT
extern void DbgUartPutStr (const char *);
extern void DbgUartPutChar (char);
#else
#define DbgUartPutStr
#define DbgUartPutChar
#endif

PEthPktQueue_OBJ EthPktQueue;
static char local_mac[6];

/**********************************************************************************/
/* InitBridge:																	  */

/**********************************************************************************/
UINT32 InitEthPktQueue (void)
{

	if ((EthPktQueue = MemAlloc (sizeof (EthPktQueue_OBJ), TRUE)) == NULL) return 1;

	EthPktQueue->iqH_head = EthPktQueue->iqH_tail = NULL;
	EthPktQueue->iqL_head = EthPktQueue->iqL_tail = NULL;
	EthPktQueue->CtrlFlag = DEF_BRG_CTRL_FLAG;
	return 0;
}



/**********************************************************************************/
/* IndicateRxPacketL:															  */

/**********************************************************************************/
void IndicateRxPacketL (PDRV_PACKET_DESC Pkt)
{
	// This function MUST be called with interrupt disabled.

	if (Pkt == NULL) return;

	//	Pkt->Next = NULL;

	if (EthPktQueue->iqL_head == NULL)
	{
		EthPktQueue->iqL_head = EthPktQueue->iqL_tail = Pkt;
	} else
	{
		EthPktQueue->iqL_tail->Next = Pkt;
		EthPktQueue->iqL_tail = Pkt;
	}
}

int eth_init (void)
{
	int ethnum;
	bsp_GetMacBase (local_mac, &ethnum);
	return 0;
}

int eth_reinit (void)
{
	int ethnum;
	bsp_GetMacBase (local_mac, &ethnum);
	return 0;
}

int eth_send (struct sk_buff *skb, unsigned char *dest_addr, unsigned short proto)
{
	int s, i;
	UINT8 *buf;
	PDRV_PACKET_DESC Pkt;
	struct ethhdr *eth_hdr;

	eth_hdr = (struct ethhdr *) skb_push (skb, ETH_HLEN);

	memcpy (eth_hdr->h_dest, dest_addr, ETH_ALEN);
	memcpy (eth_hdr->h_source, local_mac, ETH_ALEN);
	eth_hdr->h_proto = htons (proto);

	// Get free packet for the transmitting descriptor
	Pkt = (PDRV_PACKET_DESC) Am5120_GetFreePkt ();

	//to fill up the packet content..
	buf = Pkt->Buf + Pkt->buf1off;
	memcpy (buf, skb->data, skb->len);
	Pkt->PktLen = skb->len;
	if ((Pkt->PktLen) < 0x3c)
	{
		for (i = Pkt->PktLen; i < 60; i++)
		{
			*(buf + i) = 0x00;
			Pkt->PktLen++;
		}
	}

	SendPacketsL (Pkt);
}

int eth_rcv (struct sk_buff *skb)
{
	static int rxcount = 0;
	PDRV_PACKET_DESC Pkt;
	UINT32 srcport, srcvlan;
	UINT8 *buf;
	int irq_state;

	irq_state = mips_int_lock ();
	Pkt = EthPktQueue->iqL_head;
	if (EthPktQueue->iqL_head != NULL)
	{
		if (EthPktQueue->iqL_head == EthPktQueue->iqL_tail)
			EthPktQueue->iqL_head = EthPktQueue->iqL_tail = NULL;
		else
			EthPktQueue->iqL_head = Pkt->Next;

		Pkt->Next = NULL;
	}
	mips_int_unlock (irq_state);

	if (Pkt != NULL)
	{
		srcport = Pkt->SrcPort;
		buf = Pkt->Buf + Pkt->buf1off;
		//EthPktQueue->v_statis[srcvlan].RxPkt ++;
		rxcount++;
		skb->len = Pkt->PktLen;
		memcpy (skb->data, buf, skb->len);
		Am5120_RefreePkt (Pkt);
		return 0;
	}
	return -1;
}

int eth_rcv_packet (struct sk_buff *skb)
{
	struct ethhdr *eth_hdr;
	int ip_protocol;

	ip_protocol = 0;
	if (eth_rcv (skb) != -1)
	{
		ip_protocol = -1;
		eth_hdr = (struct ethhdr *) (skb->data);
		skb_pull (skb, ETH_HLEN);
		if (ntohs (eth_hdr->h_proto) == ETH_P_ARP)
			arp_rcv_packet (skb);
		else if (ntohs (eth_hdr->h_proto) == ETH_P_IP)
			ip_protocol = 1;
	}

	return ip_protocol;
}

void eth_skb_reserve (struct sk_buff *skb)
{
	skb_reserve (skb, ETH_HLEN);
}


