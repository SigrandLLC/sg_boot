#ifndef __TFTP_H
#define	__TFTP_H

#include <ctype.h> // UINT32 tftpc (...

/*
 * Trivial File Transfer Protocol (IEN-133)
 */
#define	TFTP_DATA_SIZE		512		/* data segment size */

/*
 * Packet types.
 */
#define	TFTP_RRQ	01				/* read request */
#define	TFTP_WRQ	02				/* write request */
#define	TFTP_DATA	03				/* data packet */
#define	TFTP_ACK	04				/* acknowledgement */
#define	TFTP_ERROR	05				/* error code */

/*
 * TFTP state
 */
#define	TFTP_END		0
#define	TFTP_START		1
#define	TFTP_CONTINUE	2
#define	TFTP_NO_DATA	-1

/*
 * TFTP header
 */
struct tftphdr
{
	short th_opcode; /* packet type */

	union
	{
		unsigned short tu_block; /* block # */
		short tu_code; /* error code */
		char tu_stuff[1]; /* request packet stuff */
	} __attribute__ ((__packed__)) th_u;
	char th_data[0]; /* data or error string */
} __attribute__ ((__packed__));

#define	th_block	th_u.tu_block
#define	th_code		th_u.tu_code
#define	th_stuff	th_u.tu_stuff
#define	th_msg		th_data

/*
 * Error codes.
 */
#define	EUNDEF		0		/* not defined */
#define	ENOTFOUND	1		/* file not found */
#define	EACCESS		2		/* access violation */
#define	ENOSPACE	3		/* disk full or allocation exceeded */
#define	EBADOP		4		/* illegal TFTP operation */
#define	EBADID		5		/* unknown transfer ID */
#define	EEXISTS		6		/* file already exists */
#define	ENOUSER		7		/* no such user */

/*
 * Function prototypes
 */
void   tftp_init (void);
UINT32 tftpc (char *buf, int buf_size, int mode);


#endif /* __TFTP_H */

