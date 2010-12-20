#include <mips.h>
#include <mips4kc.h>
#include <asm.h>
#include <adm5120.h>

#define VEC_STACK_FRAME			4*4
#define FRAME_AT_REG			0
#define FRAME_V0_REG			1*4
#define FRAME_V1_REG			2*4
#define FRAME_A0_REG			3*4
#define FRAME_A1_REG			4*4
#define FRAME_A2_REG			5*4
#define FRAME_A3_REG			6*4
#define FRAME_T0_REG			7*4
#define FRAME_T1_REG			8*4
#define FRAME_T2_REG			9*4
#define FRAME_T3_REG			10*4
#define FRAME_T4_REG			11*4
#define FRAME_T5_REG			12*4
#define FRAME_T6_REG			13*4
#define FRAME_T7_REG			14*4
#define FRAME_T8_REG			15*4
#define FRAME_T9_REG			16*4
#define FRAME_GP_REG			17*4
#define FRAME_RA_REG			18*4
#define FRAME_HI_REG			19*4
#define FRAME_LO_REG			20*4
#define FRAME_SR_REG			21*4
#define FRAME_CAUSE_REG			22*4
#define FRAME_EPC_REG			23*4
#define FRAME_SIZE				(FRAME_EPC_REG + 12)
.text

/*
 *  TLB refill exception handler
 *  This will be installed at 0x80000000
 */
VECTOR(TLBrefill_except, unknown)
.set noreorder
.set noat

	/*
	 *	The exception is not handled here.
	 */

1:
	b		1b
	nop									/* Branch delay slot */

.set at
.set reorder
VECTOR_END(TLBrefill_except)


/*
 *  General exception handler
 *  This will be installed at 0x80000180
 */
VECTOR(general_except, unknown)
.set noreorder
.set noat
1:
	b		1b
	nop
.set at
.set reorder
VECTOR_END(general_except)

	/*
	 *  Interrupt exception handler.
	 *  This will be installed at 0x80000200
	 */
VECTOR(int_except, unknown)
.set noreorder
.set noat
	subu	sp, sp, FRAME_SIZE			//#1	// Create a stack frame
	sw		AT, FRAME_AT_REG(sp)				//#2
	sw		v0, FRAME_V0_REG(sp)				//#3
	sw		v1, FRAME_V1_REG(sp)				//#4
	mflo	v0						//#5	// Get Hi
	mfhi	v1						//#6	// Get Lo
	sw		a0, FRAME_A0_REG(sp)				//#7
	sw		a1, FRAME_A1_REG(sp)				//#8
	sw		a2, FRAME_A2_REG(sp)				//#9
	sw		a3, FRAME_A3_REG(sp)				//#10
	mfc0	a0, CP0_STATUS_REG		//#11	// 1st arg
	sw		t0, FRAME_T0_REG(sp)				//#12
	sw		t1, FRAME_T1_REG(sp)				//#13
	sw		t2, FRAME_T2_REG(sp)				//#14
	sw		t3, FRAME_T3_REG(sp)			//#15
	mfc0	a1, CP0_CAUSE_REG		//#16	// 2nd arg
	sw		t4, FRAME_T4_REG(sp)			//#17
	sw		t5, FRAME_T5_REG(sp)			//#18
	sw		t6, FRAME_T6_REG(sp)			//#19
	sw		t7, FRAME_T7_REG(sp)			//#20
	mfc0	a2, CP0_EPC_REG			//#21
	sw		t8, FRAME_T8_REG(sp)			//#22
	sw		t9, FRAME_T9_REG(sp)			//#23
	sw		gp, FRAME_GP_REG(sp)			//#24
	sw		ra, FRAME_RA_REG(sp)			//#25
	sw		v0, FRAME_LO_REG(sp)			//#26
	sw		v1, FRAME_HI_REG(sp)			//#27
	sw		a0, FRAME_SR_REG(sp)
	sw		a2, FRAME_EPC_REG(sp)			//#28
	mtc0	zero, CP0_STATUS_REG	// Clear interrupt, EXL
	la		ra, _C_LABEL(except_ret)	// return address
	/*	Call C interrupt handler  */
	j		_C_LABEL(int_hdl)
	subu	sp, sp, VEC_STACK_FRAME		// Branch delay slot

.set at
.set reorder
VECTOR_END(int_except)


