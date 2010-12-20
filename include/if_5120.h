/*****************************************************************************
;
;    Project : ADM5120
;    Creator : David Weng
;    File    : if_5120.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/


#ifndef __IF_5120_H__
#define __IF_5120_H__


/*********************  MII Phy Reg Access  *****************/
#define PHY_ADDR_MASK			0x0000000f
#define PHY_REG_MASK			0x00001f00
#define PHY_WRITE_CMD			0x00002000
#define PHY_READ_CMD			0x00004000

#define PHY_REG_SHIFT			8

#define PHY_WRITE_OK			0x00000001
#define PHY_READ_OK				0x00000002
#define PHY_DATA_MASK			0xffff0000
#define PHY_DATA_SHIFT			16


#define	MII_PHY_CTRL_REG		0
#define	MII_PHY_STATUS_REG		1
#define MII_PHY_ID0_REG			2
#define MII_PHY_ID1_REG			3
#define MII_PHY_ANAR_REG		4
#define MII_PHY_ANLPAR_REG		5
#define MII_PHY_LINK_REG		0x10

#define MII_PHY_FORCE_LINK		0x03


/******************  Tx/Rx Desc  ******************/
// Common
#define OWN_BIT				0x80000000
#define END_OF_RING			0x10000000
#define BUF_ADDR_MASK		0x00ffffff
#define BUF2_EN				0x80000000

#define BUF1_LEN_MASK		0x000007ff

#define PKT_LEN_MASK		0x07ff0000
#define PKT_LEN_SHIFT		16

// RxDesc Only
#define SRC_PORT_MASK		0x00007000
#define SRC_PORT_SHIFT		12
#define FORCED_FLAG			0x00000400

#define RX_FRAME_TYPE_MASK	0x00000030
#define RX_FRAME_UC			0
#define RX_FRAME_MC			0x00000010
#define RX_FRAME_BC			0x00000020

#define RX_PKT_IPSUM_ERR	0x00000008
#define RX_PKT_VLAN_TAG		0x00000004

#define RX_PKT_TYPE_MASK	0x00000003
#define RX_PKT_IP			0x00000000
#define RX_PKT_PPPOE		0x00000001

#define RX_PKT_FLAG_MASK	0x0000007F

// TxDesc Only
#define TX_ADD_IPSUM		0x80000000
#define TX_DEST_VLAN_MASK	0x0000003F
#define TX_DEST_PORT_MASK	0x00003F00

typedef struct RX_DESC_S
{
	UINT32 buf1cntl;
	UINT32 buf2cntl;
	UINT32 buflen;
	UINT32 status;
} RX_DESC_T;

#define RX_DESC_ALIGN		16

typedef struct TX_DESC_S
{
	UINT32 buf1cntl;
	UINT32 buf2cntl;
	UINT32 buflen;
	UINT32 pktcntl;
} TX_DESC_T;
#define TX_DESC_ALIGN		16

// TxDesc Only
#define TX_DST_PORT_MASK	0x00007f00
#define TX_VLAN_MASK		0x0000007f


/*****************  VLAN  *******************/
#define MAX_VLAN_GROUP		6
#define VLAN_PORT_MASK		0x7f

// Vlan status
#define VLAN_NOT_DEFINED	0
#define VLAN_DISABLED		1
#define VLAN_ENABLED		2

#define VLAN_NONE			0x00
#define VLAN0_ID			0x01
#define VLAN1_ID			0x02
#define VLAN2_ID			0x04
#define VLAN3_ID			0x08
#define VLAN4_ID			0x10
#define VLAN5_ID			0x20

#define VLAN0				0
#define VLAN1				1
#define VLAN2				2
#define VLAN3				3
#define VLAN4				4
#define VLAN5				5


#define VLAN_ALL			0x6f


/***************  Switch Ports  **************/
#define MAX_PORT_NUMBER		7		// Include CPU port
#define PORT_VLAN_MASK		0x3f

#define CPU_PORT			6

// Port status
#define PORT_DISABLED		0
#define PORT_ENABLED		1

#define PORT_DISABLE_MASK	0x3f
#define PORT_ALL			0x7f
#define PORT_NONE			0x00

/*******  Buffer control  *******/
#define BUF1_OFFSET_MAX		31		// Must be 2^n -1
#define BUF1_OFFSET_RAND	(BUF1_OFFSET_MAX+1)

#define BUF1_LEN_MIN		5
//#define BUF1_LEN_MIN		0
#define BUF1_LEN_RAND_MAX	1023	// Must be 2^n - 1
#define BUF1_LEN_MAX		(BUF1_LEN_MIN + BUF1_LEN_RAND_MAX)
#define BUF1_LEN_RAND		(BUF1_LEN_MAX + 1)
#define BUF1_GUARD_BAND		16
#define BUF1_LEN_FULL		0

#define BUF2_OFFSET_MAX		15		//  Must be 2^n - 1
#define BUF2_OFFSET_RAND	(BUF2_OFFSET_MAX + 1)

#define BUF_GUARD_BAND		(BUF1_GUARD_BAND + ADM5120_CACHE_LINE_SIZE - 1)

/*************************************************************/
#define ETH_CRC_LEN			4			// Ethernet CRC Length

typedef struct _DRV_PACKET_DESC_
{
	UINT8 *Buf;
	UINT16 buf1off;
	UINT16 buf1len;
	UINT16 buf2off;
	UINT16 buf2len;
	UINT16 PktLen;
	UINT16 SrcPort;
	UINT32 flag;
	UINT32 Dst;
	struct _DRV_PACKET_DESC_ *Next;
} DRV_PACKET_DESC, *PDRV_PACKET_DESC;

