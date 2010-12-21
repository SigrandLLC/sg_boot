/*****************************************************************************
;
;    Project : ADM5120
;    Creator : David Weng
;    File    : if_5120.c
;    Abstract:
;
;*****************************************************************************/
#include <ctype.h>
#include <adm5120.h>
#include <irqlib.h>
#include <mips4kc.h>
#include <cachelib.h>
#include <memlib.h>
#include <if_5120.h>
#include <eth.h>
#include <except.h>
#include <param.h>
#include <buart.h>


/********************  Some Constants *****************/
#define NUM_TX_H_DESC		0	// Number of the Transmitting descriptors of high priority
#define NUM_TX_L_DESC		16	// Number of the Transmitting descriptors of low priority
#define NUM_RX_H_DESC		0	// Number of the Receiving descriptors of high priority
#define NUM_RX_L_DESC		32	// Number of the Receiving descriptors of low priority
#define NUM_FREE_PKT		48	// Number of the Free packets

#define MAX_INT_LOOP_CNT	1


#define PKT_BUFFER_SIZE		1792	// Size of the packet buffer.
// This number should be a multiply of CACHE_LINE_SIZE.

#define PKT_BUFFER_ALIGN	(ADM5120_CACHE_LINE_SIZE - 1)

#define DEF_CTRL_FLAG		(PKT_HARDWARE_CRC | SW_ONEBUF_PER_DESC)

#define DEF_BUF1_OFFSET		0
#define DEF_BUF1_LEN		BUF1_LEN_FULL
#define DEF_BUF2_OFFSET		0


/**********************  Macros  *********************/
#define ENQUEUE_TX_PACKET_AT_TAIL(_h, _t, _Pkt)	{					\
					if((_Pkt) != NULL)	{							\
						if((_t) == NULL)	(_h) = (_t) = (_Pkt);	\
						else {										\
							(_t)->Next = (_Pkt);					\
							(_t) = (_Pkt);							\
						}											\
						(_Pkt)->Next =	NULL;						\
					}												\
				}


#define ENQUEUE_TX_PACKET_AT_FRONT(_h, _t, _Pkt) {					\
					if(_Pkt != NULL)								\
					{												\
						(_Pkt)->Next = (_h);						\
						if((_h) == NULL) 	(_h) = (_t) = (_Pkt);	\
						else {										\
							(_h) = (_Pkt);							\
						}											\
					}												\
				}


#define DEQUEUE_TX_PACKET(_h, _t, _Pkt)		{						\
					(_Pkt) = (_h);									\
					if((_h) != NULL)								\
					{												\
						if((_h) == (_t)) (_h) = (_t) = NULL;		\
						else	(_h) = (_Pkt)->Next;				\
						(_Pkt)->Next = NULL;						\
					}												\
				}


/***********************  Local Funcitons  Prototypes  ***********************/
static int DrvAllocateResource (void);
int ProgramMac (int vlan, UINT8 *Mac);
static int ProcessRxHInt (void);
static int ProcessRxLInt (void);
static void ProcessTxLInt (void);
void SendPacketsL (PDRV_PACKET_DESC Pkt);
inline void SetPktAlign (PDRV_PACKET_DESC Pkt);
static void Am5120Isr (int intLev);

extern void panic (void);
extern UINT8 dslam_present;

#ifdef DBG_OUT_SUPPORT
extern void DbgUartPutStr (const char *);
extern void DbgUartPutChar (char);
#else
#define DbgUartPutStr
#define DbgUartPutChar
#endif

static PAM5120CONTEXT ifp;

