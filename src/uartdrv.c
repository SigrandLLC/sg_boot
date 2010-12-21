/*****************************************************************************
;
;    Project : ADM5120
;    Creator : David Weng
;    File    : buart.c
;    Abstract: Serial driver controller
;
;*****************************************************************************/
#include <mips4kc.h>
#include <adm5120.h>
#include <buart.h>
#include <uartdrv.h>
#include <timer.h>
#include <string.h> // strncpy

#include <bconfig.h>

#define TRUE		1
#define FALSE		0
int errcnt = 0;
int orcnt = 0;

//=====================  Macros  ========================
#define UART_REG_READ(_uart, _reg)		\
	*((volatile unsigned long *)(((_uart)->io_base) + (_reg)))


#define UART_REG_WRITE(_uart, _reg, _val)		\
	*((volatile unsigned long *)(((_uart)->io_base) + (_reg))) = (_val)



//================== Global variables ===================
const UART_RATE_ENTRY rate_tab[] ={
	{1200, UART_1200bps_DIVISOR},
	{2400, UART_2400bps_DIVISOR},
	{9600, UART_9600bps_DIVISOR},
	{14400, UART_14400bps_DIVISOR},
	{19200, UART_19200bps_DIVISOR},
	{38400, UART_38400bps_DIVISOR},
	{57600, UART_57600bps_DIVISOR},
	{115200, UART_115200bps_DIVISOR}
};


const int rate_option = sizeof (rate_tab) / sizeof (UART_RATE_ENTRY);

UART_OBJ uart;




//===============================================================================

int InitUart (int port, int rate)
{
	if (port == UART_PORT0)
		uart.io_base = PA2VA (ADM5120_UART0_BASE);
	else if (port == UART_PORT1)
		uart.io_base = PA2VA (ADM5120_UART1_BASE);
	else
		return FALSE;

	/* Set baudrate */
	buart_set_baud (rate);

	/* Set default line mode */
	UART_REG_WRITE (&uart, UART_LCR_H_REG, UART_WLEN_8BITS | UART_ENABLE_FIFO);

	/* Enable uart port */
	UART_REG_WRITE (&uart, UART_CR_REG, UART_PORT_EN);

	return FALSE;
}

/*--------------------------------------------------------------
 * ROUTINE NAME - buart_init
 *---------------------------------------------------------------
 * FUNCTION:  This function is used to initialize the uart channel
 *            but leaves the interrupt disabled
 * INPUT:
 * OUTPUT:
 * RETURN:
 *--------------------------------------------------------------*/
void buart_init (void)
{
	InitUart (UART_PORT0, 115200);
}

/*--------------------------------------------------------------
 * ROUTINE NAME - buart_init
 *---------------------------------------------------------------
 * FUNCTION:  This function is used to initialize the uart channel
 *            but leaves the interrupt disabled
 * INPUT:
 * OUTPUT:
 * RETURN:
 *--------------------------------------------------------------*/
void buart_set_baud (int baud_rate)
{
	int i;
	UINT32 lcr_h;

	for (i = 0; i < rate_option; i++)
		if (rate_tab[i].rate == baud_rate) break;
	if (i == rate_option) return;

	uart.baudrate = baud_rate;

	/* Save lcr_h */
	lcr_h = UART_REG_READ (&uart, UART_LCR_H_REG);

	UART_REG_WRITE (&uart, UART_LCR_M_REG, rate_tab[i].div >> 8);
	UART_REG_WRITE (&uart, UART_LCR_L_REG, rate_tab[i].div);

	/* Write lcr_h to update the lcr register */
	UART_REG_WRITE (&uart, UART_LCR_H_REG, lcr_h);
}

/*--------------------------------------------------------------
 * ROUTINE NAME - buart_read
 *---------------------------------------------------------------
 * FUNCTION: This function read one byte from the uart channel
 *           if the one byte is in the receive buffer of this
 *           chanel or -1 if nothing received.
 *
 * INPUT:
 * OUTPUT:
 * RETURN:   -1      -- BUSY
 *           else    -- byte received
 *--------------------------------------------------------------*/
