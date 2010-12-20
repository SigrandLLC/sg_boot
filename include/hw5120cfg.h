#ifndef __HW5120CFG_H__
#define __HW5120CFG_H__

#include <memdev.h>


#define OPTION_ENABLE						1
#define OPTION_DISABLE						0


//=================  5120 Core configuration  ================
// CPU clock options
#define _CPU_CLOCK_175M				175000000
#define _CPU_CLOCK_200M				200000000
#define _CPU_CLOCK_225M				225000000
#define _CPU_CLOCK_250M				250000000
// CPU clock options

//<< HWPF_5120_CPU_CLOCK
#define HWPF_5120_CPU_CLOCK     _CPU_CLOCK_175M
//>> HWPF_5120_CPU_CLOCK
//==============  End of 5106 Core configuration  =============


//***************************************************************************
// Memory configuration sections start
//***************************************************************************

//===========================================================================
//  Flash configuration section start
#ifdef FLASH_4M
#define BOARD_NOR_FLASH_SIZE    0x400000
#endif

#ifdef FLASH_2M
#define BOARD_NOR_FLASH_SIZE    0x200000
#endif




//******************************************************
// Size of static memory bank 0 (Unit: Byte)
#define HWPF_5120_SMEM0_BANK_SIZE					BOARD_NOR_FLASH_SIZE

// Data bus width of static memory bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_SMEM0_BUS_WIDTH					16

// Device data bus width of static memory bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_SMEM0_DEV_WIDTH					16

// Chip select polarity	[legal value are 0(low active) and 1(high active)]
#define HWPF_5120_SMEM0_CS_POLARITY					0

// Asyn page mode	[legal value are OPTION_DISABLE and OPTION_ENABLE]
#define HWPF_5120_SMEM0_PAGE_MODE					OPTION_DISABLE

// Chip select to Output enable wait state:
//		(HWPF_5120_SMEM0_CS_TO_OE_WAIT) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM0_CS_TO_OE_WAIT				0

// Read Wait state. (HWPF_5120_SMEM0_READ_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_SMEM0_READ_WAIT					9

// Chip select to Write enable wait state:
//		(HWPF_5120_SMEM0_CS_TO_WE_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM0_CS_TO_WE_WAIT				0

// Write Wait state. (HWPF_5120_SMEM0_WRITE_WAIT + 2) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_SMEM0_WRITE_WAIT					9

// Bus turnaround Wait state. (HWPF_5120_SMEM0_TURN_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM0_TURN_WAIT					5



//******************************************************
// Size of static memory bank 1 (Unit: Byte)
// Enable or Disable bank 1
#define HWPF_5120_SMEM1_EN							OPTION_DISABLE

// static memory bank1 connected to a NAND flash
#define HWPF_5120_SMEM1_NAND						OPTION_DISABLE

// Size of static memory bank 1 (Unit: Byte)
#define HWPF_5120_SMEM1_BANK_SIZE					0x200000

// Bus width of static memory bank 1. (legal value are 8, 16 and 32)
// HWPF_5120_SMEM1_BUS_WIDTH must be 8 when HWPF_5120_SMEM1_NAND is enabled.
#define HWPF_5120_SMEM1_BUS_WIDTH					16

// Device data bus width of static memory bank 1. (legal value are 8, 16 and 32)
// HWPF_5120_SMEM1_DEV_WIDTH must be 8 when HWPF_5120_SMEM1_NAND is enabled.
#define HWPF_5120_SMEM1_DEV_WIDTH					16

// Chip select polarity	[legal value are 0(low active) and 1(high active)]
#define HWPF_5120_SMEM1_CS_POLARITY					0

// Asyn page mode	[legal value are OPTION_DISABLE and OPTION_ENABLE]
#define HWPF_5120_SMEM1_PAGE_MODE					OPTION_DISABLE

// Chip select to Output enable wait state:
//		(HWPF_5120_SMEM1_CS_TO_OE_WAIT) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM1_CS_TO_OE_WAIT				0

// Read Wait state. (HWPF_5120_SMEM1_READ_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_SMEM1_READ_WAIT					9


// Chip select to Write enable wait state:
//		(HWPF_5120_SMEM1_CS_TO_WE_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM1_CS_TO_WE_WAIT				0

// Write Wait state. (HWPF_5120_SMEM1_WRITE_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_SMEM1_WRITE_WAIT					9

// Bus turnaround Wait state. (HWPF_5120_SMEM1_TURN_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_SMEM1_TURN_WAIT					1


//  Flash configuration section end
//===========================================================================


//===========================================================================
//  SDRAM configuration section start


//********************************************* SDRAM configuration of Bank 0
// RAS and CAS latency, ref: SDRAM data sheet.
// SDRAM CAS latency, legal value are 2 or 3 (clock)
// CAS latency 2 clock time could be applied to most SDRAM with clock rate below 100M Hz.
#define HWPF_5120_SDRAM0_CAS_LATENCY				2

// RAS to CAS latency must be greater than tRCD (Active to read/write delay)
// SDRAM RAS to CAS latency, legal value are 1, 2, or 3 (Clock)
#define HWPF_5120_SDRAM0_RAS_LATENCY				2

// Bus width of SDRAM bank0. legal values are 16 or 32
#define HWPF_5120_SDRAM0_BUS_WIDTH					BOARD_SDRAM0_BUS_WIDTH

// memory size of SDRAM chip in Mbits
#define HWPF_5120_SDRAM0_DEV_SIZE					BOARD_SDRAM0_DEV_SIZE

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define HWPF_5120_SDRAM0_DEV_DATA_WIDTH				BOARD_SDRAM0_DEV_DATA_WIDTH

//******************************************** SDRAM configuration of Bank 1
// RAS and CAS latency, ref: SDRAM data sheet.
// SDRAM bank 1 enable.
#define HWPF_5120_SDRAM1_EN						    BOARD_5120_SDRAM1_STATUS

// SDRAM CAS latency, legal value are 2 or 3 (clock)
// CAS latency 2 clock time could be applied to most SDRAM with clock rate below 100M Hz.
#define HWPF_5120_SDRAM1_CAS_LATENCY				2

// RAS to CAS latency must be greater than tRCD (Active to read/write delay)
// SDRAM RAS to CAS latency, legal value are 1, 2, or 3 (Clock)
#define HWPF_5120_SDRAM1_RAS_LATENCY				2

// Bus width of SDRAM bank0. legal values are 16 or 32
#define HWPF_5120_SDRAM1_BUS_WIDTH					BOARD_SDRAM1_BUS_WIDTH

// memory size of SDRAM chip in Mbits
#define HWPF_5120_SDRAM1_DEV_SIZE					BOARD_SDRAM1_DEV_SIZE

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define HWPF_5120_SDRAM1_DEV_DATA_WIDTH				BOARD_SDRAM1_DEV_DATA_WIDTH


//******************************************** SDRAM access timing parameters
// SDRAM refresh time. (Time unit: 16 SDRAM clock)
// Assume 100M Hz clock, refresh time 15.625 us
#define HWPF_5120_SDRAM_RFTIME						85

// Modify the following parameters according to the SDRAM device choosen.
// The time unit is the system clock.

// PRECHARGE command period. (HWPF_5120_SDRAM_tRP + 1) clock cycles.
// Range: [0-15]
#define HWPF_5120_SDRAM_tRP							1

// ACTIVE to PRECHARGE command. (HWPF_5120_SDRAM_tRAS + 1) clock cycles.
// Range [0-15]
#define HWPF_5120_SDRAM_tRAS						4

// Self-REFRESH exit time. (HWPF_5120_SDRAM_tSREX + 1) clock cycles.
// Range [0-15]
#define HWPF_5120_SDRAM_tSREX						15

// Last-Data-Out to ACTIVE command. (HWPF_5120_SDRAM_tAPR + 1) clock cycles.
// Range [0-15]
#define HWPF_5120_SDRAM_tAPR						1

// Data-in to ACTIVE command. (HWPF_5120_SDRAM_tDAL + 1) clock cycles.
// Range [0-15]
#define HWPF_5120_SDRAM_tDAL						3

// WRITE recovery time. (HWPF_5120_SDRAM_tWR + 1) clock cycles.
// Range [0-15]
#define HWPF_5120_SDRAM_tWR							1

// ACTIVE to ACTIVE command period. (HWPF_5120_SDRAM_tRC + 1) clock cycles.
// Range[0-31]
#define HWPF_5120_SDRAM_tRC							6

// AUTO REFRESH period. (HWPF_5120_SDRAM_tRFC + 1) clock cycles.
// Range[0-31]
#define HWPF_5120_SDRAM_tRFC						6

// Exit SELF-REFRESH to ACTIVE command. (HWPF_5120_SDRAM_tXSR + 1) clock cycles.
// Range[0-31]
#define HWPF_5120_SDRAM_tXSR						7

// ACTIVE bank a to ACTIVE bank b command. (HWPF_5120_SDRAM_tRRD + 1) clock cycles.
// Range[0-15]
#define HWPF_5120_SDRAM_tRRD						1

// Load MODE register to ACTIVE command. (HWPF_5120_SDRAM_tMRD + 1) clock cycles.
// Range[0-15]
#define HWPF_5120_SDRAM_tMRD						1


//  SDRAM configuration section end
//===========================================================================
//***************************************************************************
// Memory configuration sections end
//***************************************************************************

//***************************************************************************
// Ext I/O configuration sections start
//***************************************************************************

//******************************************************
// Ext I/O bank 0 start
// Enable or Disable bank 0
#define HWPF_5120_EXTIO0_EN							OPTION_DISABLE

