#ifndef __MEMDEV_H__
#define __MEMDEV_H__

#define ENABLE                                  1
#define DISABLE                                 0


// Bus width of SDRAM bank0. legal values are 16 or 32
#define BOARD_SDRAM0_BUS_WIDTH					32

// memory size of SDRAM chip in Mbits
#define BOARD_SDRAM0_DEV_SIZE					512

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define BOARD_SDRAM0_DEV_DATA_WIDTH				16

#define BOARD_5120_SDRAM1_STATUS				DISABLE

// Bus width of SDRAM bank0. legal values are 16 or 32
#define BOARD_SDRAM1_BUS_WIDTH				    16

// memory size of SDRAM chip in Mbits
#define BOARD_SDRAM1_DEV_SIZE				    256

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define BOARD_SDRAM1_DEV_DATA_WIDTH			    16


#endif