static int buart_read (void)
{
	UINT32 uartfr;
	UINT32 uartsr, data;

	uartfr = UART_REG_READ (&uart, UART_FR_REG);

	if (!(uartfr & UART_RX_FIFO_EMPTY))
	{
		data = UART_REG_READ (&uart, UART_DR_REG);
		uartsr = UART_REG_READ (&uart, UART_RSR_REG);
		if (uartsr & UART_RX_ERROR)
		{
			errcnt++;
			UART_REG_WRITE (&uart, UART_ECR_REG, uartsr);
			return -1;
		}
		if (uartsr & UART_OVERRUN_ERR)
		{
			orcnt++;
			UART_REG_WRITE (&uart, UART_ECR_REG, uartsr);
		}

		return (data & 0xff);
	}

	return -1;
}

/*--------------------------------------------------------------
 * ROUTINE NAME - buart_write
 *---------------------------------------------------------------
 * FUNCTION: This function writes one byte to the uart channel
 *           if the chanel is ready or return -1 or the channel
 *           is busy.
 *
 * INPUT:    out_byte    -- byte to send
 *
 * OUTPUT:
 * RETURN:	0   -- OK
 *          -1   -- BUSY
 *--------------------------------------------------------------*/
static int buart_write (char c)
{
	UINT32 uartfr;

	uartfr = UART_REG_READ (&uart, UART_FR_REG);

	if (!(uartfr & UART_TX_FIFO_EMPTY)) return -1;

	UART_REG_WRITE (&uart, UART_DR_REG, (UINT32) c);

	return 0;
}

/*----------------------------------------------------------------------
 * ROUTINE NAME - buart_print
 *-----------------------------------------------------------------------
 * DESCRIPTION: This routine outs a string to the io port
 *
 * INPUT:    *ptr    -- string to print
 *
 *----------------------------------------------------------------------*/
void buart_print (const char *str)
{
	for (; *str; str++)
	{
		while (buart_write (*str) == -1)
			;
	}

	return;
}

/*----------------------------------------------------------------------
 * ROUTINE NAME - buart_put
 *-----------------------------------------------------------------------
 * DESCRIPTION: This routine out a char to the io port and confirm OK
 *
 * INPUT:    out_char    -- character to print
 *
 *----------------------------------------------------------------------*/
void buart_put (char c)
{
	while (buart_write (c) == -1)
		;

	return;
}

/*----------------------------------------------------------------------
 * ROUTINE NAME - buart_get
 *-----------------------------------------------------------------------
 * DESCRIPTION:  This routine receives a byte from the console with a
 *               timeout value specified.
 *
 * INPUT:    timeout -- timeout in second
 * RETURN:   byte received or
 *           -1 if timed out
 *----------------------------------------------------------------------*/
int buart_get (int timeout)
{
	register int c;
	register unsigned long ticks;

	// If don't wait, read the channel directly
	if (timeout == 0)
		return buart_read ();

	// time-out set, block read
	ticks = UpTime ();
	while ((int) (UpTime () - ticks) < (timeout * 100))
	{
		if ((c = buart_read ()) != -1)
			return c;
	}

	return -1;
}

char buart_getchar (void)
{
	register int c;
	while ((c = buart_get (0)) == -1)
		;
	return (char) c;
}

/*----------------------------------------------------------------------
 * ROUTINE NAME - ReadLine
 *-----------------------------------------------------------------------
 * DESCRIPTION:
 *
 *----------------------------------------------------------------------*/
void ReadLine (char *buf, int num)
{
	int i, userKey;
	char readBuf[32] = {0};
	int cpNum;
	int esc_cnt = 0;

	if (buf == NULL || num > 256)
	{
		buart_print ("\n***** WARNING *****\nFunction Parameters Error!\n");
		return;
	}

	for (i = 0; i < 256; i++)
	{
GETKEY:
		userKey = buart_getchar ();

		if (esc_cnt)
		{
			esc_cnt--;
			goto GETKEY;
		}

		if (userKey == 0x0d || userKey == 0x0a)
		{
			readBuf[i] = 0;
			buart_print ("\r\n");
			break;
		} else if (userKey == 0x1b)
		{
			esc_cnt = 2;
			goto GETKEY;
		} else if (userKey == 0x08)
		{
			if (--i < 0)
			{
				i = 0;
				goto GETKEY;
			}
			buart_print ("\b \b");
			readBuf[i] = 0;
			--i;
		} else
		{
			buart_put (userKey);
			//			printf("%c", (char)userKey);
			readBuf[i] = (char) userKey;
		}

	}

	if (i < 0)
	{
		*buf = 0;
		return;
	} else if (++i <= num)
		cpNum = i;
	else
	{
		cpNum = num;

		// truncate the input string.
		buf[num - 1] = 0;
	}

	strncpy (buf, readBuf, cpNum);
}