// Interrupt polarity [legal value are 0(low active) and 1(high active)]
#define HWPF_5120_EXTIO0_INT_POLARITY				1

// Enable wait control
// This OPTION is ORed with HWPF_5120_EXTIO1_WAIT_EN!!
#define HWPF_5120_EXTIO0_WAIT_EN					OPTION_DISABLE

// Bus width of Ext I/o bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_EXTIO0_BUS_WIDTH					8

// Device data bus width of Ext I/o bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_EXTIO0_DEV_WIDTH					8

// Chip select polarity	[legal value are 0(low active) and 1(high active)]
#define HWPF_5120_EXTIO0_CS_POLARITY				0

// Asyn page mode	[legal value are OPTION_DISABLE and OPTION_ENABLE]
#define HWPF_5120_EXTIO0_PAGE_MODE					OPTION_DISABLE

// Chip select to Output enable wait state:
//		(HWPF_5120_SMEM0_CS_TO_OE_WAIT) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO0_CS_TO_OE_WAIT				0

// Read Wait state. (HWPF_5120_EXTIO0_READ_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_EXTIO0_READ_WAIT					3

// Chip select to Write enable wait state:
//		(HWPF_5120_EXTIO0_CS_TO_WE_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO0_CS_TO_WE_WAIT				0

// Write Wait state. (HWPF_5120_EXTIO0_WRITE_WAIT + 2) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_EXTIO0_WRITE_WAIT					3

// Bus turnaround Wait state. (HWPF_5120_EXTIO0_TURN_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO0_TURN_WAIT					1

// Ext I/O bank 0 end
//******************************************************

//******************************************************
// Ext I/O bank 1 start
// Enable or Disable bank 1
#define HWPF_5120_EXTIO1_EN							OPTION_DISABLE

// Interrupt polarity [legal value are 0(low active) and 1(high active)]
#define HWPF_5120_EXTIO1_INT_POLARITY				0

// Enable wait control
// This OPTION is ORed with HWPF_5120_EXTIO0_WAIT_EN!!
#define HWPF_5120_EXTIO1_WAIT_EN					OPTION_DISABLE

// Bus width of Ext I/o bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_EXTIO1_BUS_WIDTH					16

// Device data bus width of Ext I/o bank 0. (legal value are 8, 16 and 32)
#define HWPF_5120_EXTIO1_DEV_WIDTH					16

// Chip select polarity	[legal value are 0(low active) and 1(high active)]
#define HWPF_5120_EXTIO1_CS_POLARITY				0

// Asyn page mode	[legal value are OPTION_DISABLE and OPTION_ENABLE]
#define HWPF_5120_EXTIO1_PAGE_MODE					OPTION_DISABLE

// Chip select to Output enable wait state:
//		(HWPF_5120_SMEM0_CS_TO_OE_WAIT) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO1_CS_TO_OE_WAIT				0

// Read Wait state. (HWPF_5120_EXTIO0_READ_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_EXTIO1_READ_WAIT					9

// Chip select to Write enable wait state:
//		(HWPF_5120_EXTIO0_CS_TO_WE_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO1_CS_TO_WE_WAIT				0

// Write Wait state. (HWPF_5120_EXTIO0_WRITE_WAIT + 2) wait state will be inserted.
// Unit: clock, range [0-31]
#define HWPF_5120_EXTIO1_WRITE_WAIT				9

// Bus turnaround Wait state. (HWPF_5120_EXTIO0_TURN_WAIT + 1) wait state will be inserted.
// Unit: clock, range [0-15]
#define HWPF_5120_EXTIO1_TURN_WAIT					1

// Ext I/O bank 0 end
//******************************************************

//***************************************************************************
// Ext I/O configuration sections end
//***************************************************************************


//***************************************************************************
// GPIO configuration sections start
//***************************************************************************

// GPIO0_INPUT_MODE/GPIO0_OUTPUT_MODE
#define HWPF_5120_GPIO0_MODE						GPIO0_INPUT_MODE

// GPIO0_OUTPUT_LOW/GPIO0_OUTPUT_HI
#define HWPF_5120_GPIO0_DEFOUT						GPIO0_OUTPUT_LOW

// GPIO1_INPUT_MODE/GPIO1_OUTPUT_MODE
#define HWPF_5120_GPIO1_MODE						GPIO1_INPUT_MODE

// GPIO1_OUTPUT_LOW/GPIO1_OUTPUT_HI
#define HWPF_5120_GPIO1_DEFOUT						GPIO1_OUTPUT_LOW

// GPIO2_INPUT_MODE/GPIO2_OUTPUT_MODE
#define HWPF_5120_GPIO2_MODE						GPIO2_INPUT_MODE

// GPIO2_OUTPUT_LOW/GPIO2_OUTPUT_HI
#define HWPF_5120_GPIO2_DEFOUT						GPIO2_OUTPUT_LOW

// GPIO3_INPUT_MODE/GPIO2_OUTPUT_MODE
#define HWPF_5120_GPIO3_MODE						GPIO3_INPUT_MODE

// GPIO3_OUTPUT_LOW/GPIO3_OUTPUT_HI
#define HWPF_5120_GPIO3_DEFOUT						GPIO3_OUTPUT_LOW

// GPIO4_INPUT_MODE/GPIO4_OUTPUT_MODE
#define HWPF_5120_GPIO4_MODE						GPIO4_INPUT_MODE

// GPIO4_OUTPUT_LOW/GPIO4_OUTPUT_HI
#define HWPF_5120_GPIO4_DEFOUT						GPIO4_OUTPUT_LOW

// GPIO5_INPUT_MODE/GPIO5_OUTPUT_MODE
#define HWPF_5120_GPIO5_MODE						GPIO5_INPUT_MODE

// GPIO5_OUTPUT_LOW/GPIO5_OUTPUT_HI
#define HWPF_5120_GPIO5_DEFOUT						GPIO5_OUTPUT_LOW

// GPIO6_INPUT_MODE/GPIO6_OUTPUT_MODE
#define HWPF_5120_GPIO6_MODE						GPIO6_INPUT_MODE

// GPIO6_OUTPUT_LOW/GPIO6_OUTPUT_HI
#define HWPF_5120_GPIO6_DEFOUT						GPIO6_OUTPUT_LOW

// GPIO7_INPUT_MODE/GPIO7_OUTPUT_MODE
#define HWPF_5120_GPIO7_MODE						GPIO7_INPUT_MODE

// GPIO7_OUTPUT_LOW/GPIO7_OUTPUT_HI
#define HWPF_5120_GPIO7_DEFOUT						GPIO7_OUTPUT_LOW


//***************************************************************************
// GPIO configuration sections end
//***************************************************************************


//***************************************************************************
// PORT LED configuration sections start
//***************************************************************************
/* Available options are listed below
	 GPIOL_INPUT_MODE
	 GPIOL_OUTPUT_FLASH
	 GPIOL_OUTPUT_LOW
	 GPIOL_OUTPUT_HIGH
	 GPIOL_LINK_LED
	 GPIOL_SPEED_LED
	 GPIOL_DUPLEX_LED
	 GPIOL_ACT_LED
	 GPIOL_COL_LED
	 GPIOL_LINK_ACT_LED
	 GPIOL_DUPLEX_COL_LED
	 GPIOL_10MLINK_ACT_LED
	 GPIOL_100MLINK_ACT_LED
 */

//******************************************************
// PORT0 LED start
#define HWPF_5120_P0_LED0_MODE						GPIOL_LINK_ACT_LED
#define HWPF_5120_P0_LED1_MODE						GPIOL_SPEED_LED
#define HWPF_5120_P0_LED2_MODE						GPIOL_DUPLEX_COL_LED
// PORT0 LED end
//******************************************************

//******************************************************
// PORT1 LED start
#define HWPF_5120_P1_LED0_MODE						GPIOL_LINK_ACT_LED
#define HWPF_5120_P1_LED1_MODE						GPIOL_SPEED_LED
#define HWPF_5120_P1_LED2_MODE						GPIOL_DUPLEX_COL_LED
// PORT1 LED end
//******************************************************

//******************************************************
// PORT2 LED start
#define HWPF_5120_P2_LED0_MODE						GPIOL_LINK_ACT_LED
#define HWPF_5120_P2_LED1_MODE						GPIOL_SPEED_LED
#define HWPF_5120_P2_LED2_MODE						GPIOL_DUPLEX_COL_LED
// PORT2 LED end
//******************************************************
//******************************************************
// PORT3 LED start
#define HWPF_5120_P3_LED0_MODE						GPIOL_LINK_ACT_LED
#define HWPF_5120_P3_LED1_MODE						GPIOL_SPEED_LED
#define HWPF_5120_P3_LED2_MODE						GPIOL_DUPLEX_COL_LED
// PORT3 LED end
//******************************************************
//******************************************************
// PORT4 LED start
#define HWPF_5120_P4_LED0_MODE						GPIOL_LINK_ACT_LED
#define HWPF_5120_P4_LED1_MODE						GPIOL_SPEED_LED
#define HWPF_5120_P4_LED2_MODE						GPIOL_DUPLEX_COL_LED
// PORT4 LED end
//******************************************************

//***************************************************************************
// PORT LED configuration sections end
//***************************************************************************

//=================  Switch grouping configuration  ================
#define IF_BRIDGE_DISABLE				0
#define IF_BRIDGE_ATTACH				0x10
#define IF_BRIDGE_HOSTIF				0x20

