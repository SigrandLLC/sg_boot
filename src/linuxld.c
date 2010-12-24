/*****************************************************************************
;
;    Project : Common plateform
;    Creator : Sumei Chung
;    File    : loader.C
;    Abstract:
;
;*****************************************************************************/
#include <ctype.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <hw_profile.h>
#include <bconfig.h>
#include <nand.h>
#include <nf.h>
#include <img_header.h>
#include <xmodem.h>
#include <uartdrv.h>
#include <stdlib.h>
#include <zlib.h>
#include <linuxld.h>
#include <memlib.h>
#include <tftp.h>
#include <timer.h>

/* linux boot defined */
#define LINUX_ENTRY_POINT		LINUXLD_KERNEL_START - 0xa0000000 + 0x800006d8

#define LINUX_IMAGE_SIZE		LINUXLD_DOWNLOAD_SIZE

/* gzip defined */
#define _OFFSET(_t, _m)		((u_int) &(((_t *) 0)->_m))
#define Z_EOF	0


/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */


static int gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */
char *pass = "PASS";
char *fail = "FAIL";

int ungzip (Byte * compr);
extern void _icache_sync_all (void);
void jump_up_map (void);
void jump_low_map (void);

void flash_erase_all (void)
{
	buart_print ("\n\rEraseing flash.......\n\r");
	nand_erase ((UINT8 *) LINUXLD_NANDFLASH_LOADER_START, LINUXLD_NANDFLASH_LOADER_SIZE - 1, 1);
	nand_erase ((UINT8 *) LINUXLD_NANDFLASH_KERNEL_START, NAND_FLASH_SIZE - LINUXLD_NANDFLASH_KERNEL_START, 1);
}

void check_for_bad (void)
{
	buart_print ("\n\rChecking flash.......\n\r");
	check_bad_block ();
}

void find_bad_blocks (void)
{
	buart_print ("\n\rScaning flash.......\n\r");
	scan_bad_blocks ();
}

void create_bad_blocks (void)
{
	buart_print ("\n\rCreating bad blocks.......\n\r");
	bad_block ();
}

#ifndef NO_XMODEM
int update_bootloader (void)
{
	void *flash = (void *) LINUXLD_NANDFLASH_LOADER_START;
	char *image = (char *) LINUXLD_DOWNLOAD_START;
	int len, rc = 0;

	/* download loader image to temp using xmodem */
	buart_print ("\n\rDownloading..........");
	if ((len = xmodem (image, LINUXLD_NANDFLASH_LOADER_SIZE)) == -1)
		goto fail;
	else
		buart_print (pass);

	/* erase flash */
	buart_print ("\n\rEraseing flash.......");
	if (nf_erase (flash, len, 0) < 0)
		goto fail;
	else
		buart_print (pass);

	/* write flash */
	buart_print ("\n\rProgramming flash....");
	if (nf_write_boot ((UINT8 *) flash, image, len) < 0)
		goto fail;
	else
		buart_print (pass);

	goto ok;

fail:
	buart_print (fail);
	rc = -1;

ok:
	return (rc);
}
#endif

int tftpc_download (int mode)
{
	void *flash;
	char *image/*, lenstr[9]*/;
	UINT32 len, image_size;

	switch (mode)
	{
		case TFTP_LOAD_BOOTLOADER:
			flash = (void *) LINUXLD_NANDFLASH_LOADER_START;
			image = (char *) LINUXLD_DOWNLOAD_START;
			image_size = LINUXLD_LOADER_SIZE + LINUXLD_RESERVE_SIZE;
			break;
		case TFTP_LOAD_LINUX:
			flash = (void *) LINUXLD_NANDFLASH_KERNEL_START;
			image = (char *) LINUXLD_DOWNLOAD_START;
			image_size = LINUX_IMAGE_SIZE;
			break;
		default:
			buart_print ("\n\rUnknown binary download mode");
			return -1;
	}

	buart_print ("\n\rStarting the TFTP download (ESC to stop)..");
	if ((len = tftpc (image, image_size, mode)) == 0)
	{
		buart_print (fail);
		return -1;
	}
	buart_print (pass);

	//print image length and starting address in sdram.
	/*ultoa (len, lenstr);
	lenstr[8] = 0;
	buart_print ("\n\rImage Total Length: ");
	buart_print (lenstr);
	ultoa (image, lenstr);
	buart_print ("  Starting Address: ");
	buart_print (lenstr);*/

	//erase flash
	buart_print ("\n\r\n\rEraseing flash.......");


	if (nf_erase (flash, (mode == TFTP_LOAD_LINUX) ?
		(NAND_FLASH_SIZE - LINUXLD_NANDFLASH_KERNEL_START) : len, 0) < 0)
	{
		buart_print (fail);
		return -1;
	}

	buart_print (pass);

	//write flash
	buart_print ("\n\rProgramming flash....");

	if (mode == TFTP_LOAD_BOOTLOADER)
	{
		if (nf_write_boot (flash, image, len) < 0)
		{
			buart_print (fail);
			return -1;
		}
	} else
		if (nf_write (flash, image, len, 0) < 0)
	{
		buart_print (fail);
		return -1;
	}

	buart_print (pass);
	return 0;
}


