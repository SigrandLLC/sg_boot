#include <ctype.h>
#include <nf.h>
#include <nand.h>

// unlikely what I expected, the wp flag protects the entire flash.
int nf_erase (UINT8 *addr, UINT32 len, UINT8 wp)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_erase call: addr = 0x");
	ultoa (addr, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	return nand_erase (addr, len, wp);
}

int nf_write (UINT8 *dst, const UINT8 *src, UINT32 len, UINT8 wp)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_write call: dst = 0x");
	ultoa (dst, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	return nand_write (dst, src, len, wp);
}

int nf_write_boot (UINT8 *dst, const UINT8 *src, UINT32 len)
{
	nand_write_boot (dst, src, len);
	return 0;
}

int nf_read (UINT8 *dst, const UINT8 *src, UINT32 len)
{
	/*char buf[10] = {0};
	buart_print ("\n\rnf_read call: src = 0x");
	ultoa (src, buf);
	buart_print (buf);
	buart_print (", len = 0x");
	ultoa (len, buf);
	buart_print (buf);*/

	return nand_read (dst, src, len);
}

