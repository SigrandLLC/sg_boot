#ifndef _UTILS_H
#define	_UTILS_H

#include <ctype.h> // UINT32

unsigned short ntohs (unsigned short s);
unsigned long  ntohl (unsigned long  l);
unsigned short htons (unsigned short s);
unsigned long  htonl (unsigned long  l);
int ultoa (unsigned long num, char *str);
int btoa  (char num, char *str);
int mactostr (char *mac, char *str);
int macscanf (char *mac, char *buf);
int ipscanf (UINT32 *ipbuf, char *buf);
void IpAddrToStr (UINT32 tmp_ip, char *str);
unsigned int ap_hash(const char *str);

#endif
