#include <skbuff.h>
#include <eth.h>
#include <arp.h>
#include <ip.h>
#include <checksum.h>
#include <param.h>
#include <utils.h>

static UINT32 local_ip = 0;
static UINT32 gw_ip = 0;

int ip_init (void)
{
	bsp_GetTftpIp (&local_ip);
	bsp_GetGwIp(&gw_ip);
	return 0;
}

int ip_reinit (unsigned long ip)
{
	(void)ip;
	bsp_GetTftpIp (&local_ip);
	return 0;
}

int ip_rcv_packet (struct sk_buff *skb)
{
	struct iphdr *ip_hdr;
	int udp_protocol;

	udp_protocol = 0;
	if (eth_rcv_packet (skb) == 1)
	{
		ip_hdr = (struct iphdr *) (skb->data);
		if (ntohl (ip_hdr->daddr) == local_ip || ntohl(ip_hdr->daddr) == gw_ip)
		{
			skb->len = ntohs (ip_hdr->tot_len);
			skb_pull (skb, sizeof (struct iphdr));

			if (ip_hdr->protocol == UDP)
				udp_protocol = 1;
		}
	}

	return udp_protocol;

}

int ip_send (struct sk_buff *skb, unsigned long ip, unsigned char proto)
{
	struct iphdr *ip_hdr;
	static unsigned short ip_id = 32;
	unsigned char dest_eth_addr[ETH_ALEN];

	if (arp_get_eth_addr(ip, dest_eth_addr))
		if(arp_get_eth_addr(gw_ip, dest_eth_addr))
			return -1;

	ip_hdr = (struct iphdr *) skb_push (skb, sizeof (struct iphdr));

	ip_hdr->ihl = 5;
	ip_hdr->version = 4;
	ip_hdr->tos = 0;
	ip_hdr->tot_len = htons (skb->len);
	ip_hdr->id = htons (ip_id++);
	ip_hdr->frag_off = 0;
	ip_hdr->ttl = 20;
	ip_hdr->protocol = proto;
	ip_hdr->check = 0;

	ip_hdr->saddr = htonl (local_ip);
	ip_hdr->daddr = htonl (ip);
	ip_hdr->check = ip_fast_csum ((unsigned char *) ip_hdr, ip_hdr->ihl);

	eth_send (skb, dest_eth_addr, ETH_P_IP);

	return 0;
}

void ip_skb_reserve (struct sk_buff *skb)
{
	eth_skb_reserve (skb);
	skb_reserve (skb, sizeof (struct iphdr));
}

unsigned long ip_get_source_ip (struct sk_buff *skb)
{
	struct iphdr *ip_hdr;

	ip_hdr = (struct iphdr *) (skb->buf + ETH_HLEN);
	return ntohl (ip_hdr->saddr);
}