static int DrvAllocateResource (void)
{
	unsigned int i, PktIdx, TotalPktRequired;
	UINT8 *Buf;
	UINT32 memsz, membk;
	PDRV_PACKET_DESC Pkt;

	if (ifp == NULL) return -1;

	ifp->NumRxDescH = NUM_RX_H_DESC;
	ifp->NumRxDescL = NUM_RX_L_DESC;
	ifp->NumTxDescH = NUM_TX_H_DESC;
	ifp->NumTxDescL = NUM_TX_L_DESC;
	ifp->NumFreePkt = NUM_FREE_PKT;

	ifp->CtrlFlag = DEF_CTRL_FLAG;
	ifp->Buf1Off = DEF_BUF1_OFFSET;
	ifp->Buf1Len = DEF_BUF1_LEN;
	ifp->Buf2Off = DEF_BUF2_OFFSET;

	//  Allocate HwTxDesc/HwRxDesc
	memsz = sizeof (TX_DESC_T) * (ifp->NumTxDescH + ifp->NumTxDescL) + TX_DESC_ALIGN;
	if ((membk = (UINT32) SharedMemAlloc (memsz, TRUE)) == 0) return -1;

	if (membk & (TX_DESC_ALIGN - 1))
		ifp->HwTxDescH = (TX_DESC_T *) ((membk + TX_DESC_ALIGN - 1) & ~(TX_DESC_ALIGN - 1));
	else
		ifp->HwTxDescH = (TX_DESC_T *) membk;

	ifp->HwTxDescL = &ifp->HwTxDescH[ifp->NumTxDescH];

	memsz = sizeof (RX_DESC_T) * (ifp->NumRxDescH + ifp->NumRxDescL) + RX_DESC_ALIGN;
	if ((membk = (UINT32) SharedMemAlloc (memsz, TRUE)) == 0) return -1;

	if (membk & (RX_DESC_ALIGN - 1))
		ifp->HwRxDescH = (RX_DESC_T *) ((membk + RX_DESC_ALIGN - 1) & ~(RX_DESC_ALIGN - 1));
	else
		ifp->HwRxDescH = (RX_DESC_T *) membk;

	ifp->HwRxDescL = &ifp->HwRxDescH[ifp->NumRxDescH];


	// Allocate DrvTxDesc/DrvRxDesc
	ifp->DrvTxDescH = MemAlloc (sizeof (DRV_TX_DESC) * ifp->NumTxDescH, FALSE);
	ifp->DrvTxDescL = MemAlloc (sizeof (DRV_TX_DESC) * ifp->NumTxDescL, FALSE);
	ifp->DrvRxDescH = MemAlloc (sizeof (DRV_RX_DESC) * ifp->NumRxDescH, FALSE);
	ifp->DrvRxDescL = MemAlloc (sizeof (DRV_RX_DESC) * ifp->NumRxDescL, FALSE);

	TotalPktRequired = ifp->NumRxDescH + ifp->NumRxDescL + ifp->NumFreePkt;
	// Allocate Packet pool
	ifp->PktPool = MemAlloc (sizeof (DRV_PACKET_DESC) * TotalPktRequired, FALSE);
	// Allocate Buffer pool
	ifp->BufPool = MemAlloc (PKT_BUFFER_SIZE * TotalPktRequired + PKT_BUFFER_ALIGN, FALSE);
	// Align to PKT_BUFFER_ALIGN boundry
	if ((UINT32) ifp->BufPool & PKT_BUFFER_ALIGN)
		ifp->BufPool = (UINT8 *) (((UINT32) ifp->BufPool + PKT_BUFFER_ALIGN) & (~PKT_BUFFER_ALIGN));

	// Initialize Packet Descriptors
	for (Buf = ifp->BufPool, i = 0; i < TotalPktRequired; i++)
	{
		Pkt = &ifp->PktPool[i];
		Pkt->Next = NULL;
		Pkt->Buf = Buf;
		SetPktAlign (Pkt);
		Buf += PKT_BUFFER_SIZE;
	}

	// Initialize the TxDescH
	for (i = 0; i < ifp->NumTxDescH; i++)
	{
		//ifp->HwTxDescH[i].buf1ctnt = 0;
		//ifp->HwTxDescH[i].buf2cntl = 0;
		//ifp->HwTxDescH[i].buflen = 0;
		//ifp->HwTxDescH[i].pktcntl = 0;
		ifp->DrvTxDescH[i].Pkt = NULL;
	}
	ifp->HwTxDescH[--i].buf1cntl |= END_OF_RING;


	// Initialize the TxDescL
	for (i = 0; i < ifp->NumTxDescL; i++)
	{
		//ifp->HwTxDescL[i].buf1ctnt = 0;
		//ifp->HwTxDescL[i].buf2cntl = 0;
		//ifp->HwTxDescL[i].buflen = 0;
		//ifp->HwTxDescL[i].pktcntl = 0;
		ifp->DrvTxDescL[i].Pkt = NULL;
	}
	ifp->HwTxDescL[--i].buf1cntl |= END_OF_RING;


	// Initialize the RxDescH
	for (i = 0, PktIdx = 0; i < ifp->NumRxDescH; i++)
	{
		Pkt = ifp->DrvRxDescH[i].Pkt = &ifp->PktPool[PktIdx++];
		if (Pkt->buf2len != 0)
		{
			ifp->HwRxDescH[i].buf2cntl = BUF2_EN |
				((UINT32) (Pkt->Buf + Pkt->buf2off) & BUF_ADDR_MASK);
		} else
			ifp->HwRxDescH[i].buf2cntl = 0;

		ifp->HwRxDescH[i].buflen = Pkt->buf1len;
		ifp->HwRxDescH[i].buf1cntl = OWN_BIT |
			((UINT32) (Pkt->Buf + Pkt->buf1off) & BUF_ADDR_MASK);
	}
	ifp->HwRxDescH[--i].buf1cntl |= END_OF_RING;

	// Initialize the RxDescL
	for (i = 0; i < ifp->NumRxDescL; i++)
	{
		Pkt = ifp->DrvRxDescL[i].Pkt = &ifp->PktPool[PktIdx++];
		if (Pkt->buf2len != 0)
		{
			ifp->HwRxDescL[i].buf2cntl = BUF2_EN |
				((UINT32) (Pkt->Buf + Pkt->buf2off) & BUF_ADDR_MASK);
		} else
			ifp->HwRxDescL[i].buf2cntl = 0;

		ifp->HwRxDescL[i].buflen = Pkt->buf1len;
		ifp->HwRxDescL[i].buf1cntl = OWN_BIT |
			((UINT32) (Pkt->Buf + Pkt->buf1off) & BUF_ADDR_MASK);
	}
	ifp->HwRxDescL[--i].buf1cntl |= END_OF_RING;

	// Initialize FreePktList
	ifp->FreePktList = &ifp->PktPool[PktIdx];
	for (; PktIdx < TotalPktRequired - 1; PktIdx++)
	{
		ifp->PktPool[PktIdx].Next = &ifp->PktPool[PktIdx + 1];
	}
	ifp->FreePktCnt = ifp->NumFreePkt;

	// Initialize Rx/Tx Indices
	ifp->RxIdxH = ifp->RxIdxL = 0;
	ifp->TxIdxH_Head = ifp->TxIdxH_Tail = ifp->TxIdxL_Head = ifp->TxIdxL_Tail = 0;

	// Initialize the Tx queues
	ifp->TxH_QueueHead = ifp->TxH_QueueTail = ifp->TxL_QueueHead = ifp->TxL_QueueTail = NULL;

	return 0;
}

