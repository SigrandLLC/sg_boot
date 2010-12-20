/*****************************************************************************
;
;    Project : AM5120
;    Creator : Jeanson Hung
;    File    : nand.c
;    Abstract:
;
;Modification History:
;	10/8/2003,	ProChao		- support the bootloader to be compatible with the
;							  NAND flash driver in application (Linux kernel)
;
;*****************************************************************************/
#include <ctype.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <linuxld.h>
#include "nand.h"
#include "ftl-port.h"	//porting by ProChao, 10/8/2003

#define NAND_ECC

// чтение и запись - строго по границам страниц, ибо нефиг.

//---------------------------------------------------------
volatile UINT8 *base = (UINT8 *) NAND_REG_BASE;

/*static struct nand_oobinfo oobinfo_buf = {// init'ed by ProChao
	1, // структура содержит информацию о том, используем ли мы
	{0, 1, 2, 3, 4, 5} // есс, и в каких по счёту байтах находится информация
}; // о есс. Инфа о статусе сектора - в секторных байтах (5, 6)*/

static struct nand_oobinfo oobinfo_buf = {// init'ed by ProChao
	1,
	{0, 1, 2, 3, 6, 7}
};

static u_char oobdata_buf[NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE];
static u_char oobdata_buf2[NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE];

// adds the verifying block that can be used before reading/writing(erasing) block

static inline void print_b (UINT8 b)
{
	char ostr[4] = "   \0";
	btoa (b, ostr);
	buart_print (ostr);
}

static inline void print_val (char *str, int val)
{
	char ostr[9] = {0};
	buart_print ("\n\r");
	buart_print (str);
	ultoa (val, ostr);
	buart_print (" = ");
	buart_print (ostr);
}

static inline void print_val1 (char *str, int val)
{
	char ostr[9] = {0};
	buart_print ("\r");
	buart_print (str);
	ultoa (val, ostr);
	buart_print (" = ");
	buart_print (ostr);
}

static void nand_hwcontrol (int cmd)
{
	switch (cmd)
	{
		case NAND_CTL_SETCLE:
			*(base + NAND_SET_CLE) = 0x01;
			break;
		case NAND_CTL_CLRCLE:
			*(base + NAND_CLR_CLE) = 0x01;
			break;
		case NAND_CTL_SETALE:
			*(base + NAND_SET_ALE) = 0x01;
			break;
		case NAND_CTL_CLRALE:
			*(base + NAND_CLR_ALE) = 0x01;
			break;
		case NAND_CTL_SETNCE:
			*(base + NAND_SET_CEn) = 0x01;
			break;
		case NAND_CTL_CLRNCE:
			*(base + NAND_CLR_CEn) = 0x01;
		default:
			break;
	}
}

static UINT8 nand_read_byte (void)
{
	return *base; // readb( base);
}

static void nand_write_byte (UINT8 byte)
{
	*base = byte;
	//writeb( byte, base);
}

static void nand_select_chip (int chip)
{
	switch (chip)
	{
		case -1:
			nand_hwcontrol (NAND_CTL_CLRNCE);
			break;
		case 0:
			nand_hwcontrol (NAND_CTL_SETNCE);
			break;
		default: // bugs, should not be here
			break;
	}
}

// send command to the NAND device