#ifndef NO_XMODEM
// download linux image to flash using xmodem
int xmodem_download (void)
{
	void *flash = (void *) LINUXLD_NANDFLASH_KERNEL_START;
	char *image = (char *) LINUXLD_DOWNLOAD_START;
	int len, rc = 0;

	/* download linux image to temp using xmodem */
	buart_print ("\n\rDownloading..........");
	if ((len = xmodem (image, LINUX_IMAGE_SIZE)) == -1)
		goto fail;
	else
		buart_print (pass);

	/* erase flash */
	buart_print ("\n\rEraseing flash.......");

	if (nf_erase (flash, len, 0) < 0)
		goto fail;

	buart_print (pass);

	/* write flash */
	buart_print ("\n\rProgramming flash....");

	if (nf_write (flash, image, len, 0) < 0)
		goto fail;

	buart_print (pass);

	return (rc);

fail:
	buart_print (fail);
	return -1;
}
#endif

// linux boot function

void boot_linux (void)
{
	int status;
	void (*funcptr)(void);
	buart_print ("\n\rReading Linux... ");
	status = nf_read ((UINT8 *) LINUXLD_DOWNLOAD_START,
			  (UINT8 *) LINUXLD_NANDFLASH_KERNEL_START,
			  LINUX_IMAGE_SIZE);
	if (status < 0)
	{
		buart_print (fail);
		return;
	}
	buart_print (pass);

	// decompressing
	buart_print ("\n\rDecompress Linux... ");
	status = ungzip ((unsigned char *) LINUXLD_DOWNLOAD_START);

	if (status != Z_OK) // failed in unzipping
	{
		buart_print (fail);
		return; // return if cannot boot
	} else
		buart_print (pass);
	DisableTimer ();

	/*char buf[10] = {0}, *kbuf = (char *) LINUXLD_KERNEL_START;
	int x;
	for (x = 0; x < 0x1000; x++)
	{
		if (x % 16 == 0) buart_print ("\n\r");
		btoa (kbuf[x], buf);
		buart_print (buf);
		buart_print (" ");
	}
	buart_print ("\n\rLoading linux from 0x");
	ultoa (LINUX_ENTRY_POINT, buf);
	buart_print (buf);*/

	funcptr = (void *) LINUX_ENTRY_POINT;
	_icache_sync_all ();

	buart_print ("\n\rBooting Linux... ");
	buart_print ("\n\r\n\r");

	funcptr ();
}

// decompress function

int ungzip (unsigned char *zimg)
{
	int i, err, gzflags;
	z_stream d_stream; // decompression stream

	if ((zimg[0] != gz_magic[0]) || (zimg[1] != gz_magic[1]))
		return (Z_DATA_ERROR);

	if (zimg[2] != Z_DEFLATED) return (Z_DATA_ERROR);

	gzflags = zimg[3];

	// Skip the gzip header
	zimg += 10;

	if ((gzflags & EXTRA_FIELD) != 0)
	{ /* skip the extra field */
		i =  (*zimg++);
		i += (*zimg++) << 8;
		while ((i-- != 0) && (*zimg++) != Z_EOF);
	}

	if ((gzflags & ORIG_NAME) != 0)
	{ /* skip the original file name */
		while (*zimg++ != Z_EOF);
	}

	if ((gzflags & COMMENT) != 0)
	{ /* skip the .gz file comment */
		while (*zimg++ != Z_EOF);
	}

	if ((gzflags & HEAD_CRC) != 0)
	{ /* skip the header crc */
		zimg += 2;
	}

	/* Decompress the image now */
	d_stream.zalloc = zAlloc;
	d_stream.zfree = zFree;
	d_stream.opaque = (voidpf) 0;

	d_stream.next_in = zimg; // address of compress image
	d_stream.avail_in = LINUX_IMAGE_SIZE; // size of compress image

	// buart_print ("Entering inflateInit2... ");
	if ((err = inflateInit2 (&d_stream, -MAX_WBITS)) != Z_OK)
	{
		return err;
	}

	d_stream.next_out = (char *) LINUXLD_KERNEL_START; // address of decompress image
	d_stream.avail_out = LINUXLD_KERNEL_SIZE; // size of decompress space
	// buart_print ("Entering inflate... ");
	if ((err = inflate (&d_stream, Z_NO_FLUSH)) != Z_STREAM_END)
	{
		return err;
	}

	// buart_print ("Entering inflateEnd... ");
	err = inflateEnd (&d_stream);

	return err;
}