int ProgramMac (int vlan, UINT8 *Mac)
{
	UINT32 Reg0, Reg1;

	if (vlan < 0 || vlan > 6) return -1;

	Reg0 = (((UINT32) Mac[1] << 24) + ((UINT32) Mac[0] << 16)) | (vlan << SW_MAC_VLANID_SHIFT)
		| SW_MAC_AGE_VALID | SW_MAC_WRITE | SW_MAC_VLANID_EN;

	Reg1 = ((UINT32) Mac[5] << 24) + ((UINT32) Mac[4] << 16)+ ((UINT32) Mac[3] << 8) + Mac[2];


	ADM5120_SW_REG (MAC_wt1_REG) = Reg1;
	ADM5120_SW_REG (MAC_wt0_REG) = Reg0;

	while (!(ADM5120_SW_REG (MAC_wt0_REG) & SW_MAC_WRITE_DONE));

	return 0;
}

static int ProcessRxHInt (void)
{
	buart_print ("\n\rHigh priority receive and send are not supported!");
	return FALSE;
}

static int ProcessRxLInt (void)
{
	UINT32 LoopCnt = 0, idx, RxStatus;
	PDRV_PACKET_DESC Pkt, FreePkt;
	RX_DESC_T *RxDesc;
	PORT_STAT *pstat;

	idx = ifp->RxIdxL;
	RxDesc = &ifp->HwRxDescL[idx];
	while (!(RxDesc->buf1cntl & OWN_BIT))
	{
		if (ifp->FreePktList == NULL)
		{
			ifp->RxIdxL = idx;
			ifp->RxL_PktCnt += LoopCnt;
			return TRUE;
		}

		// Store the received packet
		Pkt = ifp->DrvRxDescL[idx].Pkt;
		if ((RxStatus = RxDesc->status) == 0)
		{
			// Reuse the Pkt buffer
			RxDesc->status = 0;
			RxDesc->buf1cntl = RxDesc->buf1cntl | OWN_BIT;
			if (++idx >= ifp->NumRxDescL) idx = 0;
			RxDesc = &ifp->HwRxDescL[idx];
			continue;
		}
		Pkt->SrcPort = (RxStatus & SRC_PORT_MASK) >> SRC_PORT_SHIFT;
		Pkt->PktLen = ((RxStatus & PKT_LEN_MASK) >> PKT_LEN_SHIFT) - ETH_CRC_LEN;
		Pkt->flag = RxStatus & RX_PKT_FLAG_MASK;

		// Get another free packet for the receive descriptor
		FreePkt = ifp->DrvRxDescL[idx].Pkt = ifp->FreePktList;
		ifp->FreePktList = FreePkt->Next;
		ifp->FreePktCnt--;
		FreePkt->Next = NULL;

		// Set the descriptor and release it to switch
		RxDesc->status = 0;
		if (FreePkt->buf2len != 0)
		{
			RxDesc->buf2cntl = BUF2_EN |
				(((UINT32) FreePkt->Buf + FreePkt->buf2off) & BUF_ADDR_MASK);
		} else
			RxDesc->buf2cntl = 0;

		RxDesc->buflen = FreePkt->buf1len;
		RxDesc->buf1cntl = (RxDesc->buf1cntl & END_OF_RING) | OWN_BIT
			| (((UINT32) FreePkt->Buf + FreePkt->buf1off) & BUF_ADDR_MASK);

		if (Pkt->PktLen <= Pkt->buf1len)
		{
			dcache_invalidate_block (Pkt->Buf + Pkt->buf1off, Pkt->PktLen);
		} else
		{
			dcache_invalidate_block (Pkt->Buf + Pkt->buf1off, Pkt->buf1len);
			dcache_invalidate_block (Pkt->Buf + Pkt->buf2off, Pkt->PktLen - Pkt->buf1len);
		}

		// Collect statistic information
		pstat = &ifp->portstat[Pkt->SrcPort];
		if (Pkt->flag & RX_FRAME_BC)
		{
			ifp->RxL_BCCnt++;
			++pstat->RxBCPkt;
		} else if (Pkt->flag & RX_FRAME_MC)
		{
			ifp->RxL_MCCnt++;
			++pstat->RxMCPkt;
		} else
		{
			ifp->RxL_UCCnt++;
			++pstat->RxUCPkt;
		}
		pstat->RxPktCnt++;
		pstat->RxBytes += Pkt->PktLen;

		IndicateRxPacketL (Pkt);

		if (++idx >= ifp->NumRxDescL) idx = 0;

		if (++LoopCnt > ifp->NumRxDescL) break;
		RxDesc = &ifp->HwRxDescL[idx];
	}
	ifp->RxIdxL = idx;
	ifp->RxL_PktCnt += LoopCnt;

	return FALSE;
}