static void nand_command (unsigned command, int column, int page_addr)
{
	UINT32 j;
	int readcmd;

	/* Begin command latch cycle */
	nand_hwcontrol (NAND_CTL_SETCLE);
	/* Write out the command to the device. */
	if (command == NAND_CMD_SEQIN)
	{
		if (column >= NAND_PAGE_SIZE)
		{ /* OOB area */
			column -= NAND_PAGE_SIZE;
			readcmd = NAND_CMD_READOOB;
		} else if (column < 256)
		{ /* First 256 bytes --> READ0 */
			readcmd = NAND_CMD_READ0;
		} else
		{ // 2nd 256 bytes
			column -= 256;
			readcmd = NAND_CMD_READ1;
		}
		nand_write_byte (readcmd);
	}
	nand_write_byte (command);

	/* Set ALE and clear CLE to start address cycle */
	nand_hwcontrol (NAND_CTL_CLRCLE);

	if (column != -1 || page_addr != -1)
	{
		nand_hwcontrol (NAND_CTL_SETALE);
		/* Serially input address */
		if (column != -1)
			nand_write_byte (column); // дальше 256 байта не прочитает =)
		if (page_addr != -1)
		{
			nand_write_byte ((unsigned char) (page_addr & 0xff));
			nand_write_byte ((unsigned char) ((page_addr >> 8) & 0xff));
			/* One more address cycle for higher density devices */
			if (NAND_FLASH_SIZE & 0x0c000000) // 32MB by ProChao, 10/8/2003
				nand_write_byte ((unsigned char) ((page_addr >> 16) & 0x0f));
		}
		/* Latch in address */
		nand_hwcontrol (NAND_CTL_CLRALE);
	}

	/* program and erase have their own busy handlers
	 * status and sequential in needs no delay
	 */
	switch (command)
	{
		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE1:
		case NAND_CMD_ERASE2:
		case NAND_CMD_SEQIN:
		case NAND_CMD_STATUS:
			return;

		case NAND_CMD_RESET:
			nand_hwcontrol (NAND_CTL_SETCLE);
			nand_write_byte (NAND_CMD_STATUS);
			nand_hwcontrol (NAND_CTL_CLRCLE);
			while (!(nand_read_byte () & 0x40));
			return;

			/* This applies to read commands */
		default: /* If we don't have access to the busy pin, we apply the given
					 * command delay */
			// wait 1us */
			for (j = 0; j < 1000; j++);
	}
}

// при выбранном кристалле читает одну страницу в буфер
// при oob == 1 читает ещё и OOB

void nand_read_page_oob (UINT8 *src, UINT8 *buf, UINT8 oob)
{
	UINT32 col, row1, row2, i;

	*(base + NAND_CLR_SPn_REG) = 1; // читаем основную часть блока

	col = (int) src & ADD1MASK;
	row1 = ((int) src & ADD2MASK) >> ADD2SHIFT;
	row2 = ((int) src & ADD3MASK) >> ADD3SHIFT;

	*(base + NAND_SET_CLE_REG) = 1;
	*(base + NAND_RW_REG) = 0;
	*(base + NAND_CLR_CLE_REG) = 1;
	*(base + NAND_SET_ALE_REG) = 1;

	*(base + NAND_RW_REG) = col;
	*(base + NAND_RW_REG) = row1;
	*(base + NAND_RW_REG) = row2;

	*(base + NAND_CLR_ALE_REG) = 1;

	// wait 1us
	for (i = 0; i < 1000; i++)
	{
		;
		;
	}

	for (i = 0; i < NAND_PAGE_SIZE; i++)
		buf[i] = *(base + NAND_RW_REG);

	if (oob)
	{
		*(base + NAND_SET_SPn_REG) = 1; // читаем OOB

		for (i = 0; i < NAND_PAGE_OOB_SIZE; i++)
			buf[i + NAND_PAGE_SIZE] = *(base + NAND_RW_REG);
	}
}

// при выбранном кристалле пишет одну страницу в буфер
// при oob == 1 пишет ещё и OOB

