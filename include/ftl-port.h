/*
 *	ftl-port.h -- defines to support the NAND Flash Translation Layer
 *
 *	(C) Copyright 2003, Pro Chao (proc@admtek.com.tw)
 *
 *	$Id: ftl-port.h,v 1.1.1.1 2003/10/8 10:22:53 pro Exp $
 */

#ifndef __MTD_FTL_H__
#define __MTD_FTL_H__

// ported from Linux platform
typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;
typedef unsigned long __u64;

typedef unsigned char u_char;
typedef long loff_t;
typedef unsigned int size_t;

#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

//

struct nand_oobinfo
{
	int useecc;
	int eccpos[6];
};

//
#define	SECTORSIZE      512

#define FTL_PARTN_BITS  4

//
#define NAND_CTL_SETNCE 	1
#define NAND_CTL_CLRNCE		2
#define NAND_CTL_SETCLE		3
#define NAND_CTL_CLRCLE		4
#define NAND_CTL_SETALE		5
#define NAND_CTL_CLRALE		6
#define NAND_CTL_SETWP		7
#define NAND_CTL_CLRWP		8

// control signal
//#define NAND_RW_REG		0x0	//data register
#define NAND_SET_CEn		0x1	//CE# low
#define NAND_CLR_CEn		0x2	//CE# high
#define NAND_CLR_CLE		0x3	//CLE low
#define NAND_SET_CLE		0x4	//CLE high
#define NAND_CLR_ALE		0x5	//ALE low
#define NAND_SET_ALE		0x6	//ALE high
#define NAND_SET_SPn		0x7	//SP# low (use spare area)
#define NAND_CLR_SPn		0x8	//SP# high (do not use spare area)
#define NAND_SET_WPn		0x9	//WP# low
#define NAND_CLR_WPn		0xA	//WP# high
#define NAND_STS_REG		0xB	//Status register

//
#define NAND_ECC_NONE		0
#define NAND_ECC_SOFT		1
#define NAND_ECC_HW3_256	2
#define NAND_ECC_HW3_512	3
#define NAND_ECC_HW6_512	4
#define NAND_ECC_DISKONCHIP	5

// page-shift
#define	NAND_PAGE_SHIFT		9		//512 bytes

//#define NULL	((void *) 0)