static void ProcessTxLInt (void)
{
	UINT32 LoopCnt = 0, idx;
	PDRV_PACKET_DESC Pkt;
	idx = ifp->TxIdxL_Tail;
	while (!(ifp->HwTxDescL[idx].buf1cntl & OWN_BIT)
		&& (idx != ifp->TxIdxL_Head))
	{
		Pkt = ifp->DrvTxDescL[idx].Pkt;
		ifp->DrvTxDescL[idx].Pkt = NULL;
		// Return the Packet to the Free Packet List
		SetPktAlign (Pkt);
		Pkt->Next = ifp->FreePktList;
		ifp->FreePktList = Pkt;
		ifp->FreePktCnt++;

		if (++idx == ifp->NumTxDescL) idx = 0;
		if (++LoopCnt > ifp->NumTxDescL) break;
	}
	ifp->TxIdxL_Tail = idx;
	ifp->TxL_PktCnt += LoopCnt;

	// Try to send the packets in the transmitting queue.
	if (ifp->TxL_QueueHead != NULL)
		SendPacketsL (NULL);
}


/**********************************************************************************/
/* SendPacketsL:																  */
/*     Send a low priority packet and try to transmit all the packet in the low   */
/*     priority transmitting queue.												  */
/**********************************************************************************/
void SendPacketsL (PDRV_PACKET_DESC Pkt)
{
	UINT32 Idx, IdxNext;
	UINT32 TxCnt = 0;
	TX_DESC_T *TxDesc;
	UINT8 *buf;

	if (ifp->TxL_QueueHead != NULL) //The txL Queue has already some packet, send them first..
	{
		ENQUEUE_TX_PACKET_AT_TAIL (ifp->TxL_QueueHead, ifp->TxL_QueueTail, Pkt);
		DEQUEUE_TX_PACKET (ifp->TxL_QueueHead, ifp->TxL_QueueTail, Pkt);
	}
	Idx = IdxNext = ifp->TxIdxL_Head;

	// try to transmit queued packets.
	while (Pkt != NULL)
	{
		// Check if the Tx queue is full. There is always one descriptor kept empty for queue full detection.
		if ((++IdxNext) >= ifp->NumTxDescL) IdxNext = 0;
		if (IdxNext == ifp->TxIdxL_Tail)
		{
			ifp->TxL_FullCnt++;
			break;
		}
		TxDesc = &ifp->HwTxDescL[Idx];

		// If there is any free Tx DESC
		if (TxDesc->buf1cntl & OWN_BIT) break;

		// Transmit the packet
		ifp->DrvTxDescL[Idx].Pkt = Pkt;

		if (ifp->CtrlFlag & PKT_SOFTWARE_CRC)
			Pkt->PktLen += ETH_CRC_LEN;

		if (Pkt->PktLen <= Pkt->buf1len)
		{
			TxDesc->buf2cntl = 0;
			TxDesc->buflen = Pkt->PktLen;
		} else
		{
			TxDesc->buflen = Pkt->buf1len;
			TxDesc->buf2cntl = BUF2_EN |
				((UINT32) (Pkt->Buf + Pkt->buf2off) & BUF_ADDR_MASK);
		}
		Pkt->Dst = (ADM5120_SW_REG (PHY_st_REG) & PORT_LINK_MASK) << 8;
		buf = Pkt->Buf + Pkt->buf2off;
		TxDesc->pktcntl = (Pkt->PktLen << PKT_LEN_SHIFT) | Pkt->Dst;
		if (dslam_present)
			TxDesc->pktcntl |= 0x00002000; // force 6-th port

		TxDesc->buf1cntl = (TxDesc->buf1cntl & END_OF_RING) | OWN_BIT
			| ((UINT32) (Pkt->Buf + Pkt->buf1off) & BUF_ADDR_MASK);

		Idx = IdxNext;

		if (++TxCnt >= ifp->NumTxDescL) Pkt = NULL;
		else
			DEQUEUE_TX_PACKET (ifp->TxL_QueueHead, ifp->TxL_QueueTail, Pkt);
	}
	ifp->TxIdxL_Head = Idx;

	// Start the transmitting machine.
	//	if(TxCnt > 0) ADM5106B_SW_REG(Send_trig_REG) = SEND_TRIG_LOW;
	ADM5120_SW_REG (Send_trig_REG) = SEND_TRIG_LOW;


	// Requeue the last packet if it was not transmitted.
	ENQUEUE_TX_PACKET_AT_FRONT (ifp->TxL_QueueHead, ifp->TxL_QueueTail, Pkt);
}