void nand_write_page_oob (UINT8 *dst, UINT8 *buf, UINT8 oob, UINT8 wp)
{
	UINT32 i, col, row1, row2;

	//if (wp) // снимаем write protection
		*(base + NAND_SET_WP_REG) = 1;
	// buart_print ("\n\rnand_write_page_oob - WP");

	*(base + NAND_CLR_SPn_REG) = 1;

	col = (int) dst & ADD1MASK;
	row1 = ((int) dst & ADD2MASK) >> ADD2SHIFT;
	row2 = ((int) dst & ADD3MASK) >> ADD3SHIFT;

	*(base + NAND_SET_CLE_REG) = 1;
	*(base + NAND_RW_REG) = CMD_WRITE1;
	*(base + NAND_CLR_CLE_REG) = 1;
	*(base + NAND_SET_ALE_REG) = 1;

	*(base + NAND_RW_REG) = col;
	*(base + NAND_RW_REG) = row1;
	*(base + NAND_RW_REG) = row2;

	*(base + NAND_CLR_ALE_REG) = 1;

	for (i = 0; i < NAND_PAGE_SIZE; i++)
		*(base + NAND_RW_REG) = buf[i];

	// enable spare area
	if (oob)
	{
		*(base + NAND_SET_SPn_REG) = 1;

		for (i = 0; i < NAND_PAGE_OOB_SIZE; i++)
			*(base + NAND_RW_REG) = buf[i + NAND_PAGE_SIZE];
	}

	*(base + NAND_SET_CLE_REG) = 1;
	*(base + NAND_RW_REG) = CMD_WRITE2;
	*(base + NAND_CLR_CLE_REG) = 1;
	/* wait 200 us*/
	for (i = 0; i < 20000; i++)
	{
		;
		;
	}

	/*if (wp) // ставим write protection обратно
		*(base + NAND_CLR_WP_REG) = 1;*/
}

/* Appropriate chip should already be selected */
static int nand_block_bad (unsigned long page)
{
	UINT8 status, status1;
	if (page < NAND_BLK_PER_PAGE) // весь загрузчик лежит в 256 страницах.
		return 0; // В первых страницах OOB используется для данных, этот

	// если хочется посмотреть как система реагирует на бедблоки, следует расскомментить это
	/*if ((page == NAND_BLK_PER_PAGE * 100) || (page == NAND_BLK_PER_PAGE * 150) || (page == NAND_BLK_PER_PAGE * 600))
	{
		nand_mark_bad_block (page);
		return 1;
	}*/

	// факт следует учесть, иначе первый сектор станет ББ.
	// впрочем, загрузчик пока что не умеет обрабатывать ошибки
	// страниц, но на случай реализации этого дела..
	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;
	nand_read_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 1);
	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;

	status = oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS];
	status1 = oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS + 1];
	if (status != 0xff)
	{ // might be programmed alrady with ECC info, so checking other status info
		if (status != status1) // these 2 octets should be programmed same
			return 1; // а если нет, значит есть косячный бит в OOB
		// this block is treated as the bad one
		switch (status)
		{
			case SECTOR_FREE: // available for use
			case SECTOR_USED: // of course, for being used
				return 0;
			case SECTOR_IGNORED:
			case SECTOR_DELETED:
			default:
				return 1; // be treated as not-usable
		}
	}
	return 0;
}

/*
 *	Nand_page_program function is used for write and writev !
 *	This function will always program a full page of data
 *	If you call it with a non page aligned buffer, you're lost :)
 */
static int nand_write_page (u_char *data_poi, int page,
	struct nand_oobinfo *oobsel, UINT8 wp)
{
	int i, eccmode, *oob_config;
	u_char ecc_code[6], oob[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x85, 0x19, 0x03, 0x20, 0x08, 0x00, 0x00, 0x00};
	/* force SOFT_ECC given by NAND driver, Jeanson */

	for (i = 0; i < NAND_PAGE_SIZE; i++)
		oobdata_buf[i] = data_poi[i];
	for (i = 0; i < NAND_PAGE_OOB_SIZE; i++)
		oobdata_buf[i + NAND_PAGE_SIZE] = oob[i];

	/*oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS] = SECTOR_USED;
	oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS + 1] = SECTOR_USED;
	oobdata_buf[NAND_PAGE_SIZE + 12] = oobdata_buf[NAND_PAGE_SIZE + 14] = (ERASE_MARK & 0xff00) >> 8;
	oobdata_buf[NAND_PAGE_SIZE + 13] = oobdata_buf[NAND_PAGE_SIZE + 15] = (ERASE_MARK & 0x00ff);*/