//  HWPF_5120_IF_NUM MUST be a number between 0 and 6
//<< HWPF_5120_IF_NUM
#define HWPF_5120_IF_NUM                     2
//>> HWPF_5120_IF_NUM

//<< HWPF_5120_IF_PORTMASK
#define HWPF_5120_IF0_PORTMASK         0x06
#define HWPF_5120_IF0_BRIDGE_FG        (IF_BRIDGE_ATTACH | IF_BRIDGE_HOSTIF)
#define HWPF_5120_IF1_PORTMASK         0x01
#define HWPF_5120_IF1_BRIDGE_FG        IF_BRIDGE_DISABLE
#define HWPF_5120_IF2_PORTMASK			0x38
#define HWPF_5120_IF2_BRIDGE_FG        IF_BRIDGE_ATTACH

//>> HWPF_5120_IF_PORTMASK



// !!!! Do not modify the following sections !!!!
// All configurable parameters are listed in the configuration secions above!!

//===========================================================================
// ++++++++++++++++++++++   SMEM bank0 start   ++++++++++++++++++++++
#if (HWPF_5120_SMEM0_BANK_SIZE <= 0x80000)
#define SMEM0_BANK_SIZE								ROM_SIZE_512KBYTES
#elif (HWPF_5120_SMEM0_BANK_SIZE <= 0x100000)
#define SMEM0_BANK_SIZE								ROM_SIZE_1MBYTES
#elif (HWPF_5120_SMEM0_BANK_SIZE <= 0x200000)
#define SMEM0_BANK_SIZE								ROM_SIZE_2MBYTES
#elif (HWPF_5120_SMEM0_BANK_SIZE <= 0x400000)
#define SMEM0_BANK_SIZE								ROM_SIZE_4MBYTES
#else
#error	SRAM0 which size is larger than 4M bytes is not supported!!
#endif	// (HWPF_5120_SMEM0_BANK_SIZE <= 0x80000)

#if (HWPF_5120_SMEM0_BUS_WIDTH == 32)
#define SMEM0_BUS_WIDTH							SM_MEM_WIDTH_32BIT
#elif (HWPF_5120_SMEM0_BUS_WIDTH == 16)
#define SMEM0_BUS_WIDTH							SM_MEM_WIDTH_16BIT
#elif (HWPF_5120_SMEM0_BUS_WIDTH == 8)
#define SMEM0_BUS_WIDTH							SM_MEM_WIDTH_8BIT
#else
#error The value of HWPF_5120_SMEM0_BUS_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_SMEM0_BUS_WIDTH == 32)

#if (HWPF_5120_SMEM0_DEV_WIDTH == 32)
#define SMEM0_PB									SM_PB
#elif (HWPF_5120_SMEM0_DEV_WIDTH == 16)
#define SMEM0_PB									SM_PB
#elif (HWPF_5120_SMEM0_DEV_WIDTH == 8)
#define SMEM0_PB									0
#else
#error The value of HWPF_5120_SMEM0_DEV_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_SMEM0_DEV_WIDTH == 32)

#if (HWPF_5120_SMEM0_BUS_WIDTH < HWPF_5120_SMEM0_DEV_WIDTH)
#warning The data bus width of static memory bank 0 is smaller than the width of the device !!!
#endif


#if HWPF_5120_SMEM0_CS_POLARITY == 1
#define SMEM0_CS_POLARITY							SM_CS_HIGH
#elif HWPF_5120_SMEM0_CS_POLARITY == 0
#define SMEM0_CS_POLARITY							0
#else
#error The value of HWPF_5120_SMEM0_CS_POLARITY must be either 1 or 0!!!
#endif  // (HWPF_5120_SMEM0_CS_POLARITY == 1)

#if HWPF_5120_SMEM0_PAGE_MODE == OPTION_ENABLE
#define SMEM0_PM_MODE								SM_PAGE_MODE
#elif HWPF_5120_SMEM0_PAGE_MODE == OPTION_DISABLE
#define SMEM0_PM_MODE								0
#else
#error The value of HWPF_5120_SMEM0_PAGE_MODE must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif  // (HWPF_5120_SMEM0_PAGE_MODE == 1)

#if (HWPF_5120_SMEM0_CS_TO_WE_WAIT < 0) || (HWPF_5120_SMEM0_CS_TO_WE_WAIT > 15)
#error HWPF_5120_SMEM0_CS_TO_WE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM0_CS_TO_OE_WAIT < 0) || (HWPF_5120_SMEM0_CS_TO_OE_WAIT > 15)
#error HWPF_5120_SMEM0_CS_TO_OE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM0_READ_WAIT < 0) || (HWPF_5120_SMEM0_READ_WAIT > 31)
#error HWPF_5120_SMEM0_READ_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM0_WRITE_WAIT < 0) || (HWPF_5120_SMEM0_WRITE_WAIT >31)
#error HWPF_5120_SMEM0_WRITE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM0_TURN_WAIT < 0) || (HWPF_5120_SMEM0_TURN_WAIT > 15)
#error HWPF_5120_SMEM0_TURN_WAIT is out of range!!!
#endif


#define SMEM0_CFG									( SMEM0_BUS_WIDTH 		\
													| SMEM0_CS_POLARITY 	\
													| SMEM0_PM_MODE 		\
													| SMEM0_PB)



#define SmemBank0_Config							SMEM0_CFG
#define	SmemBank0_WaitWen							HWPF_5120_SMEM0_CS_TO_WE_WAIT
#define	SmemBank0_WaitOen							HWPF_5120_SMEM0_CS_TO_OE_WAIT
#define	SmemBank0_WaitRd							HWPF_5120_SMEM0_READ_WAIT
#define	SmemBank0_WaitPage							31
#define	SmemBank0_WaitWr							HWPF_5120_SMEM0_WRITE_WAIT
#define	SmemBank0_WaitTurn							HWPF_5120_SMEM0_TURN_WAIT

#define HWPF_5120_SMEM0_BASE						ADM5120_SMEM0_BASE
#define HWPF_5120_SMEM0_SIZE						HWPF_5120_SMEM0_BANK_SIZE

// ++++++++++++++++++++++   SMEM bank0 end   ++++++++++++++++++++++
//===========================================================================


//===========================================================================
// ++++++++++++++++++++++   SMEM bank1 start   ++++++++++++++++++++++
#if (HWPF_5120_SMEM1_EN == OPTION_ENABLE)

#if (HWPF_5120_SMEM1_NAND == OPTION_ENABLE)
// NAND FLASH
#define SMEM1_BANK_SIZE							ROM_SIZE_512KBYTES
#define SMEM1_BUS_WIDTH							SM_MEM_WIDTH_8BIT
#define SMEM1_PB								SM_PB

#else		/*	!!(HWPF_5120_SMEM1_NAND == OPTION_ENABLE) */

#if (HWPF_5120_SMEM1_BANK_SIZE <= 0x80000)
#define SMEM1_BANK_SIZE								ROM_SIZE_512KBYTES
#elif (HWPF_5120_SMEM1_BANK_SIZE <= 0x100000)
#define SMEM1_BANK_SIZE								ROM_SIZE_1MBYTES
#elif (HWPF_5120_SMEM1_BANK_SIZE <= 0x200000)
#define SMEM1_BANK_SIZE								ROM_SIZE_2MBYTES
#elif (HWPF_5120_SMEM1_BANK_SIZE <= 0x400000)
#define SMEM1_BANK_SIZE								ROM_SIZE_4MBYTES
#elif (HWPF_5120_SMEM1_BANK_SIZE <= 0x800000)
#define SMEM1_BANK_SIZE								ROM_SIZE_8MBYTES
#else
#error	SRAM1 which size is larger than 8M bytes is not supported!!
#endif	// (HWPF_5120_SMEM1_BANK_SIZE <= 0x80000)


#if (HWPF_5120_SMEM1_BUS_WIDTH == 32)
#define SMEM1_BUS_WIDTH							SM_MEM_WIDTH_32BIT
#elif (HWPF_5120_SMEM1_BUS_WIDTH == 16)
#define SMEM1_BUS_WIDTH							SM_MEM_WIDTH_16BIT
#elif (HWPF_5120_SMEM1_BUS_WIDTH == 8)
#define SMEM1_BUS_WIDTH							SM_MEM_WIDTH_8BIT
#else
#error The value of HWPF_5120_SMEM1_BUS_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_SMEM1_BUS_WIDTH == 32)


#if (HWPF_5120_SMEM1_DEV_WIDTH == 32)
#define SMEM1_PB									SM_PB
#elif (HWPF_5120_SMEM1_DEV_WIDTH == 16)
#define SMEM1_PB									SM_PB
#elif (HWPF_5120_SMEM1_DEV_WIDTH == 8)
#define SMEM1_PB									0
#else
#error The value of HWPF_5120_SMEM1_DEV_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_SMEM1_DEV_WIDTH == 32)


#if (HWPF_5120_SMEM1_BUS_WIDTH < HWPF_5120_SMEM1_DEV_WIDTH)
#warning The data bus width of static memory bank 1 is smaller than the width of the device !!!
#endif

#endif		/*	!!(HWPF_5120_SMEM1_NAND == OPTION_ENABLE) */


#if HWPF_5120_SMEM1_CS_POLARITY == 1
#define SMEM1_CS_POLARITY							SM_CS_HIGH
#elif HWPF_5120_SMEM1_CS_POLARITY == 0
#define SMEM1_CS_POLARITY							0
#else
#error The value of HWPF_5120_SMEM1_CS_POLARITY must be either 1 or 0!!!
#endif  // (HWPF_5120_SMEM1_CS_POLARITY == 1)