typedef struct _DRV_RX_DESC_
{
	PDRV_PACKET_DESC Pkt;
} DRV_RX_DESC, *PDRV_RX_DESC;

typedef struct _DRV_TX_DESC_
{
	PDRV_PACKET_DESC Pkt;
} DRV_TX_DESC, *PDRV_TX_DESC;

typedef struct _AM5120_PORT_
{
	UINT32 Status;
	UINT8 MacAddr[8];
	UINT32 VlanId;
	UINT32 VlanMask;
} AM5120_PORT, *PAM5120_PORT;

typedef struct _AM5120_VLAN_
{
	UINT32 Status;
	UINT8 MacAddr[8];
	UINT32 Ports;
} AM5120_VLAN, *PAM5120_VLAN;

typedef struct _PORT_STATIS_
{
	UINT32 RxPktCnt;
	UINT32 RxBCPkt;
	UINT32 RxMCPkt;
	UINT32 RxUCPkt;
	UINT32 RxBytes;
} PORT_STAT, *PPORT_STAT;

typedef struct _AM5120_CONTEXT_
{
	UINT32 IntMask;
	UINT32 IntStatus;
	UINT32 link_status;

	UINT32 CtrlFlag;

	UINT32 DropFlag;

	UINT32 NumRxDescH;
	UINT32 NumRxDescL;
	UINT32 NumTxDescH;
	UINT32 NumTxDescL;

	TX_DESC_T *HwTxDescH;
	TX_DESC_T *HwTxDescL;
	RX_DESC_T *HwRxDescH;
	RX_DESC_T *HwRxDescL;
	PDRV_TX_DESC DrvTxDescH;
	PDRV_TX_DESC DrvTxDescL;
	PDRV_RX_DESC DrvRxDescH;
	PDRV_RX_DESC DrvRxDescL;


	UINT32 RxIdxH;
	UINT32 RxIdxL;
	UINT32 TxIdxH_Head;
	UINT32 TxIdxH_Tail;
	UINT32 TxIdxL_Head;
	UINT32 TxIdxL_Tail;

	UINT32 NumFreePkt;
	PDRV_PACKET_DESC FreePktList;
	PDRV_PACKET_DESC PktPool;
	UINT32 FreePktCnt;
	UINT8 *BufPool;

	PDRV_PACKET_DESC TxH_QueueHead;
	PDRV_PACKET_DESC TxH_QueueTail;

	PDRV_PACKET_DESC TxL_QueueHead;
	PDRV_PACKET_DESC TxL_QueueTail;

	UINT32 TxL_PktCnt;
	UINT32 TxL_FullCnt;

	UINT32 TxH_PktCnt;
	UINT32 TxH_FullCnt;

	UINT32 RxL_PktCnt;

	UINT32 RxL_UCCnt;
	UINT32 RxL_MCCnt;
	UINT32 RxL_BCCnt;

	UINT32 RxH_PktCnt;
	UINT32 RxH_UCCnt;
	UINT32 RxH_MCCnt;
	UINT32 RxH_BCCnt;

	UINT32 IntCnt;

	PORT_STAT portstat[MAX_PORT_NUMBER];

	UINT16 Buf1Off;
	UINT16 Buf1Len;
	UINT16 Buf2Off;

} AM5120CONTEXT, *PAM5120CONTEXT;

/*************************  VLAN TRANSFER FUNCTION  ***********************/
typedef struct _TRANS_ENTRY_
{
	UINT32 sa_id;
	UINT32 dst;
} TRANS_ENTRY, *PTRANS_ENTRY;


#define TX_H_INT			SEND_H_DONE_INT
#define TX_L_INT			SEND_L_DONE_INT
#define RX_H_INT			(RX_H_DONE_INT | RX_H_DESC_FULL_INT)
#define RX_L_INT			(RX_L_DONE_INT | RX_L_DESC_FULL_INT)
#define RX_H_FULL_INT		RX_H_DESC_FULL_INT
#define RX_L_FULL_INT		RX_L_DESC_FULL_INT
#define LINK_INT			PORT_STATUS_CHANGE_INT

#define AM5120_INT_MASK		(TX_H_INT | TX_L_INT | RX_H_INT | RX_L_INT | LINK_INT)

#define CPU_PORT_DEFAULT	(SW_DIS_UN_MASK)


/******  switch ctrl code ******/
#define SW_CTRL_STATIS_RESET		1
#define SW_CTRL_CRCFG_GET			2
#define SW_CTRL_CRCFG_SET			3
#define SW_CTRL_BUFFG_GET			4
#define SW_CTRL_BUFFG_SET			5
#define SW_CTRL_BUF1_ALIGN_GET		6
#define SW_CTRL_BUF1_ALIGN_SET		7
#define SW_CTRL_BUF1_LEN_GET		8
#define SW_CTRL_BUF1_LEN_SET		9
#define SW_CTRL_BUF2_ALIGN_GET		10
#define SW_CTRL_BUF2_ALIGN_SET		11
#define SW_CTRL_PADLOADCK_GET		13
#define SW_CTRL_PADLOADCK_SET		14

// VALUES FOR SW_CTRL_CRCFG_SET
#define PKT_SOFTWARE_CRC		0x01
#define PKT_HARDWARE_CRC		0x00

// VLAUES FOR SW_CTRL_BUFFG_SET
#define SW_TWOBUF_PER_DESC		0x02
#define SW_ONEBUF_PER_DESC		0x00

// SW_CTRL_PADLOADCK_SET
#define PKT_PADLOADCK_ON		0x04
#define PKT_PADLOADCK_OFF		0

// Eric + for package BGA and PQNP
#define PACKAGE_BGA
//#define PACKAGE_PQNP

#endif

