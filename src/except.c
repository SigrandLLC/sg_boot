/*****************************************************************************
;
;    Project : ADM5120
;    Creator : David Weng
;    File    : except.c
;    Abstract: 
;
;*****************************************************************************/

#include <ctype.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <except.h>

#include <ctype.h>

#define MIPS_INT_UNREGISTERD	0
#define MIPS_INT_REGISTERD		1
#define MIPS_INT_ENABLED		2

typedef struct mips_int
{
	int flags;
	mips_int_hdl hdl;
	UINT32 parm0;
	UINT32 parm1;
} MIPS_INT_ENTRY;



extern UINT32 mips_cp0_status_read (void);
extern void mips_cp0_status_write (UINT32 val);
extern int _splset (int);

MIPS_INT_ENTRY mips_int_tab[] ={
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_SOFT0
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_SOFT1
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_IRQ
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_FIQ
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_REV0
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_REV1
	{MIPS_INT_UNREGISTERD, NULL, 0, 0}, // ADM5120_MIPSINT_REV2
	{MIPS_INT_UNREGISTERD, NULL, 0, 0} // ADM5120_MIPSINT_TIMER
};

void int_hdl (UINT32 status_reg, UINT32 cause_reg)
{
	int i = 7;

	cause_reg &= status_reg;
	cause_reg = (cause_reg & CP0_CAUSE_IP_MASK) << 16;

	for (i = 7; cause_reg; i--)
	{
		if (cause_reg & 0x80000000)
		{
			if ((mips_int_tab[i].flags & MIPS_INT_ENABLED) &&
				(mips_int_tab[i].hdl != NULL))
			{
				// Call the interrupt handler
				(mips_int_tab[i].hdl)(i, mips_int_tab[i].parm0, mips_int_tab[i].parm1);
			} else
			{
				// MASK the interrupt source
				mips_int_disable (i);
			}
		}

		cause_reg <<= 1;
	}
}

void gexcept_hdl (UINT32 status_reg, UINT32 cause_reg, UINT32 epc_reg)
{
	int exc_code;

	exc_code = (cause_reg & CP0_CAUSE_EXCCODE_MASK) >> CP0_CAUSE_EXCCODE_SHIFT;

	if (exc_code == EXCCODE_INT)
	{
		int_hdl (status_reg, cause_reg);
	} else
	{
		// Panic !!!	
		while (1);
	}

}

void install_exception (void)
{
	static int installed = 0;
	UINT32 status_val, addr;
	int len;

	extern char TLBrefill_except[], TLBrefill_exceptEnd[];
	extern char general_except[], general_exceptEnd[];
	extern char int_except[], int_exceptEnd[];
	if (installed++ != 0) return;

	// install tlb exception vector
	len = TLBrefill_exceptEnd - TLBrefill_except;
	memcpy ((void *) MIPS_TLBRIFLL_VECTOR, TLBrefill_except, len);

#ifndef KSEG0_UNCACHED
#if 0
	for (addr = MIPS_TLBRIFLL_VECTOR; len > 0; len -= 0x10)
	{
		icahce_invalidate_addr (addr);
		addr += 0x10;
	}
#else
	icache_invalidate_block (MIPS_TLBRIFLL_VECTOR, len);
#endif

#endif

	// install general exception vector
	len = general_exceptEnd - general_except;
	memcpy ((void *) MIPS_GENERAL_VECTOR, general_except, len);

#ifndef KSEG0_UNCACHED
#if 0
	for (addr = MIPS_GENERAL_VECTOR; len > 0; len -= 0x10)
	{
		icahce_invalidate_addr (addr);
		addr += 0x10;
	}
#else
	icache_invalidate_block (MIPS_GENERAL_VECTOR, len);
#endif
#endif

	len = int_exceptEnd - int_except;
	memcpy ((void *) MIPS_INTERRUPT_VECTOR, int_except, len);

#ifndef KSEG0_UNCACHED
#if 0
	for (addr = MIPS_INTERRUPT_VECTOR; len > 0; len -= 0x10)
	{
		icahce_invalidate_addr (addr);
		addr += 0x10;
	}
#else
	icache_invalidate_block (MIPS_INTERRUPT_VECTOR, len);
#endif

#endif		

	// Clear BEV, all IM bis in status reg and ENABLE interrupt
	status_val = mips_cp0_status_read () | CP0_STATUS_IE_BIT;
	status_val &= ~(CP0_STATUS_BEV_BIT | CP0_STATUS_IM_MASK);
	mips_cp0_status_write (status_val);
}

int mips_int_connect (int intnum, mips_int_hdl hdl, UINT32 parm0, UINT32 parm1)
{
	int s;

	if (intnum < ADM5120_MIPSINT_SOFT0 || intnum > ADM5120_MIPSINT_TIMER)
		return -1;
	if (hdl == NULL)
		return -1;

	// Disable mips int
	s = _splset (0);
	if (mips_int_tab[intnum].flags & MIPS_INT_REGISTERD)
	{
		_splset (s);
		return -1;
	}
	mips_int_tab[intnum].flags = MIPS_INT_REGISTERD;
	mips_int_tab[intnum].hdl = hdl;
	mips_int_tab[intnum].parm0 = parm0;
	mips_int_tab[intnum].parm1 = parm1;

	_splset (s);

	return 0;
}

int mips_int_disconnect (int intnum)
{
	int s;

	if (intnum < ADM5120_MIPSINT_SOFT0 || intnum > ADM5120_MIPSINT_TIMER)
		return -1;

	// Disable mips int
	s = _splset (0);
	if (!(mips_int_tab[intnum].flags & MIPS_INT_REGISTERD))
		goto _exit;

	// Remove the interrupt handler		
	mips_int_tab[intnum].flags = MIPS_INT_UNREGISTERD;
	mips_int_tab[intnum].hdl = NULL;

	// Clear the corresponding IM bit
	s &= ~((0x1 << intnum) << CP0_STATUS_IM_SHIFT);

_exit:
	_splset (s);
	return 0;
}

int mips_int_enable (int intnum)
{
	int s, err = -1;

	if (intnum < ADM5120_MIPSINT_SOFT0 || intnum > ADM5120_MIPSINT_TIMER)
		return err;

	// Disable mips int
	s = _splset (0);
	if (mips_int_tab[intnum].flags & MIPS_INT_REGISTERD)
	{
		mips_int_tab[intnum].flags |= MIPS_INT_ENABLED;
		// Set corresponding IM bit
		s |= (0x1 << intnum) << CP0_STATUS_IM_SHIFT;
		err = 0;
	}
	// restore mips int
	_splset (s);
	return err;
}

int mips_int_disable (int intnum)
{
	int s, err = -1;

	if (intnum < ADM5120_MIPSINT_SOFT0 || intnum > ADM5120_MIPSINT_TIMER)
		return err;

	// Disable mips int
	s = _splset (0);
	if (mips_int_tab[intnum].flags & MIPS_INT_REGISTERD)
	{
		mips_int_tab[intnum].flags &= ~MIPS_INT_ENABLED;

		// Clear the corresponding IM bit
		s &= ~((0x1 << intnum) << CP0_STATUS_IM_SHIFT);
		err = 0;
	}
	// Restore mips int
	_splset (s);
	return err;
}

void panic (void)
{
	_splset (0);

	while (1);
}

