#ifndef __MIPS_H__
#define __MIPS_H__


/*  MIPS general register  */
#define zero	$0	/* always zero */
#define AT	$at	/* assembler temporary */
#define v0	$2	/* return value */
#define v1	$3
#define a0	$4	/* argument registers */
#define a1	$5
#define a2	$6
#define a3	$7
#define t0	$8	/* temp registers (not saved across subroutine calls) */
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15
#define s0	$16	/* saved across subroutine calls (callee saved) */
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define t8	$24	/* two more temporary registers */
#define t9	$25
#define k0	$26	/* kernel temporary */
#define k1	$27
#define gp	$28	/* global pointer */
#define sp	$29	/* stack pointer */
#define s8	$30	/* one more callee saved */
#define ra	$31	/* return address */


#endif
