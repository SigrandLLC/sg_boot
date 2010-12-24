/*****************************************************************************
;
;    Project : Am5120
;    Creator : Jeanson Hung
;    File    : nand.h
;    Abstract:
;
;*****************************************************************************/
#include "adm5120.h"

#ifndef __NAND_H__
#define __NAND_H__

/*typedef unsigned char UINT8;
typedef unsigned long UINT32;*/

#define NAND_REG_BASE		PA2VA(ADM5120_SMEM1_BASE)

// NAND boot initial size, 2K for 5120
#define NAND_BOOT_SIZE		0x800

#define K9F5608U0B

#ifdef K9F5608U0B
#define NAND_FLASH_SIZE		0x2000000	//32MB+1MB // revised by ProChao, 10/8/2003
#define NAND_PAGE_SIZE		0x200		//512+16Bytes
#define	NAND_PAGE_OOB_SIZE	0x10		//16Bytes
#define NAND_PAGE_PER_BLK	0x20		//32 pages per block
#define NAND_SIZE_PER_BLK	0x4000		//16k per block
#define NAND_FLASH_BLOCK_NO	2048		//2048 blocks per device
#endif

#ifdef K9F6408U0C
#define NAND_FLASH_SIZE		0x800000	//8MB+512K
#define NAND_PAGE_SIZE		0x200		//512+16Bytes
#define	NAND_PAGE_OOB_SIZE	0x10		//16Bytes
#define NAND_PAGE_PER_BLK	0x10		//16 pages per block
#endif

/* check NAND flash parameters */
#if NAND_SIZE_PER_BLK == 0
#error DID NOT DEFINE THE CORRECT NAND FLASH PARAMETERS!!!
#endif


#define CMD_READ1			0x0
#define CMD_READ2			0x1
#define CMD_READ3			0x50
#define CMD_ERASE1			0x60
#define CMD_ERASE2			0xD0
#define CMD_WRITE1			0x80
#define CMD_WRITE2			0x10

// ported by ProChao, 10/8/2003, from Linux platform
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_RESET		0xff

//
#define NAND_BADBLOCK_POS	4	//6	//5
// Думаю, что в соответствии со структурой ftl_bci, это значение должно быть 6

//
#define ADD1MASK			0x000000ff
#define ADD2MASK			0x0001fe00
#define ADD3MASK			0x01fe0000
#define ADD2SHIFT			0x9
#define ADD3SHIFT			0x11

// ported by ProChao, 10/9/2003

/*
 * Enumeration for NAND flash chip state
 */
typedef enum
{
	FL_READY,
	FL_READING,
	FL_WRITING,
	FL_ERASING,
	FL_SYNCING
} nand_state_t;

int nand_read (UINT8 *dst, UINT8 *src, UINT32 len);
int nand_write (UINT8 *dst, UINT8 *src, UINT32 size, UINT8 wp);
int nand_erase (UINT8 *addr, UINT32 len, UINT8 wp);
void nand_write_boot (UINT8 *dst, UINT8 *src, UINT32 len);
void check_bad_block (void);
void scan_bad_blocks (void);
void bad_block       (void);
void nand_calculate_ecc (const unsigned char *dat, unsigned char *ecc_code);
int  nand_correct_data (unsigned char *dat, unsigned char *read_ecc, unsigned char *calc_ecc);


#endif