	if (oobsel)
	{ // задав nand_oobinfo как NULL, указывают
		eccmode = NAND_ECC_SOFT; // на то, что ecc не используется.
		oob_config = oobsel->eccpos;
	} else
		eccmode = NAND_ECC_NONE;


	/* Send command to begin auto page programming */
	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;

	/* Write out complete page of data, take care of eccmode */
	switch (eccmode)
	{ /* No ecc and software ecc 3/256, write all */
		case NAND_ECC_NONE:
			nand_write_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 0, wp);
			break;
		case NAND_ECC_SOFT:
			nand_calculate_ecc (&oobdata_buf[0], &ecc_code[0]);
			for (i = 0; i < 3; i++)
				oobdata_buf[NAND_PAGE_SIZE + oob_config[i]] = ecc_code[i];
			/* Calculate and write the second ECC for 512 Byte page size */
			nand_calculate_ecc (&oobdata_buf[256], &ecc_code[3]);
			for (i = 3; i < 6; i++)
				oobdata_buf[NAND_PAGE_SIZE + oob_config[i]] = ecc_code[i];

			nand_write_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 1, wp);
			break;
		default:
			//printk (KERN_WARNING "Invalid NAND_ECC_MODE %d\n", this->eccmode);
			//BUG();
			break;
	}

	/* The NAND device assumes that it is always writing to
	 * a cleanly erased page. Hence, it performs its internal
	 * write verification only on bits that transitioned from
	 * 1 to 0. The device does NOT verify the whole page on a
	 * byte by byte basis. It is possible that the page was
	 * not completely erased or the page is becoming unusable
	 * due to wear. The read with ECC would catch the error
	 * later when the ECC page check fails, but we would rather
	 * catch it early in the page write stage. Better to write
	 * no data than invalid data.
	 */
	/* Send command to read back the page */
	nand_read_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf2, (eccmode == NAND_ECC_SOFT) ? 1 : 0);

	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;

	/* Loop through and verify the data */
	for (i = 0; i < NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE * (eccmode == NAND_ECC_SOFT); i++)
		if (oobdata_buf[i] != oobdata_buf2[i])
		{
			print_val ("Failed write verify, page", page);
			return -EIO;
		}

	return 0;
}

// now, read_ecc(), read_oob(), write_ecc(), and write_oob()

/* NAND read with ECC */
static int
nand_read_ecc (loff_t from, size_t len, size_t * retlen, u_char * buf,
	struct nand_oobinfo *oobsel)
{
	int j, col, page, eccmode, *oob_config;
	int read = 0, ecc_status = 0, ecc_failed = 0;
	u_char ecc_calc[6], ecc_code[6];

	// use chip default if zero
	if (oobsel)
	{
		eccmode = NAND_ECC_SOFT;
		oob_config = oobsel->eccpos;
	} else
		eccmode = NAND_ECC_NONE;

	/* Do not allow reads past end of device */
	if ((from + len) > NAND_FLASH_SIZE)
	{
		buart_print ("\n\rnand_read_ecc: Attempt read beyond end of device");
		*retlen = 0;
		return -EINVAL;
	}
	/* First we calculate the starting page */
	page = from >> ADD2SHIFT; //9, this->page_shift;

	/* Send the read command */
	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;

