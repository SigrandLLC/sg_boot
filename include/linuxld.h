/*****************************************************************************
;
;    Project : Common Plateform
;    Creator : Daniel
;    File    : linuxld.h
;    Abstract: defines the specific items that loader nedded.
;
;*****************************************************************************/
#if __cplusplus
extern "C"
{
#endif

#ifndef _LINUXLD_H_
#define _LINUXLD_H_

#include <hw5120cfg.h>


/******************************************************************************************
#### System Map (SDRAM 32 Mbyte at least) ####

      0x0           +------------------------------------+	LINUXLD_SDRAM_START
                    |     Exception (8 KB)               |
      0x2000        +------------------------------------+	LINUXLD_KERNEL_START
                    |     Linux Kernel                   |
                    |       Code & Data Segment          |
                    |       (7 MB)                       |
                    |                                    |
      0x700000      +------------------------------------+	LINUXLD_MEMPOOL_START
                    |     Bootloader memory pool         |
                    |       (1 MB)                       |
                    |                                    |
      0x800000      +------------------------------------+	LINUXLD_LOADER_START
                    |     Boot loader                    |
                    |       Code & Data Segement         |
                    |       (92 KB)                      |
      0x818000      +------------------------------------+
                    |     Reserve Area (32KB)            |
                    |                                    |
      0x820000      +------------------------------------+	LINUXLD_DOWNLOAD_START
                    |     Download Buffer                |
                    |       (24 MB)                      |
                    |                                    |
      0x2000000     +------------------------------------+
*******************************************************************************************/
#define LINUXLD_EXCEP_SIZE		0x2000
#define LINUXLD_KERNEL_SIZE		(0x700000 - LINUXLD_EXCEP_SIZE)
#define LINUXLD_MEMPOOL_SIZE	0x100000
#define LINUXLD_LOADER_SIZE		0x18000
#define LINUXLD_RESERVE_SIZE	0x8000
#define LINUXLD_DOWNLOAD_SIZE	(0x1800000 - LINUXLD_LOADER_SIZE - LINUXLD_RESERVE_SIZE)

#define LINUXLD_SDRAM_START		PA2VA(ADM5120_SDRAM_BASE)
#define LINUXLD_KERNEL_START	(LINUXLD_SDRAM_START + LINUXLD_EXCEP_SIZE)
#define LINUXLD_MEMPOOL_START	(LINUXLD_KERNEL_START + LINUXLD_KERNEL_SIZE)
#define LINUXLD_LOADER_START	(LINUXLD_MEMPOOL_START + LINUXLD_MEMPOOL_SIZE)
#define LINUXLD_DOWNLOAD_START	(LINUXLD_LOADER_START + LINUXLD_LOADER_SIZE + LINUXLD_RESERVE_SIZE)

#define RUNTIME_IMG_ADDR		(LINUXLD_LOADER_START + 0x1000)

/********************************************************************************************
#### System Map (NAND Flash) ####

	+-------------------------+  LINUXLD_NANDFLASH_LOADER_START
	|       Linuxld (96k)     |
	+-------------------------+  LINUXLD_NANDFLASH_BOOTPARAM_START
	|       Boot param (32k)  |
	+-------------------------+  LINUXLD_NANDFLASH_KERNEL_START
	|                         |
	| Linux kernel (4MB)      |
	|                         |
	+-------------------------+
 *********************************************************************************************/

#define LINUXLD_NANDFLASH_LOADER_SIZE		0x018000 // 96 Kb old: 0x14000 // 80 Kb
#define LINUXLD_NANDFLASH_BOOTPARAM_SIZE	0x008000 // 32 Kb old: 0x0C000 // 48 Kb
#define LINUXLD_NANDFLASH_KERNEL_SIZE		0x400000 // 4MB, enough for kernel, old: 2MB


#define LINUXLD_NANDFLASH_LOADER_START		0x0
#define LINUXLD_NANDFLASH_BOOTPARAM_START	(LINUXLD_NANDFLASH_LOADER_START + LINUXLD_NANDFLASH_LOADER_SIZE)
#define LINUXLD_NANDFLASH_KERNEL_START		(LINUXLD_NANDFLASH_BOOTPARAM_START + LINUXLD_NANDFLASH_BOOTPARAM_SIZE)


#ifndef __ASSEMBLER__
int  tftpc_download    (int mode);
void check_for_bad     (void);
void flash_erase_all   (void);
void find_bad_blocks   (void);
void create_bad_blocks (void);
void boot_linux        (void);
#endif

#endif /* _LINUXLD_H */



#if __cplusplus
}
#endif
