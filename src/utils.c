#include <ctype.h>
#include <utils.h>

unsigned short ntohs (unsigned short s)
{
	return (s >> 8) | (s << 8);
}

unsigned long ntohl (unsigned long l)
{
	return ((l >> 24) & 0x000000ff) |
		((l >> 8) & 0x0000ff00) |
		((l << 8) & 0x00ff0000) |
		((l << 24) & 0xff000000);
}

unsigned short htons (unsigned short s)
{
	return (s >> 8) | (s << 8);
}

unsigned long htonl (unsigned long l)
{
	return ((l >> 24) & 0x000000ff) |
		((l >> 8) & 0x0000ff00) |
		((l << 8) & 0x00ff0000) |
		((l << 24) & 0xff000000);
}

int ultoa (unsigned long num, char *str)
{
	unsigned int tmp;
	int i;
	for (i = 0; i < 8; i++)
	{
		tmp = ((num >> (32 - (i + 1)*4))&0x0000000f);
		if (tmp >= 0xa && tmp <= 0xf)
			tmp = tmp + 55;
		else
			tmp = tmp + 48;
		str[i] = (char) tmp;
	}
	return 0;
}

int btoa (char num, char *str)
{
	unsigned int tmp;
	int i;
	for (i = 0; i < 2; i++)
	{
		tmp = ((num >> (8 - (i + 1)*4))&0x0f);
		if (tmp >= 0xa && tmp <= 0xf)
			tmp = tmp + 55;
		else
			tmp = tmp + 48;
		str[i] = (char) tmp;
	}
	return 0;
}

int mactostr (UINT8 *mac, char *str)
{
	int i, j = 0;
	char tmp[] = "XX";
	for (i = 0; i < 6; i++)
	{
		btoa (mac[i], tmp);
		str[j] = tmp[0];
		str[j + 1] = tmp[1];
		if (i != 5)
			str[j + 2] = '-';
		j = j + 3;
	}
	return 0;
}

int macscanf (UINT8 *mac, char *buf)
{
	int i;
	UINT8 s, tmp;
	for (i = 0; i < 6; i++)
	{
		s = buf[i * 3];
		if (s >= '0' && s <= '9')
			s = s - '0';
		else if (s >= 'a' && s <= 'f')
			s = s - 'a' + 0xa;
		else if (s >= 'A' && s <= 'F')
			s = s - 'A' + 0xa;
		else
			break;
		tmp = (s << 4)&0xf0;

		s = buf[i * 3 + 1];
		if (s >= '0' && s <= '9')
			s = s - '0';
		else if (s >= 'a' && s <= 'f')
			s = s - 'a' + 0xa;
		else if (s >= 'A' && s <= 'F')
			s = s - 'A' + 0xa;
		else
			break;
		tmp = tmp | (s & 0x0f);

		mac[i] = (UINT8) tmp;
	}
	return i;
}

int ipscanf (UINT32 *ipbuf, char *buf)
{
	int i, j = 0;
	UINT32 tmp1 = 0, tmp2 = 0;
	*ipbuf = 0;
	for (i = 0; i < 4; i++)
	{
		while (buf[j] != '.')
		{
			if (buf[j] >= '0' && buf[j] <= '9')
				tmp1 = buf[j] - '0';
			else
				break;
			tmp2 = tmp2 * 10 + tmp1;
			j++;
		}
		if (tmp2 > 255)
			break;
		*ipbuf = (*ipbuf) | (((char) tmp2 & 0xff) << (32 - (i + 1)*8));
		j++;
		tmp2 = 0;
	}
	return i;
}

void IpAddrToStr (UINT32 tmp_ip, char *str)
{
	int i, j = 0, k = 0, remainder;
	UINT32 tmp;
	char s[3];
	for (i = 0; i < 4; i++)
	{
		tmp = (int) (tmp_ip >> (32 - (i + 1)*8)) & 0x000000ff;
		while (1)
		{
			remainder = tmp % 10;
			tmp = (int) (tmp / 10);
			if ((remainder == 0) && (tmp == 0))
				break;
			s[j] = remainder + '0';
			j++;
		}
		if (i != 0)
			k++;
		if (j == 0)
		{
			str[k] = '0';
			str[++k] = '.';
		} else if (j == 1)
		{
			str[k] = s[0];
			str[++k] = '.';
		} else if (j == 2)
		{
			str[k] = s[1];
			str[++k] = s[0];
			str[++k] = '.';
		} else if (j == 3)
		{
			str[k] = s[2];
			str[++k] = s[1];
			str[++k] = s[0];
			str[++k] = '.';
		}
		if (i == 3)
			str[k] = '\0';
		else
			j = 0;
	}

}