/* These error numbers are intended to be MIPS ABI compatible */
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO			5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Arg list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */
#define	ENOMSG		35	/* No message of desired type */
#define	EIDRM		36	/* Identifier removed */
#define	ECHRNG		37	/* Channel number out of range */
#define	EL2NSYNC	38	/* Level 2 not synchronized */
#define	EL3HLT		39	/* Level 3 halted */
#define	EL3RST		40	/* Level 3 reset */
#define	ELNRNG		41	/* Link number out of range */
#define	EUNATCH		42	/* Protocol driver not attached */
#define	ENOCSI		43	/* No CSI structure available */
#define	EL2HLT		44	/* Level 2 halted */
#define	EDEADLK		45	/* Resource deadlock would occur */
#define	ENOLCK		46	/* No record locks available */
#define	EBADE		50	/* Invalid exchange */
#define	EBADR		51	/* Invalid request descriptor */
#define	EXFULL		52	/* Exchange full */
#define	ENOANO		53	/* No anode */
#define	EBADRQC		54	/* Invalid request code */
#define	EBADSLT		55	/* Invalid slot */
#define	EDEADLOCK	56	/* File locking deadlock error */
#define	EBFONT		59	/* Bad font file format */
#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */
#define	EDOTDOT		73	/* RFS specific error */
#define	EMULTIHOP	74	/* Multihop attempted */
#define	EBADMSG		77	/* Not a data message */
#define	ENAMETOOLONG	78	/* File name too long */
#define	EOVERFLOW	79	/* Value too large for defined data type */
#define	ENOTUNIQ	80	/* Name not unique on network */
#define	EBADFD		81	/* File descriptor in bad state */
#define	EREMCHG		82	/* Remote address changed */
#define	ELIBACC		83	/* Can not access a needed shared library */
#define	ELIBBAD		84	/* Accessing a corrupted shared library */
#define	ELIBSCN		85	/* .lib section in a.out corrupted */
#define	ELIBMAX		86	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	87	/* Cannot exec a shared library directly */
#define	EILSEQ		88	/* Illegal byte sequence */
#define	ENOSYS		89	/* Function not implemented */
#define	ELOOP		90	/* Too many symbolic links encountered */
#define	ERESTART	91	/* Interrupted system call should be restarted */
#define	ESTRPIPE	92	/* Streams pipe error */
#define	ENOTEMPTY	93	/* Directory not empty */
#define	EUSERS		94	/* Too many users */
#define	ENOTSOCK	95	/* Socket operation on non-socket */
#define	EDESTADDRREQ	96	/* Destination address required */
#define	EMSGSIZE	97	/* Message too long */
#define	EPROTOTYPE	98	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	99	/* Protocol not available */
#define	EPROTONOSUPPORT	120	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	121	/* Socket type not supported */
#define	EOPNOTSUPP	122	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	123	/* Protocol family not supported */
#define	EAFNOSUPPORT	124	/* Address family not supported by protocol */
#define	EADDRINUSE	125	/* Address already in use */
#define	EADDRNOTAVAIL	126	/* Cannot assign requested address */
#define	ENETDOWN	127	/* Network is down */
#define	ENETUNREACH	128	/* Network is unreachable */
#define	ENETRESET	129	/* Network dropped connection because of reset */
#define	ECONNABORTED	130	/* Software caused connection abort */
#define	ECONNRESET	131	/* Connection reset by peer */
#define	ENOBUFS		132	/* No buffer space available */
#define	EISCONN		133	/* Transport endpoint is already connected */
#define	ENOTCONN	134	/* Transport endpoint is not connected */
#define	EUCLEAN		135	/* Structure needs cleaning */
#define	ENOTNAM		137	/* Not a XENIX named type file */
#define	ENAVAIL		138	/* No XENIX semaphores available */
#define	EISNAM		139	/* Is a named type file */
#define	EREMOTEIO	140	/* Remote I/O error */
#define EINIT		141	/* Reserved */
#define EREMDEV		142	/* Error 142 */
#define	ESHUTDOWN	143	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	144	/* Too many references: cannot splice */
#define	ETIMEDOUT	145	/* Connection timed out */
#define	ECONNREFUSED	146	/* Connection refused */
#define	EHOSTDOWN	147	/* Host is down */
#define	EHOSTUNREACH	148	/* No route to host */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	EALREADY	149	/* Operation already in progress */
#define	EINPROGRESS	150	/* Operation now in progress */
#define	ESTALE		151	/* Stale NFS file handle */
#define ECANCELED	158	/* AIO operation canceled */

/*
 * These error are Linux extensions.
 */
#define ENOMEDIUM	159	/* No medium found */
#define EMEDIUMTYPE	160	/* Wrong medium type */

#define EDQUOT		1133	/* Quota exceeded */

/* Block Control Information */
#define	SECTOR_FREE		0xff		// macros for Status & Status1 fields
#define	SECTOR_USED		0x55
#define	SECTOR_IGNORED	0x11
#define	SECTOR_DELETED	0x00

#define ERASE_MARK 0x3c69

struct ftl_bci
{
	__u8 ECCsig[6];
	__u8 Status;
	__u8 Status1;
} __attribute__ ((packed));

struct ftl_unithead1
{
	__u16 virtualUnitNo;
	__u16 prevUnitNo;
	__u8 ANAC;
	__u8 NACs;
	__u8 parityPerField;
	__u8 discarded;
} __attribute__ ((packed));

struct ftl_unithead2
{
	__u8 parityPerField;
	__u8 ANAC;
	__u16 prevUnitNo;
	__u16 virtualUnitNo;
	__u8 NACs;
	__u8 discarded;
} __attribute__ ((packed));

struct ftl_unittail
{
	__u8 Reserved[4];
	__u16 EraseMark;
	__u16 EraseMark1;
} __attribute__ ((packed));

union ftl_uci
{
	struct ftl_unithead1 a;
	struct ftl_unithead2 b;
	struct ftl_unittail c;
};

struct ftl_oob
{
	struct ftl_bci b;
	union ftl_uci u;
};


#endif /* __MTD_FTL_H__ */

