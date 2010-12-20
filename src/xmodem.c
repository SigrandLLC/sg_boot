/*****************************************************************************
;
;    Project : Common plateform
;    Creator : Sumei Chung
;    File    : xmodem.C
;    Abstract:
;
;*****************************************************************************/
#include "xmodem.h"

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

extern void buart_put (char);
extern int buart_get (int);

/*--------------------------------------------------------------
 * ROUTINE NAME - read_packet
 *---------------------------------------------------------------
 * FUNCTION: Get XMODEM_SIZE bytes from uart.  Timeout if 3
 *           seconds elapse
 *
 * INPUT:    char *buf   -- buffer to read the packet
 * OUTPUT:
 * RETURN:	0   -- OK
 *          -1   -- error
 *--------------------------------------------------------------*/
static int read_packet (char *buf)
{
	register int i; /* Number of characters read */
	register int c; /* Next character read */
	register unsigned char chksum; /* accumulate checksum here */

	chksum = 0;
	for (i = 0; i < XMODEM_SIZE; i++)
	{
		if ((c = buart_get (3)) == -1)
		{
			return -1;
		} else
		{
			buf[i] = c;
			chksum += c;
		}
	}

	/* Read and confirm checksum */
	if ((c = buart_get (3)) == -1)
	{
		return -1;
	} else if ((c & 0xff) != chksum)
	{
		/* chksum error, abort operation */
		return -1;
	}

	return 0;
}

/*--------------------------------------------------------------
 * ROUTINE NAME - xmodem
 *---------------------------------------------------------------
 * FUNCTION: Get XMODEM_SIZE bytes from uart.  Timeout if 3
 *           seconds elapse
 *
 * INPUT:    char *buf   -- buffer to read the packet
 * OUTPUT:
 * RETURN:	0   -- OK
 *          -1   -- error
 *--------------------------------------------------------------*/
int xmodem (char *buf, int buf_size)
{
	register int c = 0; /* 1st byte of packet */
	register int errors; /* Running count of errors */
	register int errorflag; /* True when error found while reading a packet    */
	register int fatalerror; /* True when fatal error found while reading packet */
	register unsigned char pktnum; /* 2nd byte of packet: packet # (mod 128) */
	register unsigned char pktchk; /* 3rd byte of packet: should be complement ofpktnum*/
	register int pktcnt; /* total # of packets received so far */
	register int initial_tries;

	register char *buf_end = buf + buf_size;
	register char *working = buf;


	fatalerror = FALSE;
	errors = pktcnt = 0;

	/* start communications by providing NAK to the sending
	 * end.  The number and time over which these are provided
	 * is 10 seconds at once per second, followed by about
	 * one minute at 4 second intervals.  the maximum tolerable
	 * inter-character time is 3 seconds, and if that occurs,
	 * we will wait for 1 second of silence from the host and
	 * then NAK the packet.
	 */

	initial_tries = 10;
	do
	{
		buart_put (NAK);
	} while (--initial_tries && ((c = buart_get (1)) == -1));

	goto PreLoaded;

	/* MAIN Do-While loop to read packets */
	do
	{
		/* start by read first byte in packet */
		c = buart_get (3);

PreLoaded:
		errorflag = FALSE;

		switch (c)
		{
			case -1:
				errorflag = TRUE;
				break;

			case ACK: /* transmitter confirms reception */
				if (pktcnt > 0)
					continue;
				break;

			case EOT: /* transmitter signals finish */
				if ((c = buart_get (1)) == -1)
				{
					c = EOT;
				} else if (c != EOT)
				{
					/* EOT from data portion of a (delayed) packet */
					c = -1;
					errorflag = TRUE;
				}
				break;

			case CAN: /* bailing out? */
				if ((buart_get (3) & 0x7f) != CAN)
					errorflag = TRUE;
				else
					fatalerror = TRUE;
				break;

			case SOH: /* start reading packet */
				pktnum = buart_get (3);
				pktchk = buart_get (3);

				if (pktnum != ((unsigned char) ~pktchk))
				{
					/* MISREAD PACKET # */
					errorflag = TRUE;
					break;
				}

				if (pktnum == (pktcnt & 0xff))
				{
					/* DUPLICATE PACKET -- DISCARD */
					while (buart_get (3) != -1)
						;

					buart_put (ACK);
					break;
				}

				if (pktnum != ((pktcnt + 1) & 0xff))
				{
					/* PHASE ERROR */
					fatalerror = TRUE;
					break;
				}

				/* Read the data 128 byte packet and calculate checksum */
				/* for a packet of data */
				if (read_packet (working) != 0)
				{
					errorflag = TRUE;
					break;
				}

				/* increment the memory pointer  */
				if (working >= buf_end)
				{
					fatalerror = TRUE;
					break;
				}

				working += XMODEM_SIZE;
				errors = 0;
				pktcnt++;

				/* ACK the packet */
				buart_put (ACK);
				break;

			default:
				continue;
		}
		/* end of switch */

		/* check on errors or batch transfers */
		if (errorflag || pktcnt == 0)
		{
			if (errorflag)
			{
				if (++errors >= ERRORMAX)
					fatalerror = TRUE;
			}

			if (!fatalerror) /* wait for line to settle */
			{
				while (buart_get (1) != -1)
					;

				buart_put (NAK);
			}
		}
	} while (c != EOT && !fatalerror);


	/*************************
	  End of transmission
	 **************************/
	if (c == EOT)
	{
		buart_put (ACK);
	} else /* error exit */
	{
		if (pktcnt != 0)
		{
			/* Cancel download by sending two CAN to remote */
			buart_put (CAN);
			buart_put (CAN);
		}
	}

	/********************************
	  Now wait for line settle down
	 *********************************/
	while (buart_get (2) != -1)
		;

	if (fatalerror)
		return -1;

	return (pktcnt * XMODEM_SIZE);
}
