#include <mips4kc.h>
#include <adm5120.h>
#include <hw_profile.h>
#include <ctype.h>
#include <nand.h>
#include <bconfig.h>
#include <uartdrv.h>
#include <if_5120.h>
#include <linuxld.h>
#include <banner.h>
#include <param.h>
#include <bsp_cfg.h>
#include <dslam.h>

#define VERSION "1.3"

#define LOADER_START_HINT       '@'


UINT8 dslam_present = 0;
char hexdigit[] = "0123456789abcdef";
char bt_name[] = "\r\nADM0000 Boot:";
const char version_string[] = "\n\rVersion "VERSION" (" __DATE__ " - " __TIME__ ")";
void *_heap_h;

extern void _icache_sync_all (void);


// This routine prints the main menu
static void print_menu ()
{
	buart_print ("\n\r"
		"\n\rADM5120 based router:"
		"\n\r=================================="
		"\n\rBootloader Menu"
		"\n\r [1] Xmodem download"
		"\n\r [2] TFTP download"
		"\n\r [3] Print boot params"
		"\n\r [4] Set boot params"
		"\n\r [5] Flash operations"
		"\n\r [6] Reset\n"
		"\n\rPlease enter your number:");
}

static void print_tftpc_menu (void)
{
	buart_print ("\n\rTFTP Client Menu");
	buart_print ("\n\r===============================");
	buart_print ("\n\r [B]: Update bootloader");
	buart_print ("\n\r [S]: Update system");
	buart_print ("\n\r [A]: Update all");
	buart_print ("\n\r [P]: Set parameters");
	buart_print ("\n\r [X]: exit");
	buart_print ("\n\rEnter your option:");
}

void tftp_client_menu (void)
{
	char key;
	while (1)
	{
		print_tftpc_param ();
		print_tftpc_menu ();
		key = buart_getchar ();
		buart_put (key);
		switch (key)
		{
			case 'B':
			case 'b':
				tftpc_download (TFTP_LOAD_BOOTLOADER);
				break;

			case 'S':
			case 's':
				tftpc_download (TFTP_LOAD_LINUX);
				break;

			case 'A':
			case 'a':
				tftpc_download (TFTP_LOAD_BOOTLOADER);
				tftpc_download (TFTP_LOAD_LINUX);
				break;

			case 'P':
			case 'p':
				set_tftpc_param ();
				break;

			case 'X':
			case 'x':
				return;

			default:
				break;
		}
	}

}

static void print_xmodem_menu (void)
{
	buart_print ("\n\rXmodem Client Menu");
	buart_print ("\n\r===============================");
	buart_print ("\n\r [B]: Update bootloader");
	buart_print ("\n\r [S]: Update system");
	buart_print ("\n\r [X]: Exit");
	buart_print ("\n\rEnter your option:");
}

void xmodem_client_menu (void)
{
	char key;
	while (1)
	{
		print_xmodem_menu ();
		key = buart_getchar ();
		buart_put (key);
		switch (key)
		{
			case 'B':
			case 'b':
				update_bootloader ();
				break;

			case 'S':
			case 's':
				xmodem_download ();
				break;

			case 'X':
			case 'x':
				return;

			default:
				break;
		}
	}

}

static void print_flash_menu (void)
{
	buart_print ("\n\rFlash Client Menu");
	buart_print ("\n\r===============================");
	buart_print ("\n\r [P]: Print existent bad blocks");
	buart_print ("\n\r [E]: Erase entire flash");
	buart_print ("\n\r [A]: Erase entire flash, scan for bad blocks, and mark them");
	buart_print ("\n\r [C]: Create bad blocks");
	buart_print ("\n\r [X]: Exit");
	buart_print ("\n\rEnter your option:");
}

void flash_client_menu (void)
{
	char key;
	while (1)
	{
		print_flash_menu ();
		key = buart_getchar ();
		buart_put (key);
		switch (key)
		{
			case 'P':
			case 'p':
				check_for_bad ();
				break;

			case 'E':
			case 'e':
				flash_erase_all ();
				break;

			case 'A':
			case 'a':
				flash_erase_all ();
				find_bad_blocks ();
				break;

			case 'C':
			case 'c':
				create_bad_blocks ();
				break;

			case 'X':
			case 'x':
				return;

			default:
				break;
		}
	}

}

// This routine is the C entry pointer of the loader