#if HWPF_5120_SMEM1_PAGE_MODE == OPTION_ENABLE
#define SMEM1_PM_MODE								SM_PAGE_MODE
#elif HWPF_5120_SMEM1_PAGE_MODE == OPTION_DISABLE
#define SMEM1_PM_MODE								0
#else
#error The value of HWPF_5120_SMEM1_PAGE_MODE must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif  // (HWPF_5120_SMEM1_PAGE_MODE == 1)

#if (HWPF_5120_SMEM1_CS_TO_WE_WAIT < 0) || (HWPF_5120_SMEM1_CS_TO_WE_WAIT > 15)
#error HWPF_5120_SMEM1_CS_TO_WE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM1_CS_TO_OE_WAIT < 0) || (HWPF_5120_SMEM1_CS_TO_OE_WAIT > 15)
#error HWPF_5120_SMEM1_CS_TO_OE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM1_READ_WAIT < 0) || (HWPF_5120_SMEM1_READ_WAIT > 31)
#error HWPF_5120_SMEM1_READ_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM1_WRITE_WAIT < 0) || (HWPF_5120_SMEM1_WRITE_WAIT >31)
#error HWPF_5120_SMEM1_WRITE_WAIT is out of range!!!
#endif

#if (HWPF_5120_SMEM1_TURN_WAIT < 0) || (HWPF_5120_SMEM1_TURN_WAIT > 15)
#error HWPF_5120_SMEM1_TURN_WAIT is out of range!!!
#endif


#define SMEM1_CFG									( SMEM1_BUS_WIDTH 		\
													| SMEM1_CS_POLARITY 	\
													| SMEM1_PM_MODE 		\
													| SMEM1_PB)



#define SmemBank1_Config							SMEM1_CFG
#define	SmemBank1_WaitWen							HWPF_5120_SMEM1_CS_TO_WE_WAIT
#define	SmemBank1_WaitOen							HWPF_5120_SMEM1_CS_TO_OE_WAIT
#define	SmemBank1_WaitRd							HWPF_5120_SMEM1_READ_WAIT
#define	SmemBank1_WaitPage							31
#define	SmemBank1_WaitWr							HWPF_5120_SMEM1_WRITE_WAIT
#define	SmemBank1_WaitTurn							HWPF_5120_SMEM1_TURN_WAIT


#define HWPF_5120_FLASH_BANK_NUM					2

#define HWPF_5120_ROMSIZE_CFG						( (SMEM0_BANK_SIZE << ROM0_SIZE_SHIFT)	\
													| (SMEM1_BANK_SIZE << ROM1_SIZE_SHIFT))

#else	// (HWPF_5120_SMEM1_EN == OPTION_ENABLE)
#define HWPF_5120_FLASH_BANK_NUM					1
#define HWPF_5120_ROMSIZE_CFG						( (SMEM0_BANK_SIZE << ROM0_SIZE_SHIFT)	\
													| (ROM_SIZE_DISABLE << ROM1_SIZE_SHIFT))
#endif	// ! (HWPF_5120_SMEM1_EN == OPTION_ENABLE)
// ++++++++++++++++++++++   SMEM bank1 end   ++++++++++++++++++++++

//===========================================================================


//===========================================================================
// ++++++++++++++++++++++   SDRAM bank0 start   ++++++++++++++++++++++
// SDRAM bank 0 configuration
#ifndef HWPF_5120_SDRAM0_CAS_LATENCY
#error HWPF_5120_SDRAM0_CAS_LATENCY must be defined first!!
#endif

#ifndef HWPF_5120_SDRAM0_RAS_LATENCY
#error HWPF_5120_SDRAM0_RAS_LATENCY must be defined first!!
#endif


#if (HWPF_5120_SDRAM0_CAS_LATENCY == 2)
#define SDRAM0_CAS_LATENCY							SDRAM_CAS_LATENCY_2
#define MPMC_SDRAM0_CAS_LATENCY						DM_CAS_LATENCY_2
#elif (HWPF_5120_SDRAM0_CAS_LATENCY == 3)
#define SDRAM0_CAS_LATENCY							SDRAM_CAS_LATENCY_3
#define MPMC_SDRAM0_CAS_LATENCY						DM_CAS_LATENCY_3
#else
#error HWPF_5120_SDRAM0_CAS_LATENCY must be either 2 or 3!!
#endif

#if (HWPF_5120_SDRAM0_RAS_LATENCY == 1)
#define MPMC_SDRAM0_RAS_LATENCY						DM_RAS_LATENCY_1
#elif (HWPF_5120_SDRAM0_RAS_LATENCY == 2)
#define MPMC_SDRAM0_RAS_LATENCY						DM_RAS_LATENCY_2
#elif (HWPF_5120_SDRAM0_RAS_LATENCY == 3)
#define MPMC_SDRAM0_RAS_LATENCY						DM_RAS_LATENCY_3
#else
#error HWPF_5120_SDRAM0_RAS_LATENCY must be either 1, 2, or 3!!
#endif  // (HWPF_5120_SDRAM0_RAS_LATENCY == 1)

#define SDRAM_BANK0_RAS_CAS							(MPMC_SDRAM0_CAS_LATENCY 	\
													| MPMC_SDRAM0_RAS_LATENCY)


#if (HWPF_5120_SDRAM0_BUS_WIDTH != 32) && (HWPF_5120_SDRAM0_BUS_WIDTH != 16)
#error HWPF_5120_SDRAM0_BUS_WIDTH must be either 16 or 32
#endif

#if (HWPF_5120_SDRAM0_DEV_DATA_WIDTH != 8) && (HWPF_5120_SDRAM0_DEV_DATA_WIDTH != 16) && (HWPF_5120_SDRAM0_DEV_DATA_WIDTH != 32)
#error HWPF_5120_SDRAM0_DEV_DATA_WIDTH must be one of 8, 16, or 32.
#endif


#if HWPF_5120_SDRAM0_BUS_WIDTH == 16
// 16Bits Bus
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 32
#error HWPF_5120_SDRAM0_DEV_DATA_WIDTH is larger than HWPF_5120_SDRAM0_DEV_DATA_WIDTH!!
#endif

#define SDRAM0_BURST_LEN						SDRAM_BTLEN_8
#define MPMC_SDRAM0_AM14						0


#if HWPF_5120_SDRAM0_DEV_SIZE == 16
#define MPMC_SDRAM0_AM11_9					0
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				4
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK0_SIZE				2
#define SDRAM_BANK0_MODE_SHIFT			10
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 64
#define MPMC_SDRAM0_AM11_9					(0x1 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				16
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK0_SIZE				8
#define SDRAM_BANK0_MODE_SHIFT			11
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 128
#define MPMC_SDRAM0_AM11_9					(0x2 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				32
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#else
#define SDRAM_BANK0_SIZE				16
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 256
#define MPMC_SDRAM0_AM11_9					(0x3 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				64
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#else
#define SDRAM_BANK0_SIZE				32
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 512
#define MPMC_SDRAM0_AM11_9					(0x4 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				128
#define SDRAM_BANK0_MODE_SHIFT			14
#define MPMC_SDRAM0_AM8_7				(0x0 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_11BIT
#else
#define SDRAM_BANK0_SIZE				64
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#endif
#else
#error HWPF_5120_SDRAM0_DEV_SIZE must be one of 16, 64, 128, 256, or 512!!
#endif


#else
// 32 bits Bus
#define SDRAM0_BURST_LEN						SDRAM_BTLEN_4
#define MPMC_SDRAM0_AM14						(0x1<<14)

#if HWPF_5120_SDRAM0_DEV_SIZE == 16
#define MPMC_SDRAM0_AM11_9					0
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				8
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#elif HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 16
#define SDRAM_BANK0_SIZE				4
#define SDRAM_BANK0_MODE_SHIFT			11
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#else
#error 512Kx32 SDRAM is not supported.
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 64
#define MPMC_SDRAM0_AM11_9					(0x1 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				32
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#elif HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 16
#define SDRAM_BANK0_SIZE				16
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#else
#define SDRAM_BANK0_SIZE				8
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x2 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 128
#define MPMC_SDRAM0_AM11_9					(0x2 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				64
#define SDRAM_BANK0_MODE_SHIFT			14
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#elif HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 16
#define SDRAM_BANK0_SIZE				32
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK0_SIZE				16
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x2 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 256
#define MPMC_SDRAM0_AM11_9					(0x3 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				128
#define SDRAM_BANK0_MODE_SHIFT			14
#define MPMC_SDRAM0_AM8_7				0
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#elif HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 16
#define SDRAM_BANK0_SIZE				64
#define SDRAM_BANK0_MODE_SHIFT			13
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK0_SIZE				32
#define SDRAM_BANK0_MODE_SHIFT			12
#define MPMC_SDRAM0_AM8_7				(0x2 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM0_DEV_SIZE == 512
#define MPMC_SDRAM0_AM11_9					(0x4 << 9)
#if HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 8
#define SDRAM_BANK0_SIZE				256
#define SDRAM_BANK0_MODE_SHIFT			15
#define MPMC_SDRAM0_AM8_7				(0x0 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_11BIT
#elif HWPF_5120_SDRAM0_DEV_DATA_WIDTH == 16
#define SDRAM_BANK0_SIZE				128
#define SDRAM_BANK0_MODE_SHIFT			14
#define MPMC_SDRAM0_AM8_7				(0x1 << 7)
#define MPMC_SDRAM0_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM0_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM0_CW					DM_CFG_COL_WIDTH_10BIT
#else
#error 16Mx32 SDRAM is not supported.
#endif
#else
#error HWPF_5120_SDRAM0_DEV_SIZE must be one of 16, 64, 128, 256, or 512!!
#endif
#endif


