#include <asm.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>


LEAF(icahce_invalidate_line)
	.set noreorder
	cache	ICACHE_HIT_INVALIDATE, 0(a0)
	jr		ra
	nop
	.set reorder
END(icahce_invalidate_line)


LEAF(icache_invalidate_block)
	.set noreorder
/*
 *	a0 : start address
 *	a1 : length
 */

	addu	a1, a0, a1			// a1 holds the end address
	li		v0, 0xf
	addu	a1, a1, v0
	not		v0, v0
	and		a0, a0, v0			// align a0 to the start of cache line
	and		a1, a1, v0

	subu	v0, a1, a0			// v0 holds the length of the block
	slt		v1, v0, 4*16
	bnez	v1, 2f				// Jump, if less than 4 cache lines
	nop
1:
	cache	ICACHE_HIT_INVALIDATE, 0x00(a0)
	cache	ICACHE_HIT_INVALIDATE, 0x10(a0)
	cache	ICACHE_HIT_INVALIDATE, 0x20(a0)
	cache	ICACHE_HIT_INVALIDATE, 0x30(a0)
	addu	a0, a0, 4*16
	subu	v0, a1, a0
	slt		v1, 4*16
	beqz	v1, 1b
	nop

2:
	beq		a1, a0, 4f
	nop
3:
	cache	ICACHE_HIT_INVALIDATE, 0x00(a0)
	addu	a0, a0, 16
	bne		a1, a0, 3b
	nop

4:
	jr		ra
	nop
	.set reorder
END(icache_invalidate_block)


LEAF(dcahce_invalidate_line)
	.set noreorder
	cache	DCACHE_HIT_INVALIDATE, 0(a0)
	jr		ra
	nop
	.set reorder
END(dcahce_invalidate_line)


LEAF(dcache_invalidate_block)
	.set noreorder
/*
 *	a0 : start address
 *	a1 : length
 */

	addu	a1, a0, a1			// a1 holds the end address
	li		v0, 0xf
	addu	a1, a1, v0
	not		v0, v0
	and		a0, a0, v0			// align a0 to the start of cache line
	and		a1, a1, v0

	subu	v0, a1, a0			// v0 holds the length of the block
	slt		v1, v0, 4*16
	bnez	v1, 2f				// Jump, if less than 4 cache lines
	nop
1:
	cache	DCACHE_HIT_INVALIDATE, 0x00(a0)
	cache	DCACHE_HIT_INVALIDATE, 0x10(a0)
	cache	DCACHE_HIT_INVALIDATE, 0x20(a0)
	cache	DCACHE_HIT_INVALIDATE, 0x30(a0)
	addu	a0, a0, 4*16
	subu	v0, a1, a0
	slt		v1, 4*16
	beqz	v1, 1b
	nop

2:
	beq		a1, a0, 4f
	nop
3:
	cache	DCACHE_HIT_INVALIDATE, 0x00(a0)
	addu	a0, a0, 16
	bne		a1, a0, 3b
	nop

4:
	jr		ra
	nop
	.set reorder
END(dcache_invalidate_block)
