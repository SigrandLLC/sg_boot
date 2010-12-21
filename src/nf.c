#include <ctype.h>
#include <nf.h>
#include <nand.h>

// флаг wp работает совсем не так, как я ожидал, он защищает всю флешку, а не
// определённые сектора, но написанный функционал я оставлю, он никому не мешает
int nf_erase (UINT8 *addr, UINT32 len, UINT8 wp)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_erase call: addr = 0x");
	ultoa (addr, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	nand_erase (addr, len, wp);
	return 0;
}

int nf_write (UINT8 *dst, UINT8 *src, UINT32 len, UINT8 wp)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_write call: dst = 0x");
	ultoa (dst, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	nand_write (dst, src, len, wp);
	return 0;
}

int nf_write_boot (UINT8 *dst, UINT8 *src, UINT32 len)
{
	nand_write_boot (dst, src, len);
	return 0;
}

int nf_read (UINT8 *dst, UINT8 *src, UINT32 len)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_read call: src = 0x");
	ultoa (src, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	nand_read (dst, src, len);
	return 0;
}

