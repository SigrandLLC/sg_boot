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
#include <menu.h>
#include <memlib.h>
#include <timer.h>
#include <except.h>
#include <tftp.h>
#include <version.h>

#define LOADER_START_HINT       '@'


UINT8 dslam_present = 0;
static char hexdigit[] = "0123456789abcdef";
static char bt_name[] = "\r\nADM0000 Boot:";
static const char version_string[] = "\n\rVersion "VERSION" (" __DATE__ " - " __TIME__ ")";

extern void _icache_sync_all (void);



static void reset(void)
{
	buart_print ("\n\r");
	ADM5120_SW_REG (SftRest_REG) = SOFTWARE_RESET;
}


//+ xmodem menu
#ifndef NO_XMODEM
extern int update_bootloader ();
extern int xmodem_download (void);

static menu_entry_t xmodem_menu[] =
{
	{ .key = 'B', .line = "Update bootloader", .func_void = (menu_func_void_t)update_bootloader },
	{ .key = 'S', .line = "Update system"    , .func_void = (menu_func_void_t)xmodem_download },
	{ .key = 'X', .line = "Exit"             , .func_int  = menu_exit },
	{ .key = 'R', .line = "Reset"            , .func_void = reset },
	{ .key = '\0' }
};

void xmodem_client_menu (void)
{
	menu_do_all("Xmodem Client Menu", NULL, xmodem_menu);
}
#endif
//+ xmodem menu


//+ flash menu
static void flash_erase_all_scan_bad(void)
{
	flash_erase_all ();
	find_bad_blocks ();
}

static menu_entry_t flash_menu[] =
{
	{ .key = 'P', .line = "Print existent bad blocks", .func_void = check_for_bad },
	{ .key = 'C', .line = "Create bad blocks"        , .func_void = create_bad_blocks },
	{ .key = 'E', .line = "Erase entire flash"       , .func_void = flash_erase_all },
	{ .key = 'A', .line = "Erase entire flash, scan for bad blocks, and mark them"
							 , .func_void = flash_erase_all_scan_bad },
	{ .key = 'X', .line = "Exit"                     , .func_int  = menu_exit },
	{ .key = 'R', .line = "Reset"                    , .func_void = reset },
	{ .key = '\0' }
};

void flash_client_menu (void)
{
	menu_do_all("Flash Client Menu", NULL, flash_menu);
}
//- flash menu


#if 0 // too dangerous and useless
void tftpc_download_all(void)
{
	tftpc_download (TFTP_LOAD_BOOTLOADER);
	tftpc_download (TFTP_LOAD_LINUX);
}
#endif

//+ main menu
static menu_entry_t main_menu[] =
{
	{ .key = 'P', .line = "Set parameters"   , .func_void = SetAllParam },
#ifndef NO_XMODEM
	{ .key = 'M', .line = "Xmodem download"  , .func_void = xmodem_client_menu },
#endif
	{ .key = 'S', .line = "Update OS    "    , .func_int  = tftpc_download, .int_data = TFTP_LOAD_LINUX },
	{ .key = 'B', .line = "Update Bootloader", .func_int  = tftpc_download, .int_data = TFTP_LOAD_BOOTLOADER },
//	{ .key = 'A', .line = "Update All"       , .func_void = tftpc_download_all },
//	{ .key = 'F', .line = "Flash operations" , .func_void = flash_client_menu },
	{ .key = 'R', .line = "Reset"            , .func_void = reset },
	{ .key = '\0' }
};
//- main menu


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
	buart_print ("\r\n\n");
	buart_print (bt_name);
	//buart_print ("\n\r");
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
	buart_print ("\n\r\n\rPress any key to enter boot menu or 'q' to boot OS immediately...");
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
		c = buart_get (0);
		if (c > 0)
		{
			if (c == 'q' || c == 'Q')
			{
				boot_linux ();
				break;
			}
			break;
		}
	}

	// Read the Network configuration data.
	if (boot_param_init () < 0)
	{
		buart_print ("\n\rRead bsp error.");
		panic ();
	}

	// Init Switch.
	if (if5120_init () < 0)
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
	buart_print ("\n\rMemory map: ");
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
		menu_do_all("ADM5120 based router: Bootloader Menu", PrintAllParam, main_menu);
}