	/* Loop until all data read */
	while (read < len)
	{
		switch (eccmode)
		{
			case NAND_ECC_NONE: /* No ECC, Read in a page */
				nand_read_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 0);
				break;

			case NAND_ECC_SOFT: /* Software ECC 3/256: Read in a page + oob data */
				nand_read_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 1);
				nand_calculate_ecc (&oobdata_buf[0], &ecc_calc[0]);
				nand_calculate_ecc (&oobdata_buf[256], &ecc_calc[3]);
				break;
			default:
				break;
		}
		// будет работать косячно, если читать не с начала сектора, но
		// мы читаем всегда с начала сектора =)
		// if this page is the 1st page of current block
		if ((page % NAND_BLK_PER_PAGE) == 0)
		{ // yes, this page is the 1st page, using the oob_data to check block validity
			if (oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS] != SECTOR_FREE)
			{ // this block might be used normal (marked FREE or USED)
				if ((oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS] !=
					oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS + 1]) ||
					((oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS] != SECTOR_USED) &&
					(oobdata_buf[NAND_PAGE_SIZE + NAND_BADBLOCK_POS] != SECTOR_FREE)))
				{
					page += NAND_BLK_PER_PAGE; // move forward to next block
					continue; // блок плохой? Прыгнули к следующему.
				}
			}
		}
		/* Skip ECC, if not active */
		if (eccmode == NAND_ECC_NONE)
			goto readdata;
		/* Pick the ECC bytes out of the oob data */
		for (j = 0; j < 6; j++)
			ecc_code[j] = oobdata_buf[NAND_PAGE_SIZE + oob_config[j]];
		/* correct data, if neccecary */
		ecc_status = nand_correct_data (&oobdata_buf[0], &ecc_code[0], &ecc_calc[0]);
		if (ecc_status == -1) // это значит, что данные прочитать уже не удастся
			ecc_failed++;
		// = следует как-то сообщить пользователю о постигшей его утрате.
		// или м.б. попытаться прочитать блок ещё раз 30? И если ecc_status = 1,
		// значит уже есть одна ошибка, которую мы исправили.

		if (eccmode != NAND_ECC_HW3_512) // всегда верно
		{
			ecc_status = nand_correct_data (&oobdata_buf[256], &ecc_code[3], &ecc_calc[3]);
			if (ecc_status == -1)
				ecc_failed++;
		}
readdata:
		// здесь нужна задержка, иначе виснет
		for (j = 0; j < 10000; j++);
		;
		// buart_print (".");
		for (j = 0; (j < NAND_PAGE_SIZE) && (read < len); j++)
			buf[read++] = oobdata_buf[j];
		// Increment page address
		page++;
	}
	// De-select the NAND device
	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;

	/* Return success, if no ECC failures, else -EIO
	 * fs driver will take care of that, because
	 * retlen == desired len and result == -EIO */
	*retlen = read;

	return ecc_failed ? -EIO : 0;
}

static int nand_mark_bad_block (UINT32 page)
{
	int i;

	page -= page % NAND_BLK_PER_PAGE; // на всякий случай, сместимся к 1-й странице
	nand_erase ((UINT8 *)(page * NAND_PAGE_SIZE), NAND_SIZE_PER_BLK, 0); // эту единицу следует убрать после отладки

	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;

	for (i = 0; i < NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE; i++)
		oobdata_buf[i] = 0; // забиваем блок нулями, в т.ч. и OOB

	nand_write_page_oob ((UINT8 *)(page * NAND_PAGE_SIZE), oobdata_buf, 1, 0); // эту тоже (убрал)

	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;

	// print_val ("Block marked as bad", page * NAND_PAGE_SIZE);

	return 0;
}

#define NOTALIGNED(x) ((x & (NAND_PAGE_SIZE-1)) != 0)

// NAND write with ECC

static int nand_write_ecc (loff_t to, size_t len, size_t * retlen, const u_char * buf,
	struct nand_oobinfo *oobsel, UINT8 wp)
{
	unsigned long page1st, blkAddr = -1;
	int page, ret = 0, written = 0;
	// struct nand_chip *this = mtd->priv;
	u_char *data_poi;

	/* Do not allow write past end of device */
	if ((to + len) > NAND_FLASH_SIZE)
	{
		buart_print ("\n\rnand_write_ecc: Attempt to write past end of page.");
		return -EINVAL;
	}
	/* reject writes, which are not page aligned */
	if (NOTALIGNED (to))
	{
		buart_print ("\n\rnand_write_ecc: Attempt to write not page aligned data.");
		return -EINVAL;
	}

	/* Shift to get page */
	page = ((int) to) >> ADD2SHIFT;