/*void ProcessLinkInt(PAM5120CONTEXT ifp)
{
	UINT32 link_change, link_status;
	int i=0;

	buart_print("+P");

	link_status = ADM5120_SW_REG(PHY_st_REG) & PORT_LINK_MASK;

	// Flip PORT_MII_LINK_DOWN bit.
	link_status ^= PORT_MII_LINKFAIL;
	link_change = link_status ^ ifp->link_status;
	ifp->link_status = link_status;
	while(link_change)
	{
		if(link_change & 0x1)
		{
			buart_put(i+0x30);
			if(link_status & (1 << i))
				buart_put('u');
			else
				buart_put('d');
		}
		link_change>>=1;
		i++;
	}

}
 */

inline void SetPktAlign (PDRV_PACKET_DESC Pkt)
{
	UINT32 buf2off;

	// Set buffer1 offset
	if (ifp->Buf1Off == BUF1_OFFSET_RAND)
	{
		// Buffer1 offset random
		Pkt->buf1off = rand () & BUF1_OFFSET_MAX;
	} else
		Pkt->buf1off = ifp->Buf1Off;

	if (ifp->CtrlFlag & SW_TWOBUF_PER_DESC)
	{
		// Two buffer per descriptor

		// Set Buffer1 length
		if (ifp->Buf1Len == BUF1_LEN_RAND)
			Pkt->buf1len = BUF1_LEN_MIN + (rand () & BUF1_LEN_RAND_MAX);
		else
			Pkt->buf1len = ifp->Buf1Len;

		// Set buffer 2 offset
		buf2off = (Pkt->buf1off + Pkt->buf1len + BUF_GUARD_BAND) & ~(ADM5120_CACHE_LINE_SIZE - 1);
		if (ifp->Buf2Off == BUF2_OFFSET_RAND)
			Pkt->buf2off = buf2off + (rand () & BUF2_OFFSET_MAX);
		else
			Pkt->buf2off = buf2off + ifp->Buf2Off;

		// Set buffer 2 length
		Pkt->buf2len = PKT_BUFFER_SIZE - Pkt->buf2off;

	} else
	{
		// One buffer per descriptor
		Pkt->buf1len = PKT_BUFFER_SIZE - Pkt->buf1off;
		Pkt->buf2len = Pkt->buf2off = 0;
	}
}

