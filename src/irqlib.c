#include <ctype.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <except.h>
#include <irqlib.h>


/***************************  Variablers ********************************/
static IRQ_OBJ irq_tab[INT_LVL_MAX + 1] ={
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_TIMER
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_UART0
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_UART1
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_USBHOST
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_EXTIO_0
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_EXTIO_1
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_PCI_0
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_PCI_1
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0}, // INT_LVL_PCI_2
	{IRQL_NOT_CONNECTED, IRQ_INT_MODE, NULL, 0, 0} // INT_LVL_SWITCH
};


static int irq_init = 0;

/***************************  Inline Functions ***************************/

static __inline void _irq_enable (int irql)
{
	ADM5120_INTC_REG (IRQ_ENABLE_REG) = (1 << irql);
}

static __inline void _irq_disable (int irql)
{
	ADM5120_INTC_REG (IRQ_DISABLE_REG) = (1 << irql);
}

void IrqEnable (int irql)
{
	int s;

	// Disable all interrupts (FIQ/IRQ) 
	s = mips_int_lock ();

	if ((irql < 0) || (irql > INT_LVL_MAX) ||
		(irq_tab[irql].status == IRQL_NOT_CONNECTED)) goto err_exit;

	irq_tab[irql].status = IRQL_ENABLED;
	_irq_enable (irql);

err_exit:
	// Restore the interrupts states 
	mips_int_unlock (s);
}

void IrqDisable (int irql)
{
	int s;

	// Disable all interrupts (FIQ/IRQ) 
	s = mips_int_lock ();

	if ((irql < 0) || (irql > INT_LVL_MAX) ||
		(irq_tab[irql].status == IRQL_NOT_CONNECTED)) goto err_exit;

	_irq_disable (irql);
	irq_tab[irql].status = IRQL_DISABLED;

err_exit:
	// Restore the interrupts states 
	mips_int_unlock (s);
}

UINT32 irqConnect (int irql, int mode, IRQ_HANDLER handler, UINT32 parm0, UINT32 parm1)
{
	int s;
	UINT32 reg;

	if (irq_init == 0) return FALSE;

	if (mode != IRQ_INT_MODE && mode != FIQ_INT_MODE)
		return FALSE;

	if ((irql < 0) || (irql > INT_LVL_MAX))
		return FALSE;

	// Disable all interrupts (FIQ/IRQ)
	s = mips_int_lock ();
	if (irq_tab[irql].status != IRQL_NOT_CONNECTED)
	{
		mips_int_unlock (s);
		return FALSE;
	}

	irq_tab[irql].irqmode = mode;
	irq_tab[irql].parm0 = parm0;
	irq_tab[irql].parm1 = parm1;
	irq_tab[irql].handler = handler;
	irq_tab[irql].status = IRQL_DISABLED;

	if (mode == FIQ_INT_MODE)
	{
		reg = ADM5120_INTC_REG (IRQ_MODE_REG) | (1 << irql);
		ADM5120_INTC_REG (IRQ_MODE_REG) = reg;
	} else
	{
		reg = ADM5120_INTC_REG (IRQ_MODE_REG) & ~(1 << irql);
		ADM5120_INTC_REG (IRQ_MODE_REG) = reg;
	}

	// Restore the interrupts states 
	mips_int_unlock (s);
	return TRUE;
}

void irqDisconnect (int irql)
{
	int s;

	if (irq_init == 0) return;

	// Disable all interrupts (FIQ/IRQ)
	s = mips_int_lock ();
	if ((irql < 0) || (irql > INT_LVL_MAX) ||
		(irq_tab[irql].status == IRQL_NOT_CONNECTED)) goto ErrExit;

	// Is the irql still enabled?
	if (irq_tab[irql].status != IRQL_DISABLED)
	{
		_irq_disable (irql);
	}

	// Clear the IRQ OBJ entry	
	irq_tab[irql].status = IRQL_NOT_CONNECTED;
	irq_tab[irql].handler = NULL;
	irq_tab[irql].parm0 = 0;
	irq_tab[irql].parm1 = 0;

ErrExit:
	// Restore the interrupts states
	mips_int_unlock (s);
}

void irq_lock (void)
{
	mips_int_disable (ADM5120_MIPSINT_IRQ);
}

void irq_unlock (void)
{
	mips_int_enable (ADM5120_MIPSINT_IRQ);
}

void fiq_lock (void)
{
	mips_int_disable (ADM5120_MIPSINT_FIQ);
}

void fiq_unlock (void)
{
	mips_int_enable (ADM5120_MIPSINT_FIQ);
}

static void irq_handler (int intnum)
{
	UINT32 intsrc;
	int i;
	IRQ_OBJ *pIRQ;

	intsrc = ADM5120_INTC_REG (IRQ_STATUS_REG) & IRQ_MASK;
	for (i = 0; intsrc; intsrc >>= 1, i++)
	{
		if (intsrc & 0x1)
		{
			pIRQ = &irq_tab[i];
			if ((pIRQ->status == IRQL_ENABLED) && (pIRQ->handler != NULL))
			{
				(pIRQ->handler)(i, pIRQ->parm0, pIRQ->parm1);
			} else
			{
				/* Unhandled irq */

				/* Disable the irql and try to limb */
				_irq_disable (i);
			}
		}
	}
}

void fiq_handler (int intnum)
{
	UINT32 intsrc;
	IRQ_OBJ *pIRQ;
	int i;

	intsrc = ADM5120_INTC_REG (FIQ_STATUS_REG) & IRQ_MASK;
	for (i = 0; intsrc; intsrc >>= 1, i++)
	{
		if (intsrc & 0x1)
		{
			pIRQ = &irq_tab[i];
			if ((pIRQ->status == IRQL_ENABLED) && (pIRQ->handler != NULL))
			{
				(pIRQ->handler)(i, pIRQ->parm0, pIRQ->parm1);
			} else
			{
				/* Unhandled fiq */

				/* Disable the irql and try to limb */
				_irq_disable (i);
			}
		}
	}
}

int sys_irq_init (void)
{
	int s;

	if (irq_init != 0) return -1;
	s = mips_int_lock ();

	mips_int_connect (ADM5120_MIPSINT_FIQ, (mips_int_hdl) fiq_handler, 0, 0);
	mips_int_connect (ADM5120_MIPSINT_IRQ, (mips_int_hdl) irq_handler, 0, 0);

	mips_int_enable (ADM5120_MIPSINT_FIQ);
	mips_int_enable (ADM5120_MIPSINT_IRQ);

	irq_init = 1;
	mips_int_unlock (s);

	return 0;
}