#define SDRAM_BANK0_MODE 							(SDRAM0_BURST_LEN		\
													| SDRAM0_CAS_LATENCY	\
													| SDRAM_OPMODE_STANDARD \
													| SDRAM_WBTMODE_ENABLE)



#define MPMC_SDRAM_BANK0_CFG						(MPMC_SDRAM0_NB 		\
													| MPMC_SDRAM0_RW 		\
													| MPMC_SDRAM0_CW		\
													| MPMC_SDRAM0_AM14		\
													| MPMC_SDRAM0_AM11_9	\
													| MPMC_SDRAM0_AM8_7)

#if SDRAM_BANK0_SIZE == 4
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_4MBYTES
#elif SDRAM_BANK0_SIZE == 8
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_8MBYTES
#elif SDRAM_BANK0_SIZE == 16
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_16MBYTES
#elif SDRAM_BANK0_SIZE == 32
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_16MBYTES
#elif SDRAM_BANK0_SIZE == 64
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_64MBYTES
#elif SDRAM_BANK0_SIZE == 128
#define HWPF_5120_SDRAMSIZE_CFG				SDRAM_SIZE_128MBYTES
#else
#error SDRAM bank0 configuration error!!
#endif


#define HWPF_5120_SDRAM0_BASE						ADM5120_SDRAM_BASE
#define HWPF_5120_SDRAM0_SIZE						(SDRAM_BANK0_SIZE << 20)

#if (HWPF_5120_SDRAM1_EN == OPTION_ENABLE)
#define HWPF_5120_SDRAM_BANK_NUM					2
#define HWPF_5120_SDRAM_CNTL						MEMCNTL_SDRAM1_EN
#else
#define HWPF_5120_SDRAM_CNTL						0
#define HWPF_5120_SDRAM_BANK_NUM					1
#define HWPF_5120_SDRAM_MEM_TOP_LIMIT				(HWPF_5120_SDRAM0_BASE + HWPF_5120_SDRAM0_SIZE)
#endif

// ++++++++++++++++++++++   SDRAM bank0 end   ++++++++++++++++++++++
//===========================================================================


//===========================================================================
// ++++++++++++++++++++++   SDRAM bank1 start   ++++++++++++++++++++++
#if (HWPF_5120_SDRAM_BANK_NUM == 2)

#ifndef HWPF_5120_SDRAM1_CAS_LATENCY
#error HWPF_5120_SDRAM1_CAS_LATENCY must be defined first!!
#endif  // HWPF_5120_SDRAM1_CAS_LATENCY

#ifndef HWPF_5120_SDRAM1_RAS_LATENCY
#error HWPF_5120_SDRAM1_RAS_LATENCY must be defined first!!
#endif	// HWPF_5120_SDRAM1_RAS_LATENCY


#if (HWPF_5120_SDRAM1_CAS_LATENCY == 2)
#define SDRAM1_CAS_LATENCY							SDRAM_CAS_LATENCY_2
#define MPMC_SDRAM1_CAS_LATENCY						DM_CAS_LATENCY_2
#elif (HWPF_5120_SDRAM1_CAS_LATENCY == 3)
#define SDRAM1_CAS_LATENCY							SDRAM_CAS_LATENCY_3
#define MPMC_SDRAM1_CAS_LATENCY						DM_CAS_LATENCY_3
#else
#error HWPF_5120_SDRAM1_CAS_LATENCY must be either 2 or 3!!
#endif

#if (HWPF_5120_SDRAM1_RAS_LATENCY == 1)
#define MPMC_SDRAM1_RAS_LATENCY						DM_RAS_LATENCY_1
#elif (HWPF_5120_SDRAM1_RAS_LATENCY == 2)
#define MPMC_SDRAM1_RAS_LATENCY						DM_RAS_LATENCY_2
#elif (HWPF_5120_SDRAM1_RAS_LATENCY == 3)
#define MPMC_SDRAM1_RAS_LATENCY						DM_RAS_LATENCY_3
#else
#error HWPF_5120_SDRAM1_RAS_LATENCY must be either 1, 2, or 3!!
#endif  // (HWPF_5120_SDRAM1_RAS_LATENCY == 1)

#define SDRAM_BANK1_RAS_CAS							(MPMC_SDRAM1_CAS_LATENCY \
													| MPMC_SDRAM1_RAS_LATENCY)



#if (HWPF_5120_SDRAM1_BUS_WIDTH != 32) && (HWPF_5120_SDRAM1_BUS_WIDTH != 16)
#error HWPF_5120_SDRAM1_BUS_WIDTH must be either 16 or 32
#endif

#if (HWPF_5120_SDRAM1_DEV_DATA_WIDTH != 8) && (HWPF_5120_SDRAM1_DEV_DATA_WIDTH != 16) && (HWPF_5120_SDRAM1_DEV_DATA_WIDTH != 32)
#error HWPF_5120_SDRAM1_DEV_DATA_WIDTH must be one of 8, 16, or 32.
#endif


#if HWPF_5120_SDRAM1_BUS_WIDTH == 16
// 16Bits Bus
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 32
#error HWPF_5120_SDRAM1_DEV_DATA_WIDTH is larger than HWPF_5120_SDRAM1_DEV_DATA_WIDTH!!
#endif

#define SDRAM1_BURST_LEN						SDRAM_BTLEN_8
#define MPMC_SDRAM1_AM14						0


#if HWPF_5120_SDRAM1_DEV_SIZE == 16
#define MPMC_SDRAM1_AM11_9					0
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				4
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK1_SIZE				2
#define SDRAM_BANK1_MODE_SHIFT			10
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 64
#define MPMC_SDRAM1_AM11_9					(0x1 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				16
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK1_SIZE				8
#define SDRAM_BANK1_MODE_SHIFT			11
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 128
#define MPMC_SDRAM1_AM11_9					(0x2 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				32
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#else
#define SDRAM_BANK1_SIZE				16
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 256
#define MPMC_SDRAM1_AM11_9					(0x3 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				64
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#else
#define SDRAM_BANK1_SIZE				32
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 512
#define MPMC_SDRAM1_AM11_9					(0x4 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				128
#define SDRAM_BANK1_MODE_SHIFT			14
#define MPMC_SDRAM1_AM8_7				(0x0 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_11BIT
#else
#define SDRAM_BANK1_SIZE				64
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#endif
#else
#error HWPF_5120_SDRAM1_DEV_SIZE must be one of 16, 64, 128, 256, or 512!!
#endif


#else
// 32 bits Bus
#define SDRAM1_BURST_LEN						SDRAM_BTLEN_4
#define MPMC_SDRAM1_AM14						(0x1<<14)

#if HWPF_5120_SDRAM1_DEV_SIZE == 16
#define MPMC_SDRAM1_AM11_9					0
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				8
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#elif HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 16
#define SDRAM_BANK1_SIZE				4
#define SDRAM_BANK1_MODE_SHIFT			11
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_2BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#else
#error 512Kx32 SDRAM is not supported.
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 64
#define MPMC_SDRAM1_AM11_9					(0x1 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				32
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#elif HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 16
#define SDRAM_BANK1_SIZE				16
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#else
#define SDRAM_BANK1_SIZE				8
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x2 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_11BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 128
#define MPMC_SDRAM1_AM11_9					(0x2 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				64
#define SDRAM_BANK1_MODE_SHIFT			14
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#elif HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 16
#define SDRAM_BANK1_SIZE				32
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK1_SIZE				16
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x2 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_12BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 256
#define MPMC_SDRAM1_AM11_9					(0x3 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				128
#define SDRAM_BANK1_MODE_SHIFT			14
#define MPMC_SDRAM1_AM8_7				0
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#elif HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 16
#define SDRAM_BANK1_SIZE				64
#define SDRAM_BANK1_MODE_SHIFT			13
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_9BIT
#else
#define SDRAM_BANK1_SIZE				32
#define SDRAM_BANK1_MODE_SHIFT			12
#define MPMC_SDRAM1_AM8_7				(0x2 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_8BIT
#endif
#elif HWPF_5120_SDRAM1_DEV_SIZE == 512
#define MPMC_SDRAM1_AM11_9					(0x4 << 9)
#if HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 8
#define SDRAM_BANK1_SIZE				256
#define SDRAM_BANK1_MODE_SHIFT			15
#define MPMC_SDRAM1_AM8_7				(0x0 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_11BIT
#elif HWPF_5120_SDRAM1_DEV_DATA_WIDTH == 16
#define SDRAM_BANK1_SIZE				128
#define SDRAM_BANK1_MODE_SHIFT			14
#define MPMC_SDRAM1_AM8_7				(0x1 << 7)
#define MPMC_SDRAM1_NB					DM_CFG_4BANK_DEV
#define MPMC_SDRAM1_RW					DM_CFG_ROW_WIDTH_13BIT
#define MPMC_SDRAM1_CW					DM_CFG_COL_WIDTH_10BIT
#else
#error 16Mx32 SDRAM is not supported.
#endif
#else
#error HWPF_5120_SDRAM1_DEV_SIZE must be one of 16, 64, 128, 256, or 512!!
#endif
#endif


#define SDRAM_BANK1_MODE 							(SDRAM1_BURST_LEN		\
													| SDRAM1_CAS_LATENCY	\
													| SDRAM_OPMODE_STANDARD \
													| SDRAM_WBTMODE_ENABLE)