	/* //Select the NAND device
	nand_select_chip (0);
	nand_command (NAND_CMD_RESET, -1, -1);
	// *(base + NAND_SET_WP_REG) = 1;
	// Check the WP bit
	nand_command (NAND_CMD_STATUS, -1, -1);
	if (!(nand_read_byte () & 0x80)) // = надо применить этот механизм к
	{ // области загрузочных параметров
		buart_print ("nand_write_ecc: Device is write protected!!!\n");
		ret = -EIO;
		goto out;
	}*/
	/* Loop until all data is written */
	while (written < len)
	{ // before doing write, checking if this block is bad block or not ?
		// 32-pages per block, determine the 1st page
		page1st = page - page % NAND_BLK_PER_PAGE;
		/* get the first page of this current block */
		if (blkAddr != page1st)
		{ // checking this block, if not checked yet
			blkAddr = page1st;
			if (nand_block_bad (blkAddr) != 0) // 0 - хороший, 1 - плохой
			{ // this block is bad block, double confirm this is bad one, then try next block
				print_val ("\n\rBad flash block detected! Address", page1st * NAND_PAGE_SIZE);
				// move forward to the 1st page of next block
				page = page1st + NAND_BLK_PER_PAGE;
				continue;
			}
		}
		//
		data_poi = (u_char*) & buf[written];
		/* We use the same function for write and writev */
		if (nand_write_page (data_poi, page, oobsel, wp))
			// т.е. мы не смогли записать страницу, пометим блок как плохой
		{ // и начнём писать со следующего блока то, что начали писать в этом
			nand_mark_bad_block (page1st);
			written -= (page * NAND_PAGE_SIZE) % NAND_SIZE_PER_BLK; // вычтем то,
			// что уже успели записать в плохой блок.
			blkAddr = -1; // дабы снова проверить, ББ ли? И, естественно, прыгнуть.
			continue;
		}
		/* Update written bytes count */
		written += NAND_PAGE_SIZE;
		/* Increment page address */
		page++;
	}
out:
	// clear the WP
	/*// *(base + NAND_CLR_WP_REG) = 1;
	// De-select the NAND device
	nand_select_chip (-1);*/
	*retlen = written;

	return ret;
}

int nand_read (UINT8 *dst, UINT8 *src, UINT32 len)
{
	size_t retlen;

	if (nand_read_ecc ((loff_t) src, (size_t) len, &retlen, (u_char *) dst, &oobinfo_buf))
		buart_print ("\n\rOne or more bad pages detected. Check flash for bad blocks.");
	return (int) retlen;
}

int nand_write (UINT8 *dst, UINT8 *src, UINT32 len, UINT8 wp)
{
	size_t retlen;

	nand_write_ecc ((loff_t) dst, (size_t) len, &retlen, (const u_char *) src, &oobinfo_buf, wp);
	return (int) retlen;
}

// note that, the addr must be the block starting address

