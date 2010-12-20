#include <mips.h>
#include <mips4kc.h>
#include <asm.h>
#include <adm5120.h>

#define SYS_STACK_SIZE			0x4000

.text
LEAF(_ldrinit)
	.set noreorder
	/*  Disable interrupt  */
	mfc0	v0, CP0_STATUS_REG
	and		v0, CP0_STATUS_BEV_BIT
	mtc0	v0, CP0_STATUS_REG
	
	// Jump to SDRAM 
	la		t0, 2f				
	jr		t0
	nop							/*	Branch delay slot  */
2:			
	// We are in SDRAM now
	/* clear BSS section  */
	la		a0, _C_LABEL(__bss_start)
	la		a1, _C_LABEL(_end)
	subu	a1, a1, a0
	
	/*  Clear BSS Loop */
3:
#ifndef RTL_SIMULATION
	sw		zero, 0(a0)
	sw		zero, 4(a0)
	subu	a1, a1, 4*4			// substract the length to be zeroed
	sw		zero, 8(a0)
	sw		zero, 12(a0)
	bgtz	a1, 3b
	addu	a0, a0, 4*4			// next destination address // Branch delay slot
#endif
	
	/*  Load gp  */
	la		gp, _C_LABEL(_gp)
	
	/*  Initialize stack  */
	la		t0, _C_LABEL(_end)
	li		t1, SYS_STACK_SIZE
	add		sp, t0, t1
	
	/* Align sp to 16byte boundry*/
	li		t0, ~0xf
	and		sp, sp, t0
	
	/* Store the first free memory address */
	addu	t0, sp, 0x10
	la		t1, _C_LABEL(_heap_h)
	sw		t0, 0(t1)
	
	// Call c_entry and never returned
	jal		_C_LABEL(c_entry)
	nop							// Branch delay slot
	
	/*  Never Reached  */
1:	
	b		1b
	nop
		
	.set reorder
END(_ldrinit)


/*  Get program counter  */
LEAF(_get_pc)
	.set noreorder
	jr		ra
	move	v0, ra				/*	Branch delay slot */
	.set reorder
END(_get_pc)


LEAF(_icache_sync_all)
	.set noreorder
	mtc0	zero, CP0_TAGLO_REG
	mtc0	zero, CP0_TAGHI_REG
	li		a0, MEM_KSEG0_BASE
	addiu	a1, a0, CPU_ICACHE_DFE_SIZE - 0x10*8
	
1:
	cache	ICACHE_INDEX_STORETAG, 0x00(a0)
	cache	ICACHE_INDEX_STORETAG, 0x10(a0)
	cache	ICACHE_INDEX_STORETAG, 0x20(a0)
	cache	ICACHE_INDEX_STORETAG, 0x30(a0)
	cache	ICACHE_INDEX_STORETAG, 0x40(a0)
	cache	ICACHE_INDEX_STORETAG, 0x50(a0)
	cache	ICACHE_INDEX_STORETAG, 0x60(a0)
	cache	ICACHE_INDEX_STORETAG, 0x70(a0)
	bne		a0, a1, 1b
	addiu	a0, a0, 0x10*8					/* Branch delay slot */	

	jr		ra
	nop										/* Branch delay slot */	
	.set reorder
END(_icache_sync_all)