#define MPMC_SDRAM_BANK1_CFG						(MPMC_SDRAM1_NB 		\
													| MPMC_SDRAM1_RW 		\
													| MPMC_SDRAM1_CW		\
													| MPMC_SDRAM1_AM14		\
													| MPMC_SDRAM1_AM11_9	\
													| MPMC_SDRAM1_AM8_7)

#if SDRAM_BANK1_SIZE == 4
#define MEM_SDRAM1_SIZE							SDRAM_SIZE_4MBYTES
#elif SDRAM_BANK1_SIZE == 8
#define MEM_SDRAM1_SIZE							SDRAM_SIZE_8MBYTES
#elif SDRAM_BANK1_SIZE == 16
#define MEM_SDRAM1_SIZE							SDRAM_SIZE_16MBYTES
#elif SDRAM_BANK1_SIZE == 64
#define MEM_SDRAM1_SIZE							SDRAM_SIZE_64MBYTES
#elif SDRAM_BANK1_SIZE == 128
#define MEM_SDRAM1_SIZE							SDRAM_SIZE_128MBYTES
#else
#error SDRAM bank1 configuration error!!
#endif

#if SDRAM_BANK1_SIZE != SDRAM_BANK0_SIZE
#error SDRAM_BANK1_SIZE must be equal to SDRAM_BANK0_SIZE!!!
#endif

#define HWPF_5120_SDRAM1_BASE						(HWPF_5120_SDRAM0_BASE + HWPF_5120_SDRAM0_SIZE)
#define HWPF_5120_SDRAM1_SIZE						(SDRAM_BANK1_SIZE << 20)

#define HWPF_5120_SDRAM_MEM_TOP_LIMIT				(HWPF_5120_SDRAM1_BASE + HWPF_5120_SDRAM1_SIZE)
#endif	// (HWPF_5120_SDRAM_BANK_NUM == 2)

#define HWPF_5120_DRAM_END							HWPF_5120_SDRAM_MEM_TOP_LIMIT

// ++++++++++++++++++++++   SDRAM bank1 end   ++++++++++++++++++++++

// ++++++++++++++++++++   SDRAM Timing start   +++++++++++++++++++++
#if (HWPF_5120_SDRAM_tRP < 0) || (HWPF_5120_SDRAM_tRP > 15)
#error HWPF_5120_SDRAM_tRP is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tRAS < 0) || (HWPF_5120_SDRAM_tRAS > 15)
#error HWPF_5120_SDRAM_tRAS is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tSREX < 0) || (HWPF_5120_SDRAM_tSREX > 15)
#error HWPF_5120_SDRAM_tSREX is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tAPR < 0) || (HWPF_5120_SDRAM_tAPR > 15)
#error HWPF_5120_SDRAM_tAPR is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tDAL < 0) || (HWPF_5120_SDRAM_tDAL > 15)
#error HWPF_5120_SDRAM_tDAL is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tWR < 0) || (HWPF_5120_SDRAM_tWR > 15)
#error HWPF_5120_SDRAM_tWR is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tRC < 0) || (HWPF_5120_SDRAM_tRC > 31)
#error HWPF_5120_SDRAM_tRC is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tRFC < 0) || (HWPF_5120_SDRAM_tRFC > 31)
#error HWPF_5120_SDRAM_tRFC is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tXSR < 0) || (HWPF_5120_SDRAM_tXSR > 31)
#error HWPF_5120_SDRAM_tXSR is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tRRD < 0) || (HWPF_5120_SDRAM_tRRD > 15)
#error HWPF_5120_SDRAM_tRRD is out of range!!!
#endif

#if (HWPF_5120_SDRAM_tMRD < 0) || (HWPF_5120_SDRAM_tMRD > 15)
#error HWPF_5120_SDRAM_tMRD is out of range!!!
#endif

// +++++++++++++++++++++   SDRAM Timing end   ++++++++++++++++++++++
//===========================================================================


//===========================================================================
// ++++++++++++++++++++   Ext I/O bank0 start   ++++++++++++++++++++
// Ext I/O bank 0 configuration
#if (HWPF_5120_EXTIO0_EN == OPTION_ENABLE)

#if (HWPF_5120_EXTIO0_INT_POLARITY == 0)
#define HWPF_5120_EXTIO0_INT_CFG					IRQ_EXTIO0_ACT_LOW
#elif (HWPF_5120_EXTIO0_INT_POLARITY == 1)
#define HWPF_5120_EXTIO0_INT_CFG					0
#else
#error The value of HWPF_5120_EXTIO0_INT_POLARITY must be either 0 or 1!!!
#endif

#if (HWPF_5120_EXTIO0_WAIT_EN != OPTION_ENABLE) && (HWPF_5120_EXTIO0_WAIT_EN != OPTION_DISABLE)
#error The value of HWPF_5120_EXTIO0_WAIT_EN must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif

#if (HWPF_5120_EXTIO0_BUS_WIDTH == 32)
#define SMEM2_BUS_WIDTH								SM_MEM_WIDTH_32BIT
#elif (HWPF_5120_EXTIO0_BUS_WIDTH == 16)
#define SMEM2_BUS_WIDTH								SM_MEM_WIDTH_16BIT
#elif (HWPF_5120_EXTIO0_BUS_WIDTH == 8)
#define SMEM2_BUS_WIDTH								SM_MEM_WIDTH_8BIT
#else
#error The value of HWPF_5120_EXTIO0_BUS_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_EXTIO0_BUS_WIDTH == 32)

#if (HWPF_5120_EXTIO0_DEV_WIDTH == 32)
#define SMEM2_PB									SM_PB
#elif (HWPF_5120_EXTIO0_DEV_WIDTH == 16)
#define SMEM2_PB									SM_PB
#elif (HWPF_5120_EXTIO0_DEV_WIDTH == 8)
//#define SMEM2_PB									0
#define SMEM2_PB									SM_PB
#else
#error The value of HWPF_5120_EXTIO0_DEV_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_EXTIO0_DEV_WIDTH == 32)

#if (HWPF_5120_EXTIO0_BUS_WIDTH < HWPF_5120_EXTIO0_DEV_WIDTH)
#warning The data bus width of Ext I/O bank 0 is smaller than the width of the device !!!
#endif

#if HWPF_5120_EXTIO0_CS_POLARITY == 1
#define SMEM2_CS_POLARITY							SM_CS_HIGH
#elif HWPF_5120_EXTIO0_CS_POLARITY == 0
#define SMEM2_CS_POLARITY							0
#else
#error The value of HWPF_5120_EXTIO0_CS_POLARITY must be either 1 or 0!!!
#endif  // (HWPF_5120_EXTIO0_CS_POLARITY == 1)

#if HWPF_5120_EXTIO0_PAGE_MODE == OPTION_ENABLE
#define SMEM2_PM_MODE								SM_PAGE_MODE
#elif HWPF_5120_EXTIO0_PAGE_MODE == OPTION_DISABLE
#define SMEM2_PM_MODE								0
#else
#error The value of HWPF_5120_EXTIO0_PAGE_MODE must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif  // (HWPF_5120_EXTIO0_PAGE_MODE == OPTION_ENABLE)

#if (HWPF_5120_EXTIO0_CS_TO_WE_WAIT < 0) || (HWPF_5120_EXTIO0_CS_TO_WE_WAIT > 15)
#error HWPF_5120_EXTIO0_CS_TO_WE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO0_CS_TO_OE_WAIT < 0) || (HWPF_5120_EXTIO0_CS_TO_OE_WAIT > 15)
#error HWPF_5120_EXTIO0_CS_TO_OE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO0_READ_WAIT < 0) || (HWPF_5120_EXTIO0_READ_WAIT > 31)
#error HWPF_5120_EXTIO0_READ_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO0_WRITE_WAIT < 0) || (HWPF_5120_EXTIO0_WRITE_WAIT >31)
#error HWPF_5120_EXTIO0_WRITE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO0_TURN_WAIT < 0) || (HWPF_5120_EXTIO0_TURN_WAIT > 15)
#error HWPF_5120_EXTIO0_TURN_WAIT is out of range!!!
#endif


#define SMEM2_CFG									( SMEM2_BUS_WIDTH 		\
													| SMEM2_CS_POLARITY 	\
													| SMEM2_PM_MODE 		\
													| SMEM2_PB)


#define SmemBank2_Config							SMEM2_CFG
#define	SmemBank2_WaitWen							HWPF_5120_EXTIO0_CS_TO_WE_WAIT
#define	SmemBank2_WaitOen							HWPF_5120_EXTIO0_CS_TO_OE_WAIT
#define	SmemBank2_WaitRd							HWPF_5120_EXTIO0_READ_WAIT
#define	SmemBank2_WaitPage							31
#define	SmemBank2_WaitWr							HWPF_5120_EXTIO0_WRITE_WAIT
#define	SmemBank2_WaitTurn							HWPF_5120_EXTIO0_TURN_WAIT

#endif	// HWPF_5120_EXTIO0_EN == OPTION_ENABLE

// +++++++++++++++++++++   Ext I/O bank0 end   +++++++++++++++++++++

// ++++++++++++++++++++   Ext I/O bank1 start   ++++++++++++++++++++
// Ext I/O bank 1 configuration
#if (HWPF_5120_EXTIO1_EN == OPTION_ENABLE)