/**********  Exported Functions  ****************************************************/
int if5120_init (void)
{
	int i, eth_num;
	UINT32 reg1, reg2;
	UINT8 MacAddr[6];

	// install exception handler
	install_exception ();

	// Init irq
	sys_irq_init ();

	// Per port PHY reset and enable auto-negotiation.
	ADM5120_SW_REG (PHY_cntl2_REG) |= SW_PHY_AN_MASK | SW_PHY_NORMAL_MASK;

	//Enable MII AN monitor via MDIO.(Dumb Mode)
	//ADM5120_SW_REG (Port_conf2_REG) |= SW_GMII_AN_EN;
	ADM5120_SW_REG (Port_conf2_REG) = SW_GMII_FORCE_SPD_100M |
									  SW_GMII_FORCE_FULL_DUPLEX |
									  SW_GMII_TXC_CHECK_EN | SW_GMII_EN;

	// Allocate the driver context
	ifp = (PAM5120CONTEXT) MemAlloc (sizeof (AM5120CONTEXT), TRUE);
	if (ifp == NULL) return -1;

	// Disable Switch Interrupts
	ADM5120_SW_REG (SW_Int_mask_REG) = 0xFFFFFFF;

	// Set Phy control values
	ADM5120_SW_REG (PHY_cntl4_REG) = PHY_VOLT23 | PHY_ROMCODE_25;

	if (DrvAllocateResource () != 0)
		buart_print ("\n\rSwitch allocate resource error.");

	//  Hw initialization starts here
	// Initialize the ifp Desc
	ADM5120_SW_REG (Send_HBaddr_REG) = (UINT32) ifp->HwTxDescH;
	ADM5120_SW_REG (Send_LBaddr_REG) = (UINT32) ifp->HwTxDescL;
	ADM5120_SW_REG (Recv_HBaddr_REG) = (UINT32) ifp->HwRxDescH;
	ADM5120_SW_REG (Recv_LBaddr_REG) = (UINT32) ifp->HwRxDescL;

	if (ifp->CtrlFlag & PKT_SOFTWARE_CRC)
		ADM5120_SW_REG (CPUp_conf_REG) = CPU_PORT_DEFAULT;
	else
		ADM5120_SW_REG (CPUp_conf_REG) = CPU_PORT_DEFAULT | SW_PADING_CRC;

	// Daniel fix
	// Disable all port, enable MC, BP and FC
	ADM5120_SW_REG (Port_conf0_REG) = SW_DISABLE_PORT_MASK | SW_EN_MC_MASK |
		SW_EN_BP_MASK | SW_EN_FC_MASK;

	// Init VLAN group Regs, VLAN_GI & VLAN_GII
	reg1 = PORT_ALL;
	//reg1 = 0x20; // cpu
	reg2 = PORT_NONE;
	ADM5120_SW_REG (VLAN_G1_REG) = reg1;
	ADM5120_SW_REG (VLAN_G2_REG) = reg2;

	if (!(ADM5120_SW_REG (Global_St_REG) & ALLMEM_TEST_DONE))
	{
		buart_print ("\n\rWaiting for switch memory test...");
		while (!(ADM5120_SW_REG (Global_St_REG) & ALLMEM_TEST_DONE));
		buart_print ("\n\rSwitch memory test done!!\r\n");
		for (i = 0; i < 2000; i++);
	}

	// Program Vlan0 MAC address
	bsp_GetMacBase (MacAddr, &eth_num);
	ProgramMac (VLAN0, MacAddr);

	// Clear all interrupt status bit if any one was set
	reg1 = ADM5120_SW_REG (SW_Int_st_REG);
	ADM5120_SW_REG (SW_Int_st_REG) = reg1;

	// Connect Interrupt
	irqConnect (INT_LVL_SWITCH, IRQ_INT_MODE, (IRQ_HANDLER) Am5120Isr, 0, 0);
	IrqEnable (INT_LVL_SWITCH);

	// Enable CPU port
	reg1 = ADM5120_SW_REG (CPUp_conf_REG) &(~SW_CPU_PORT_DISABLE);
	ADM5120_SW_REG (CPUp_conf_REG) = reg1;

	// Enable VLAN0 port
	reg1 = ADM5120_SW_REG (Port_conf0_REG) & (~PORT_DISABLE_MASK);
	//reg1 = ADM5120_SW_REG (Port_conf0_REG) & (~0x00000020);
	ADM5120_SW_REG (Port_conf0_REG) = reg1;

	// Clear Watchdog 1 counter
	reg1 = ADM5120_SW_REG (Watchdog1_REG);
	ADM5120_SW_REG (Watchdog1_REG) = (5 << WATCHDOG_TIMER_SET_SHIFT);
	ifp->IntMask = AM5120_INT_MASK;
	ifp->link_status = 0;

	// Enable switch interrupts
	ADM5120_SW_REG (SW_Int_mask_REG) = ~ifp->IntMask;

	return 0;
}

