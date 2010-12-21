#include <utils.h>
#include "skbuff.h"
#include "eth.h"
#include "ip.h"
#include "udp.h"

int udp_init (void)
{
	return 0;
}

int udp_rcv_packet (struct sk_buff *skb)
{
	struct udphdr *udp_hdr;
	int tftp_protocol;

	tftp_protocol = 0;
	if (ip_rcv_packet (skb) == 1)
	{
		udp_hdr = (struct udphdr *) (skb->data);
		skb->len = ntohs (udp_hdr->len);
		skb_pull (skb, sizeof (struct udphdr));
		if (ntohs (udp_hdr->dest) == TFTP)
			tftp_protocol = 1;
	}
	return tftp_protocol;
}

int udp_send (struct sk_buff *skb, unsigned long ip,
	unsigned short source, unsigned short dest)
{
	struct udphdr *udp_hdr;

	udp_hdr = (struct udphdr *) skb_push (skb, sizeof (struct udphdr));
	udp_hdr->source = htons (source);
	udp_hdr->dest = htons (dest);
	udp_hdr->len = htons (skb->len);
	udp_hdr->check = 0;

	ip_send (skb, ip, UDP);

	return 0;
}

void udp_skb_reserve (struct sk_buff *skb)
{
	ip_skb_reserve (skb);
	skb_reserve (skb, sizeof (struct udphdr));
}

unsigned short udp_get_source_port (struct sk_buff *skb)
{
	struct udphdr *udp_hdr;
	udp_hdr = (struct udphdr *) (skb->buf + ETH_HLEN + sizeof (struct iphdr));

	return ntohs (udp_hdr->source);
}