#if (HWPF_5120_EXTIO1_INT_POLARITY == 0)
#define HWPF_5120_EXTIO1_INT_CFG					IRQ_EXTIO1_ACT_LOW
#elif (HWPF_5120_EXTIO1_INT_POLARITY == 1)
#define HWPF_5120_EXTIO1_INT_CFG					0
#else
#error The value of HWPF_5120_EXTIO1_INT_POLARITY must be either 0 or 1!!!
#endif

#if (HWPF_5120_EXTIO1_WAIT_EN != OPTION_ENABLE) && (HWPF_5120_EXTIO1_WAIT_EN != OPTION_DISABLE)
#error The value of HWPF_5120_EXTIO1_WAIT_EN must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif

#if (HWPF_5120_EXTIO1_BUS_WIDTH == 32)
#define SMEM3_BUS_WIDTH								SM_MEM_WIDTH_32BIT
#elif (HWPF_5120_EXTIO1_BUS_WIDTH == 16)
#define SMEM3_BUS_WIDTH								SM_MEM_WIDTH_16BIT
#elif (HWPF_5120_EXTIO1_BUS_WIDTH == 8)
#define SMEM3_BUS_WIDTH								SM_MEM_WIDTH_8BIT
#else
#error The value of HWPF_5120_EXTIO1_BUS_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_SMEM1_BUS_WIDTH == 32)

#if (HWPF_5120_EXTIO1_DEV_WIDTH == 32)
#define SMEM3_PB									SM_PB
#elif (HWPF_5120_EXTIO1_DEV_WIDTH == 16)
#define SMEM3_PB									SM_PB
#elif (HWPF_5120_EXTIO1_DEV_WIDTH == 8)
//#define SMEM3_PB									0
#define SMEM3_PB									SM_PB
#else
#error The value of HWPF_5120_EXTIO1_DEV_WIDTH must be either 8, 16, or 32!!!!
#endif // (HWPF_5120_EXTIO1_DEV_WIDTH == 32)

#if (HWPF_5120_EXTIO1_BUS_WIDTH < HWPF_5120_EXTIO1_DEV_WIDTH)
#warning The data bus width of Ext I/O bank 1 is smaller than the width of the device !!!
#endif

#if HWPF_5120_EXTIO1_CS_POLARITY == 1
#define SMEM3_CS_POLARITY							SM_CS_HIGH
#elif HWPF_5120_EXTIO1_CS_POLARITY == 0
#define SMEM3_CS_POLARITY							0
#else
#error The value of HWPF_5120_EXTIO1_CS_POLARITY must be either 1 or 0!!!
#endif  // (HWPF_5120_EXTIO0_CS_POLARITY == 1)

#if HWPF_5120_EXTIO1_PAGE_MODE == OPTION_ENABLE
#define SMEM3_PM_MODE								SM_PAGE_MODE
#elif HWPF_5120_EXTIO1_PAGE_MODE == OPTION_DISABLE
#define SMEM3_PM_MODE								0
#else
#error The value of HWPF_5120_EXTIO1_PAGE_MODE must be either OPTION_ENABLE or OPTION_DISABLE!!!
#endif  // (HWPF_5120_EXTIO0_PAGE_MODE == OPTION_ENABLE)

#if (HWPF_5120_EXTIO1_CS_TO_WE_WAIT < 0) || (HWPF_5120_EXTIO1_CS_TO_WE_WAIT > 15)
#error HWPF_5120_EXTIO1_CS_TO_WE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO1_CS_TO_OE_WAIT < 0) || (HWPF_5120_EXTIO1_CS_TO_OE_WAIT > 15)
#error HWPF_5120_EXTIO1_CS_TO_OE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO1_READ_WAIT < 0) || (HWPF_5120_EXTIO1_READ_WAIT > 31)
#error HWPF_5120_EXTIO1_READ_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO1_WRITE_WAIT < 0) || (HWPF_5120_EXTIO1_WRITE_WAIT >31)
#error HWPF_5120_EXTIO1_WRITE_WAIT is out of range!!!
#endif

#if (HWPF_5120_EXTIO1_TURN_WAIT < 0) || (HWPF_5120_EXTIO1_TURN_WAIT > 15)
#error HWPF_5120_EXTIO1_TURN_WAIT is out of range!!!
#endif


#define SMEM3_CFG									( SMEM3_BUS_WIDTH 		\
													| SMEM3_CS_POLARITY 	\
													| SMEM3_PM_MODE 		\
													| SMEM3_PB)


#define SmemBank3_Config							SMEM3_CFG
#define	SmemBank3_WaitWen							HWPF_5120_EXTIO1_CS_TO_WE_WAIT
#define	SmemBank3_WaitOen							HWPF_5120_EXTIO1_CS_TO_OE_WAIT
#define	SmemBank3_WaitRd							HWPF_5120_EXTIO1_READ_WAIT
#define	SmemBank3_WaitPage							31
#define	SmemBank3_WaitWr							HWPF_5120_EXTIO1_WRITE_WAIT
#define	SmemBank3_WaitTurn							HWPF_5120_EXTIO1_TURN_WAIT

#endif	// !(HWPF_5120_EXTIO1_EN == OPTION_ENABLE)

// +++++++++++++++++++++   Ext I/O bank0 end   +++++++++++++++++++++
//===========================================================================


//===========================================================================
// ++++++++++++++++++++   GPIO start   ++++++++++++++++++++
// GPIO configuration

#if (HWPF_5120_GPIO0_MODE == GPIO0_INPUT_MODE)
#undef HWPF_5120_GPIO0_DEFOUT
#define HWPF_5120_GPIO0_DEFOUT					GPIO0_OUTPUT_LOW
#elif (HWPF_5120_GPIO0_MODE == GPIO0_OUTPUT_MODE)
#if ((HWPF_5120_GPIO0_DEFOUT != GPIO0_OUTPUT_LOW) && (HWPF_5120_GPIO0_DEFOUT != GPIO0_OUTPUT_HI))
#error HWPF_5120_GPIO0_DEFOUT must be either GPIO0_OUTPUT_LOW or GPIO0_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO0_MODE must be either GPIO0_INPUT_MODE or GPIO0_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO0_MODE != GPIO0_INPUT_MODE)

#if (HWPF_5120_GPIO1_MODE == GPIO1_INPUT_MODE)
#undef HWPF_5120_GPIO1_DEFOUT
#define HWPF_5120_GPIO1_DEFOUT					GPIO1_OUTPUT_LOW
#elif (HWPF_5120_GPIO1_MODE == GPIO1_OUTPUT_MODE)
#if ((HWPF_5120_GPIO1_DEFOUT != GPIO1_OUTPUT_LOW) && (HWPF_5120_GPIO1_DEFOUT != GPIO1_OUTPUT_HI))
#error HWPF_5120_GPIO1_DEFOUT must be either GPIO1_OUTPUT_LOW or GPIO1_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO1_MODE must be either GPIO1_INPUT_MODE or GPIO1_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO1_MODE != GPIO1_INPUT_MODE)


#if (HWPF_5120_GPIO2_MODE == GPIO2_INPUT_MODE)
#undef HWPF_5120_GPIO2_DEFOUT
#define HWPF_5120_GPIO2_DEFOUT					GPIO2_OUTPUT_LOW
#elif (HWPF_5120_GPIO2_MODE == GPIO2_OUTPUT_MODE)
#if ((HWPF_5120_GPIO2_DEFOUT != GPIO2_OUTPUT_LOW) && (HWPF_5120_GPIO2_DEFOUT != GPIO2_OUTPUT_HI))
#error HWPF_5120_GPIO2_DEFOUT must be either GPIO2_OUTPUT_LOW or GPIO2_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO2_MODE must be either GPIO2_INPUT_MODE or GPIO2_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO2_MODE != GPIO2_INPUT_MODE)


#if (HWPF_5120_GPIO3_MODE == GPIO3_INPUT_MODE)
#undef HWPF_5120_GPIO3_DEFOUT
#define HWPF_5120_GPIO3_DEFOUT					GPIO3_OUTPUT_LOW
#elif (HWPF_5120_GPIO3_MODE == GPIO3_OUTPUT_MODE)
#if ((HWPF_5120_GPIO3_DEFOUT != GPIO3_OUTPUT_LOW) && (HWPF_5120_GPIO3_DEFOUT != GPIO3_OUTPUT_HI))
#error HWPF_5120_GPIO3_DEFOUT must be either GPIO3_OUTPUT_LOW or GPIO3_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO3_MODE must be either GPIO3_INPUT_MODE or GPIO3_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO3_MODE != GPIO3_INPUT_MODE)


#if (HWPF_5120_GPIO4_MODE == GPIO4_INPUT_MODE)
#undef HWPF_5120_GPIO4_DEFOUT
#define HWPF_5120_GPIO4_DEFOUT					GPIO4_OUTPUT_LOW
#elif (HWPF_5120_GPIO4_MODE == GPIO4_OUTPUT_MODE)
#if ((HWPF_5120_GPIO4_DEFOUT != GPIO4_OUTPUT_LOW) && (HWPF_5120_GPIO4_DEFOUT != GPIO4_OUTPUT_HI))
#error HWPF_5120_GPIO4_DEFOUT must be either GPIO4_OUTPUT_LOW or GPIO4_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO4_MODE must be either GPIO4_INPUT_MODE or GPIO4_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO4_MODE != GPIO4_INPUT_MODE)