void if5120shutdown (void)
{
	int i, s;

	s = mips_int_lock ();

	ADM5120_SW_REG (Port_conf0_REG) |= SW_DISABLE_PORT_MASK;
	ADM5120_SW_REG (CPUp_conf_REG) |= SW_CPU_PORT_DISABLE;

	mips_int_unlock (s);

	for (i = 0; i < 1000000; i++);

}

void if5120turnon (void)
{
	int i, s;
	UINT32 reg;

	s = mips_int_lock ();
	// Re-enable ports
	reg = ADM5120_SW_REG (Port_conf0_REG) & ~SW_DISABLE_PORT_MASK;
	ADM5120_SW_REG (Port_conf0_REG) = reg;
	// Re-enable CPU port
	reg = ADM5120_SW_REG (CPUp_conf_REG) & ~SW_CPU_PORT_DISABLE;
	ADM5120_SW_REG (CPUp_conf_REG) = reg;
	mips_int_unlock (s);

	for (i = 0; i < 1000000; i++);

}

PDRV_PACKET_DESC Am5120_GetFreePkt (void)
{
	PDRV_PACKET_DESC Pkt = NULL;
	int s;
	s = mips_int_lock ();

	if (ifp->FreePktList != NULL)
	{
		Pkt = ifp->FreePktList;
		ifp->FreePktList = Pkt->Next;
		ifp->FreePktCnt--;
		Pkt->Next = NULL;
	}

	mips_int_unlock (s);
	return Pkt;
}