int nand_erase (UINT8 *addr, UINT32 len, UINT8 wp)
{
	UINT32 i, j, row1, row2;

	base = (UINT8 *) NAND_REG_BASE;

	if (wp) // снимаем write protection
		//buart_print ("\n\rnand_erase - WP");
		*(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_SPn_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;
	// here, NAND_SIZE_PER_BLK=0x4000
	for (i = 0; i < len; i += NAND_SIZE_PER_BLK, addr += NAND_SIZE_PER_BLK)
	{
		row1 = ((int) addr & ADD2MASK) >> ADD2SHIFT;
		row2 = ((int) addr & ADD3MASK) >> ADD3SHIFT;
		//
		*(base + NAND_SET_CLE_REG) = 1;
		*(base + NAND_RW_REG) = CMD_ERASE1;
		*(base + NAND_CLR_CLE_REG) = 1;
		*(base + NAND_SET_ALE_REG) = 1;
		*(base + NAND_RW_REG) = row1;
		*(base + NAND_RW_REG) = row2;
		*(base + NAND_CLR_ALE_REG) = 1;
		*(base + NAND_SET_CLE_REG) = 1;
		*(base + NAND_RW_REG) = CMD_ERASE2;
		*(base + NAND_CLR_CLE_REG) = 1;
		/* wait 1ms */
		for (j = 0; j < 100000; j++);
	}

	*(base + NAND_SET_CE_REG) = 1;

	return 0;
}

void check_bad_block (void)
{
	int i, page_no = NAND_FLASH_BLOCK_NO * NAND_BLK_PER_PAGE;
	// т.к. при записи загрузчика (первые 8 блоков) ECC не пишется (и оно ему в
	// любом случае не поможет), то и проверять его смысла нет
	for (i = NAND_BLK_PER_PAGE * 8; i < page_no; i += NAND_BLK_PER_PAGE)
	{
		if (nand_block_bad (i) != 0)
			print_val ("Bad block", i * NAND_PAGE_SIZE);
	}
}

// for writing bootloader

void nand_write_boot (UINT8 *dst, UINT8 *src, UINT32 len)
{
	volatile UINT8 *base;
	UINT32 i = 0;

	base = (UINT8 *) NAND_REG_BASE;
	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;

	/* write spare area if < NAND_PAGE_OOB_SIZE */
	while (i < NAND_BOOT_SIZE)
	{
		nand_write_page_oob (dst, &src[i], 1, 0);
		i += NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE;
		dst += NAND_PAGE_SIZE;
	}

	i = NAND_BOOT_SIZE;

	while (i < len)
	{
		nand_write_page_oob (dst, &src[i], 0, 0);
		i += NAND_PAGE_SIZE;
		dst += NAND_PAGE_SIZE;
	}

	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;
}


// после стирания сектора, всё его содержимое становится FF. Битый бит, это
// такой бит, который при записи в него 1 остаётся в 0. Насколько мне известно,
// залипших 1 в nand-flash не бывает. Поэтому достаточно прочитать содержимое
// всех блоков после форматирования. Если хоть один бит в блоке = 0,
// значит блок плохой, и его следует пометить таковым.

void scan_bad_blocks ()
{
	UINT32 j, src, page, block, bad = 0;
	// *(base + NAND_SET_WP_REG) = 1;
	*(base + NAND_CLR_CE_REG) = 1;

	buart_print ("\n\r");
	for (block = 0; block < NAND_FLASH_BLOCK_NO; block++)
	{
		bad = 0;
		if ((block * NAND_SIZE_PER_BLK >= LINUXLD_NANDFLASH_LOADER_SIZE) && (block * NAND_SIZE_PER_BLK < LINUXLD_NANDFLASH_KERNEL_START))
			continue; // пропускаем область с параметрами, т.к. оне на затирается форматированием
		print_val1 ("block", block * NAND_PAGE_SIZE * NAND_BLK_PER_PAGE);

		for (page = 0; page < NAND_BLK_PER_PAGE; page++)
		{
			src = (block * NAND_BLK_PER_PAGE + page) * NAND_PAGE_SIZE;
			nand_read_page_oob ((UINT8 *)src, oobdata_buf, 1);

			for (j = 0; j < NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE; j++)
				if (oobdata_buf[j] != 0xff) // битый байт
				{
					buart_print (" - bad.\n\r");
					nand_mark_bad_block (block * NAND_BLK_PER_PAGE);
					bad = 1;
					break;
				}
			if (bad == 1) break;
		}
	}

	*(base + NAND_SET_CE_REG) = 1;
	// *(base + NAND_CLR_WP_REG) = 1;

	buart_print ("\n\r");
}

void bad_block (void)
{
	nand_select_chip (0);
	nand_hwcontrol (NAND_CTL_CLRWP);
	//nand_mark_bad_block(0x140);	//0x028000
	nand_mark_bad_block (0x1300); //0x260000
	nand_mark_bad_block (0x1320); //0x264000
	nand_mark_bad_block (0xA800); //0x1500000
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip (-1);
}