#if (HWPF_5120_GPIO5_MODE == GPIO5_INPUT_MODE)
#undef HWPF_5120_GPIO5_DEFOUT
#define HWPF_5120_GPIO5_DEFOUT					GPIO5_OUTPUT_LOW
#elif (HWPF_5120_GPIO5_MODE == GPIO5_OUTPUT_MODE)
#if ((HWPF_5120_GPIO5_DEFOUT != GPIO5_OUTPUT_LOW) && (HWPF_5120_GPIO5_DEFOUT != GPIO5_OUTPUT_HI))
#error HWPF_5120_GPIO5_DEFOUT must be either GPIO5_OUTPUT_LOW or GPIO5_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO5_MODE must be either GPIO5_INPUT_MODE or GPIO5_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO5_MODE != GPIO5_INPUT_MODE)


#if (HWPF_5120_GPIO6_MODE == GPIO6_INPUT_MODE)
#undef HWPF_5120_GPIO6_DEFOUT
#define HWPF_5120_GPIO6_DEFOUT					GPIO6_OUTPUT_LOW
#elif (HWPF_5120_GPIO6_MODE == GPIO6_OUTPUT_MODE)
#if ((HWPF_5120_GPIO6_DEFOUT != GPIO6_OUTPUT_LOW) && (HWPF_5120_GPIO6_DEFOUT != GPIO6_OUTPUT_HI))
#error HWPF_5120_GPIO6_DEFOUT must be either GPIO6_OUTPUT_LOW or GPIO6_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO6_MODE must be either GPIO6_INPUT_MODE or GPIO6_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO6_MODE != GPIO6_INPUT_MODE)


#if (HWPF_5120_GPIO7_MODE == GPIO7_INPUT_MODE)
#undef HWPF_5120_GPIO7_DEFOUT
#define HWPF_5120_GPIO7_DEFOUT					GPIO7_OUTPUT_LOW
#elif (HWPF_5120_GPIO7_MODE == GPIO7_OUTPUT_MODE)
#if ((HWPF_5120_GPIO7_DEFOUT != GPIO7_OUTPUT_LOW) && (HWPF_5120_GPIO7_DEFOUT != GPIO7_OUTPUT_HI))
#error HWPF_5120_GPIO7_DEFOUT must be either GPIO7_OUTPUT_LOW or GPIO7_OUTPUT_HI !!
#endif
#else
#error HWPF_5120_GPIO7_MODE must be either GPIO7_INPUT_MODE or GPIO7_OUTPUT_MODE!!
#endif	// (HWPF_5120_GPIO7_MODE != GPIO7_INPUT_MODE)




#define HWPF_5120_GPIO_CFG0_MODE			( HWPF_5120_GPIO0_MODE		\
											| HWPF_5120_GPIO1_MODE		\
											| HWPF_5120_GPIO2_MODE		\
											| HWPF_5120_GPIO3_MODE		\
											| HWPF_5120_GPIO4_MODE		\
											| HWPF_5120_GPIO5_MODE		\
											| HWPF_5120_GPIO6_MODE		\
											| HWPF_5120_GPIO7_MODE )

#define HWPF_5120_GPIO_CFG0_OUTEN			((~(HWPF_5120_GPIO_CFG0_MODE << 16)) & GPIO_CONF0_OUTEN_MASK)

#define HWPF_5120_GPIO_CFG0_OUT				( HWPF_5120_GPIO0_DEFOUT	\
											| HWPF_5120_GPIO1_DEFOUT	\
											| HWPF_5120_GPIO2_DEFOUT	\
											| HWPF_5120_GPIO3_DEFOUT	\
											| HWPF_5120_GPIO4_DEFOUT	\
											| HWPF_5120_GPIO5_DEFOUT	\
											| HWPF_5120_GPIO6_DEFOUT	\
											| HWPF_5120_GPIO7_DEFOUT )

#define HWPF_5120_GPIO_CFG0					( HWPF_5120_GPIO_CFG0_OUT		\
											| HWPF_5120_GPIO_CFG0_OUTEN		\
											| HWPF_5120_GPIO_CFG0_MODE )


// +++++++++++++++++++++   GPIO end   +++++++++++++++++++++
//===========================================================================


//===========================================================================
// ++++++++++++++++++++   Port0 LED start   ++++++++++++++++++++
// Port0 LED configuration
#if (HWPF_5120_P0_LED0_MODE < 0) || (HWPF_5120_P0_LED0_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P0_LED0_MODE is invalid!!!
#endif

#if (HWPF_5120_P0_LED1_MODE < 0) || (HWPF_5120_P0_LED1_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P0_LED1_MODE is invalid!!!
#endif

#if (HWPF_5120_P0_LED2_MODE < 0) || (HWPF_5120_P0_LED2_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P0_LED2_MODE is invalid!!!
#endif

#define HPWF_5120_P0_LED_CFG						( (HWPF_5120_P0_LED0_MODE << PORT_LED0_SHIFT) \
													| (HWPF_5120_P0_LED1_MODE << PORT_LED1_SHIFT) \
													| (HWPF_5120_P0_LED2_MODE << PORT_LED2_SHIFT))

// ++++++++++++++++++++   Port0 LED end   ++++++++++++++++++++

// ++++++++++++++++++++   Port1 LED start   ++++++++++++++++++++
// Port1 LED configuration
#if (HWPF_5120_P1_LED0_MODE < 0) || (HWPF_5120_P1_LED0_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P1_LED0_MODE is invalid!!!
#endif

#if (HWPF_5120_P1_LED1_MODE < 0) || (HWPF_5120_P1_LED1_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P0_LED1_MODE is invalid!!!
#endif

#if (HWPF_5120_P1_LED2_MODE < 0) || (HWPF_5120_P1_LED2_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P1_LED2_MODE is invalid!!!
#endif

#define HPWF_5120_P1_LED_CFG						( (HWPF_5120_P1_LED0_MODE << PORT_LED0_SHIFT) \
													| (HWPF_5120_P1_LED1_MODE << PORT_LED1_SHIFT) \
													| (HWPF_5120_P1_LED2_MODE << PORT_LED2_SHIFT))

// ++++++++++++++++++++   Port1 LED end   ++++++++++++++++++++

// ++++++++++++++++++++   Port2 LED start   ++++++++++++++++++++
// Port2 LED configuration
#if (HWPF_5120_P2_LED0_MODE < 0) || (HWPF_5120_P2_LED0_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P2_LED0_MODE is invalid!!!
#endif

#if (HWPF_5120_P2_LED1_MODE < 0) || (HWPF_5120_P2_LED1_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P2_LED1_MODE is invalid!!!
#endif

#if (HWPF_5120_P2_LED2_MODE < 0) || (HWPF_5120_P2_LED2_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P2_LED2_MODE is invalid!!!
#endif

#define HPWF_5120_P2_LED_CFG						( (HWPF_5120_P2_LED0_MODE << PORT_LED0_SHIFT) \
													| (HWPF_5120_P2_LED1_MODE << PORT_LED1_SHIFT) \
													| (HWPF_5120_P2_LED2_MODE << PORT_LED2_SHIFT))

// ++++++++++++++++++++   Port2 LED end   ++++++++++++++++++++

// ++++++++++++++++++++   Port3 LED start   ++++++++++++++++++++
// Port3 LED configuration
#if (HWPF_5120_P3_LED0_MODE < 0) || (HWPF_5120_P3_LED0_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P3_LED0_MODE is invalid!!!
#endif

#if (HWPF_5120_P3_LED1_MODE < 0) || (HWPF_5120_P3_LED1_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P3_LED1_MODE is invalid!!!
#endif

#if (HWPF_5120_P3_LED2_MODE < 0) || (HWPF_5120_P3_LED2_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P3_LED2_MODE is invalid!!!
#endif

#define HPWF_5120_P3_LED_CFG						( (HWPF_5120_P3_LED0_MODE << PORT_LED0_SHIFT) \
													| (HWPF_5120_P3_LED1_MODE << PORT_LED1_SHIFT) \
													| (HWPF_5120_P3_LED2_MODE << PORT_LED2_SHIFT))

// ++++++++++++++++++++   Port3 LED end   ++++++++++++++++++++

// ++++++++++++++++++++   Port4 LED start   ++++++++++++++++++++
// Port4 LED configuration
#if (HWPF_5120_P4_LED0_MODE < 0) || (HWPF_5120_P4_LED0_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P0_LED0_MODE is invalid!!!
#endif

#if (HWPF_5120_P4_LED1_MODE < 0) || (HWPF_5120_P4_LED1_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P4_LED1_MODE is invalid!!!
#endif

#if (HWPF_5120_P4_LED2_MODE < 0) || (HWPF_5120_P4_LED2_MODE > GPIOL_100MLINK_ACT_LED)
#error The value of HWPF_5120_P4_LED2_MODE is invalid!!!
#endif

#define HPWF_5120_P4_LED_CFG						( (HWPF_5120_P4_LED0_MODE << PORT_LED0_SHIFT) \
													| (HWPF_5120_P4_LED1_MODE << PORT_LED1_SHIFT) \
													| (HWPF_5120_P4_LED2_MODE << PORT_LED2_SHIFT))

// ++++++++++++++++++++   Port4 LED end   ++++++++++++++++++++
//===========================================================================



//===========================================================================
// +++++++++++++++++   MISC inference start   +++++++++++++++++
// This should be the last inference section.!!


#define HWPF_5120_MEMCNTL_CFG				(HWPF_5120_SDRAMSIZE_CFG	\
											| HWPF_5120_SDRAM_CNTL		\
											| HWPF_5120_ROMSIZE_CFG)



// ++++++++++++++++++   MISC inference end   ++++++++++++++++++
//===========================================================================

#endif
