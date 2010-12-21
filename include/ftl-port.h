/*
 *	ftl-port.h -- defines to support the NAND Flash Translation Layer
 *
 *	(C) Copyright 2003, Pro Chao (proc@admtek.com.tw)
 *
 *	$Id: ftl-port.h,v 1.1.1.1 2003/10/8 10:22:53 pro Exp $
 */

#ifndef __MTD_FTL_H__
#define __MTD_FTL_H__

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

typedef unsigned char u_char;
typedef long loff_t;

#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

//

struct nand_oobinfo
{
	int useecc;
	int eccpos[6];
};

//
#define	SECTORSIZE      512

#define FTL_PARTN_BITS  4

//
#define NAND_CTL_SETNCE 	1
#define NAND_CTL_CLRNCE		2
#define NAND_CTL_SETCLE		3
#define NAND_CTL_CLRCLE		4
#define NAND_CTL_SETALE		5
#define NAND_CTL_CLRALE		6
#define NAND_CTL_SETWP		7
#define NAND_CTL_CLRWP		8

// control signal
//#define NAND_RW_REG		0x0	//data register
#define NAND_SET_CEn		0x1	//CE# low
#define NAND_CLR_CEn		0x2	//CE# high
#define NAND_CLR_CLE		0x3	//CLE low
#define NAND_SET_CLE		0x4	//CLE high
#define NAND_CLR_ALE		0x5	//ALE low
#define NAND_SET_ALE		0x6	//ALE high
#define NAND_SET_SPn		0x7	//SP# low (use spare area)
#define NAND_CLR_SPn		0x8	//SP# high (do not use spare area)
#define NAND_SET_WPn		0x9	//WP# low
#define NAND_CLR_WPn		0xA	//WP# high
#define NAND_STS_REG		0xB	//Status register

//
#define NAND_ECC_NONE		0
#define NAND_ECC_SOFT		1
#define NAND_ECC_HW3_256	2
#define NAND_ECC_HW3_512	3
#define NAND_ECC_HW6_512	4
#define NAND_ECC_DISKONCHIP	5

// page-shift
#define	NAND_PAGE_SHIFT		9		//512 bytes

//#define NULL	((void *) 0)

/* Block Control Information */
#define	SECTOR_FREE		0xff		// macros for Status & Status1 fields
#define	SECTOR_USED		0x55
#define	SECTOR_IGNORED	0x11
#define	SECTOR_DELETED	0x00

#define ERASE_MARK 0x3c69

struct ftl_bci
{
	__u8 ECCsig[6];
	__u8 Status;
	__u8 Status1;
} __attribute__ ((packed));

struct ftl_unithead1
{
	__u16 virtualUnitNo;
	__u16 prevUnitNo;
	__u8 ANAC;
	__u8 NACs;
	__u8 parityPerField;
	__u8 discarded;
} __attribute__ ((packed));

struct ftl_unithead2
{
	__u8 parityPerField;
	__u8 ANAC;
	__u16 prevUnitNo;
	__u16 virtualUnitNo;
	__u8 NACs;
	__u8 discarded;
} __attribute__ ((packed));

struct ftl_unittail
{
	__u8 Reserved[4];
	__u16 EraseMark;
	__u16 EraseMark1;
} __attribute__ ((packed));

union ftl_uci
{
	struct ftl_unithead1 a;
	struct ftl_unithead2 b;
	struct ftl_unittail c;
};

struct ftl_oob
{
	struct ftl_bci b;
	union ftl_uci u;
};


#endif /* __MTD_FTL_H__ */