VECTOR(dbg_except, unknown)
	/*
	 *  !!!!  Not implemented yet   !!!!
	 */

	/*	Not reached */
2:
	b		2b
	nop									/* Branch delay slot */
VECTOR_END(dbg_except)


LEAF(except_ret)
	.set noreorder
	.set noat

	addu	sp, sp, VEC_STACK_FRAME


	// Restore status
	lw		a0, FRAME_SR_REG(sp)
	lw		a1, FRAME_EPC_REG(sp)
	lw		v0, FRAME_LO_REG(sp)
	lw		v1, FRAME_HI_REG(sp)
	mtc0	a0, CP0_STATUS_REG
	mtlo	v0
	mthi	v1
	mtc0	a1, CP0_EPC_REG

	lw		AT, FRAME_AT_REG(sp)
	lw		v0, FRAME_V0_REG(sp)
	lw		v1, FRAME_V1_REG(sp)
	lw		a0, FRAME_A0_REG(sp)
	lw		a1, FRAME_A1_REG(sp)
	lw		a2, FRAME_A2_REG(sp)
	lw		a3, FRAME_A3_REG(sp)
	lw		t0, FRAME_T0_REG(sp)
	lw		t1, FRAME_T1_REG(sp)
	lw		t2, FRAME_T2_REG(sp)
	lw		t3, FRAME_T3_REG(sp)
	lw		t4, FRAME_T4_REG(sp)
	lw		t5, FRAME_T5_REG(sp)
	lw		t6, FRAME_T6_REG(sp)
	lw		t7, FRAME_T7_REG(sp)
	lw		t8, FRAME_T8_REG(sp)
	lw		t9, FRAME_T9_REG(sp)

	lw		gp, FRAME_GP_REG(sp)
	lw		ra, FRAME_RA_REG(sp)

	// Restore sp
	addu	sp, sp, FRAME_SIZE
	// return
	eret
	nop

	.set at
	.set reorder
END(except_ret)


LEAF(_splrestore)
	.set noreorder
	mfc0	v0, CP0_STATUS_REG
	and		a0, a0, CP0_STATUS_IM_MASK
	li		v1, ~CP0_STATUS_IM_MASK
	and		v1, v1, v0
	or		v1, v1, a0
	mtc0	v1, CP0_STATUS_REG
	and		v0, v0, CP0_STATUS_IM_MASK
	j		ra
	nop
	.set reorder
END(_splrestore)


LEAF(_splset)
	mfc0	v0, CP0_STATUS_REG
	and		a0, a0, (CP0_STATUS_IM_MASK | CP0_STATUS_IE_BIT)
	li		v1, ~(CP0_STATUS_IM_MASK | CP0_STATUS_IE_BIT)
	and		v1, v1, v0
	or		v1, v1, a0
	mtc0	v1, CP0_STATUS_REG
	and		v0, v0, (CP0_STATUS_IM_MASK | CP0_STATUS_IE_BIT)
	j		ra
	nop
END(_splset)


LEAF(_splget)
	mfc0	v0, CP0_STATUS_REG
	and		v0, v0, (CP0_STATUS_IM_MASK | CP0_STATUS_IE_BIT)
	j		ra
	nop
END(_splget)


LEAF(mips_int_lock)
	.set noreorder
	mfc0	v0, CP0_STATUS_REG
	li		v1, ~CP0_STATUS_IE_BIT
	and		v1, v1, v0
	mtc0	v1, CP0_STATUS_REG
	j		ra
	and		v0, v0, CP0_STATUS_IE_BIT
	.set reorder
END(mips_int_lock)


LEAF(mips_int_unlock)
	mfc0	v0, CP0_STATUS_REG
	and		a0, a0, CP0_STATUS_IE_BIT
	or		v0, v0, a0
	mtc0	v0, CP0_STATUS_REG
	j		ra
	nop
END(mips_int_unlock)




LEAF(mips_cp0_status_read)
	mfc0	v0, CP0_STATUS_REG
	j		ra
	nop
END(mips_cp0_status_read)

LEAF(mips_cp0_status_write)
	mtc0	a0, CP0_STATUS_REG
	nop
	nop
	j		ra
	nop
END(mips_cp0_status_write)


LEAF(mips_cp0_config1_read)
	mfc0	v0, CP0_CONFIG1_REG, CP0_CONFIG1_SEL
	j		ra
	nop
END(mips_cp0_config1_read)
