#include <asm.h>

/*
 *	Fold a partial checksum without adding pseudo headers
 */
static inline unsigned short int csum_fold (unsigned int sum)
{
	__asm__ (
		".set\tnoat\t\t\t# csum_fold\n\t"
		"sll\t$1,%0,16\n\t"
		"addu\t%0,$1\n\t"
		"sltu\t$1,%0,$1\n\t"
		"srl\t%0,%0,16\n\t"
		"addu\t%0,$1\n\t"
		"xori\t%0,0xffff\n\t"
		".set\tat"
		: "=r" (sum)
		: "0" (sum));

	return sum;
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 *
 *	By Jorge Cwik <jorge@laser.satlink.net>, adapted for linux by
 *	Arnt Gulbrandsen.
 */
static inline unsigned short ip_fast_csum (unsigned char *iph,
	unsigned int ihl)
{
	unsigned int sum;
	unsigned long dummy;

	/*
	 * This is for 32-bit MIPS processors.
	 */
	__asm__ __volatile__ (
		".set\tnoreorder\t\t\t# ip_fast_csum\n\t"
		".set\tnoat\n\t"
		"lw\t%0, (%1)\n\t"
		"subu\t%2, 4\n\t"
		"#blez\t%2, 2f\n\t"
		" sll\t%2, 2\n\t"
		"lw\t%3, 4(%1)\n\t"
		"addu\t%2, %1\n\t"
		"addu\t%0, %3\n\t"
		"sltu\t$1, %0, %3\n\t"
		"lw\t%3, 8(%1)\n\t"
		"addu\t%0, $1\n\t"
		"addu\t%0, %3\n\t"
		"sltu\t$1, %0, %3\n\t"
		"lw\t%3, 12(%1)\n\t"
		"addu\t%0, $1\n\t"
		"addu\t%0, %3\n\t"
		"sltu\t$1, %0, %3\n\t"
		"addu\t%0, $1\n"

		"1:\tlw\t%3, 16(%1)\n\t"
		"addiu\t%1, 4\n\t"
		"addu\t%0, %3\n\t"
		"sltu\t$1, %0, %3\n\t"
		"bne\t%2, %1, 1b\n\t"
		" addu\t%0, $1\n"

		"2:\t.set\tat\n\t"
		".set\treorder"
		: "=&r" (sum), "=&r" (iph), "=&r" (ihl), "=&r" (dummy)
		: "1" (iph), "2" (ihl));

	return csum_fold (sum);
}