void c_entry (void)
{
	int i, c, countdown = 3;
	unsigned long tick1, tick2;

	buart_init ();
	memlib_init ();
	InitTimer ();

	//print '@' at the very beginning
	i = ADM5120_SW_REG (CODE_REG) & CODE_ID_MASK;
	bt_name[5] = hexdigit[(i >> 12)&0xf];
	bt_name[6] = hexdigit[(i >> 8)&0xf];
	bt_name[7] = hexdigit[(i >> 4)&0xf];
	bt_name[8] = hexdigit[i & 0xf];
	buart_print (bt_name);
	buart_print ("\n\r");
	//buart_print ("\n\rCopyleft");
	buart_print ("\n\rCPU: Infineon ");
	buart_print (CPU_NO);
	buart_put ('-');
	buart_print (CPU_CLOCK);
	buart_print ("\n\rSDRAM: ");
	buart_print (SDRAM_SIZE);
	buart_print ("\n\rFlash: ");
	buart_print (FLASH_TYPE);
	buart_put ('-');
	buart_print (FLASH_SIZE);
	buart_print ("\n\rBoot System: ");
	buart_print (BOOT_SYSTEM);
	buart_print (version_string);
	buart_print ("\n\r\n\rPress any key to enter boot menu..");
	buart_print ("\n\r");

	// wait BOOT_DELAY second before boot
	buart_put (countdown + '0');
	tick1 = UpTime ();
	while (TRUE)
	{
		tick2 = UpTime ();
		if ((tick2 - tick1) >= 200) // секунда?
		{
			if (countdown-- < 0)
			{
				boot_linux ();
				break;	// если загрузка не удалась
			}
			buart_put ('\r');
			buart_put (countdown + '0');
			tick1 = UpTime ();
		}
		if (buart_get (0) > 0)
			break;
	}

	// Read the Network configuration data.
	if (boot_param_init () != 0)
	{
		buart_print ("\n\rRead bsp error.");
		panic ();
	}

	// Init Switch.
	if (if5120_init () != 0)
	{
		buart_print ("\n\rSwitch init error.");
		panic ();
	}

	// Init networks for TFTP.
	tftp_init ();

	check_for_bad ();

	dslam_present = IsDslamBoardPresent ();
	if (dslam_present)
	{
		buart_print ("\n\rDSLAM board detected.");
		ADM5120_SW_REG (GPIO_conf0_REG) = 0;
		ADM5120_SW_REG (GPIO_conf2_REG) = 0;
		//ADM5120_SW_REG (Port_conf2_REG) = 0x10a;	// включим порт
		//store_vlan2port ();
		store_default (0);							// включаем нулевой
		store_default (0);
		store_default (1);							// и первый свитч
	}

	// Тест памяти
	/*volatile UINT32 *x;
	buart_print ("Testing memory");
	for (x = 0xa0000000; x < 0xa4000000; x++)
	{
		if ((x < LINUXLD_MEMPOOL_START) || (x > LINUXLD_DOWNLOAD_START))
		{
			*x = (UINT32)(x);
			if (((UINT32)(x) % 0x100000) == 0)
				buart_print (".");
		}
		else
			if (((UINT32)(x) % 0x100000) == 0)
				buart_print ("x");
	}
	buart_print ("\n\rTesting memory");
	for (x = 0xa0000000; x < 0xa4000000; x++)
	{
		if ((x < LINUXLD_MEMPOOL_START) || (x > LINUXLD_DOWNLOAD_START))
		{
			if (((UINT32)(x) % 0x100000) == 0)
			{
				if (*x == (UINT32)(x))
					buart_print ("+");
				else
					buart_print ("-");
			}
		}
		else
			if (((UINT32)(x) % 0x100000) == 0)
				buart_print ("x");
	}*/

	// Карта памяти
	/*char buf[10] = {0};
	buart_print ("\n\rMemory map:");
	buart_print ("\n\r EXCEP:     0x");
	ultoa (LINUXLD_SDRAM_START, buf);
	buart_print (buf);
	buart_print (" - 0x");
	ultoa (LINUXLD_SDRAM_START + LINUXLD_EXCEP_SIZE, buf);
	buart_print (buf);
	buart_print ("\n\r KERNEL:    0x");
	ultoa (LINUXLD_KERNEL_START, buf);
	buart_print (buf);
	buart_print (" - 0x");
	ultoa (LINUXLD_KERNEL_START + LINUXLD_KERNEL_SIZE, buf);
	buart_print (buf);
	buart_print ("\n\r MEMPOOL:   0x");
	ultoa (LINUXLD_MEMPOOL_START, buf);
	buart_print (buf);
	buart_print (" - 0x");
	ultoa (LINUXLD_MEMPOOL_START + LINUXLD_MEMPOOL_SIZE, buf);
	buart_print (buf);
	buart_print ("\n\r LOADER:    0x");
	ultoa (LINUXLD_LOADER_START, buf);
	buart_print (buf);
	buart_print (" - 0x");
	ultoa (LINUXLD_LOADER_START + LINUXLD_LOADER_SIZE, buf);
	buart_print (buf);
	buart_print ("\n\r DOWNLOAD:  0x");
	ultoa (LINUXLD_DOWNLOAD_START, buf);
	buart_print (buf);
	buart_print (" - 0x");
	ultoa (LINUXLD_DOWNLOAD_START + LINUXLD_DOWNLOAD_SIZE, buf);
	buart_print (buf);*/

	//Bootloader Function Start.
	while (1)
	{
		print_menu ();

		c = buart_getchar ();

		buart_put (c);

		switch (c)
		{
			case '1':
				xmodem_client_menu ();
				break;

			case '2':
				tftp_client_menu ();
				break;

			case '3':
				PrintBspParam ();
				break;

			case '4':
				set_boot_param ();
				break;

			case '5':
				flash_client_menu ();
				break;

			case '6':
				ADM5120_SW_REG (SftRest_REG) = SOFTWARE_RESET;
				break;

			default:
				break;
		}
	}
}