void Am5120_RefreePkt (PDRV_PACKET_DESC Pkt)
{
	int irq_state;

	irq_state = mips_int_lock ();

	Pkt->Next = ifp->FreePktList;
	ifp->FreePktList = Pkt;
	ifp->FreePktCnt++;

	mips_int_unlock (irq_state);
}

/***** Switch Interrupt Handler ******************************************************/
void Am5120Isr (int intLev)
{
	UINT32 IntReg, IntMask/*, reg*/;
	int LoopCnt = 0, RxFull = 0;

	// Disable Switch Interrupts
	ADM5120_SW_REG (SW_Int_mask_REG) = 0xFFFFFFF;

	// Clear watchdot timer 1
	// 	reg = ADM5120_SW_REG(Watchdog1_REG);
	// Ack all interrupts
	IntReg = ADM5120_SW_REG (SW_Int_st_REG);
	ADM5120_SW_REG (SW_Int_st_REG) = IntReg;

	IntReg = ifp->IntStatus | (IntReg & AM5120_INT_MASK);
	while (IntReg && LoopCnt < MAX_INT_LOOP_CNT)
	{
		//		if(IntReg &  GLOBAL_QUE_FULL_INT) ifp->DropFlag = TRUE;
		//buart_put('I');
		if (IntReg & LINK_INT); //ProcessLinkInt(ifp);
		if (IntReg & TX_H_INT); //ProcessTxHInt(ifp);
		if (IntReg & TX_L_INT) ProcessTxLInt ();
		if (IntReg & RX_H_INT)
		{
			if (ProcessRxHInt () != 0)
				RxFull |= RX_H_FULL_INT;
		}
		if (IntReg & RX_L_INT)
		{
			if (ProcessRxLInt () != 0)
				RxFull |= RX_L_FULL_INT;
		}

		//		if(IntReg & MUST_DROP_INT)	buart_put('d');
		IntReg = ADM5120_SW_REG (SW_Int_st_REG);
		// Acknowledge the interrupts
		ADM5120_SW_REG (SW_Int_st_REG) = IntReg;
		IntReg &= AM5120_INT_MASK;
		LoopCnt++;
	}

	// Record unhandled interrupts
	ifp->IntStatus = IntReg | RxFull;

	// If the rx H/L descriptor is still full, mask  its interrupt temporarily.
	// If it is not masked, the interrupt will be asserted immediately after
	// the switch's IRQ being re-enabled. This will block the system from doing
	// anything other than servicing the switch's ISR/DSR routine. However, the
	// ISR/DSR is waiting the system to return some of its buffers for receiving.
	// A dead lock situation is entered.
	IntMask = ~ifp->IntMask | RxFull;


	// Enable switch interrupts
	ADM5120_SW_REG (SW_Int_mask_REG) = IntMask;

	ifp->IntCnt++;
}



