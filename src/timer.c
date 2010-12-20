#include <ctype.h>
#include <mips4kc.h>
#include <adm5120.h>

static UINT32 timeticks = 0;

void InitTimer (void)
{
	ADM5120_SW_REG (Timer_int_REG) = SW_TIMER_INT_DISABLE | SW_TIMER_INT;
	ADM5120_SW_REG (Timer_REG) = SW_TIMER_10MS_TICKS | SW_TIMER_EN;
	timeticks = 1;
}

UINT32 UpTime (void)
{
	unsigned long status;

	status = ADM5120_SW_REG (Timer_int_REG);

	if (status & SW_TIMER_INT)
	{
		/* Acknowledge the timer int */
		ADM5120_SW_REG (Timer_int_REG) = status;
		timeticks++;
	}

	return timeticks;
}

void DisableTimer (void)
{
	/*buart_print("\n\rTimer irqdisable..");
	irqDisconnect(INT_LVL_TIMER);
	IrqDisable(INT_LVL_TIMER);*/
}

