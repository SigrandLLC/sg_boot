#if __cplusplus
extern "C"
{
#endif

#ifndef _XMODEM_H_
#define _XMODEM_H

#define XMODEM_SIZE             128


	/* ASCII characters */
#define NAK                     025     /* no acnowledge */
#define SOH                     001     /* start of header */
#define EOT                     004     /* end of transmission */
#define ACK                     006     /* acnowledge */
#define CAN                     030     /* cancel */
#define LF                      0xa

#define ERRORMAX                15     /* max errors tolerated */
#define FILE_CORRUPT            0x80


	int xmodem (char *buf, int buf_size);


#endif /* _XMODEM_H */

#if __cplusplus
}
#endif
