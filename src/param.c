#include <ctype.h>
#include <string.h> // strlen strcpy memcpy
#include <param.h>
#include <utils.h>
#include <memlib.h>
#include <uartdrv.h>
#include <bsp_cfg.h>
#include <adm5120.h>
#include <linuxld.h>
#include <mips4kc.h>
#include <nf.h>
#include <arp.h>
#include <ip.h>
#include <udp.h>
#include <menu.h>

/*****************************************************************************************/
void Set_Board_SerialNo (void);
void Set_Board_Version (void);
static void Set_Mac (void);
static void Set_IP  (void);

/*****************************************************************************************/
// lnitial boot parameter
static BOARD_CFG_T *cfg;
static int cfg_changed;

int expert_mode;
extern const int bootloader_type_ram;

/*****************************************************************************************/
/*		Parameter Init								 */
/*****************************************************************************************/
int boot_param_init (void)
{
	int err = -1;
	// Read the Network configuration data.
	cfg = (BOARD_CFG_T *) MemAlloc (sizeof (BOARD_CFG_T), TRUE);
	if (cfg != NULL)
		err = nf_read ((UINT8 *) cfg, (UINT8 *) LINUXLD_NANDFLASH_BOOTPARAM_START, sizeof (BOARD_CFG_T));
	cfg_changed = 0;
	expert_mode = bootloader_type_ram;
	return err;
}

#if 0
/*****************************************************************************************/
/*		Main Parameter																	 */

/*****************************************************************************************/
int set_boot_param (void)
{
	char *image = (char *) LINUXLD_DOWNLOAD_START;

	/* Print Item */
	buart_print ("\n\r  Set Boot Parameters.");
	buart_print ("\n\r==========================\n\r");

	/* Set serial number */
	// Set_Board_SerialNo ();

	/* Set board version */
	// Set_Board_Version ();

	/* Set Mac address */
	Set_Mac ();

	/* Set bootloader ip */
	Set_IP ();

	/* Before Write back, backup original content */
	if (nf_read (image, (char *) LINUXLD_NANDFLASH_BOOTPARAM_START, LINUXLD_NANDFLASH_BOOTPARAM_SIZE) < 0)
	{
		buart_print ("\n\rRead buffer error!!");
		return -1;
	}
	memcpy (image, (char *) cfg, sizeof (BOARD_CFG_T));

	/* Write back new parameter to flash */
	if (nf_erase ((char *) LINUXLD_NANDFLASH_BOOTPARAM_START, LINUXLD_NANDFLASH_BOOTPARAM_SIZE, 1) < 0) // параметры write protected
		buart_print ("\n\rErase flash error.");
	else if (nf_write ((char *) LINUXLD_NANDFLASH_BOOTPARAM_START, image, LINUXLD_NANDFLASH_BOOTPARAM_SIZE, 1) < 0)
		buart_print ("\n\rWrite flash error.");
	if (cfg->tftploipmagic == TFTPLOIPMAGIC && cfg->macmagic == MAC_MAGIC)
		arp_add_entry (cfg->mac, cfg->tftp_param.local_ip);

	cfg_changed = 0;
	return 0;
	/* Wait any key */
	//buart_print("\n\r\n\rPlease press any key to continue...");
	//buart_getchar();
}
#endif

static int bsp_SetMac (UINT8 *mac, int macnum)
{
	int i;
	UINT32 sum;

	if ((mac == NULL) || (macnum < 1) || (macnum > BSP_MAX_MAC_NUM))
		return -1;

	// Allow only unicast mac address
	if (mac[0] != 0) return -1;

	if (cfg->macmagic == MAC_MAGIC)
	{
		if (memcmp(mac, cfg->mac, 6) != 0)
			cfg_changed = 1;
	}
	else
		cfg_changed = 1;

	memset(cfg->mac, 0, sizeof(cfg->mac));
	memcpy(cfg->mac, mac, 6);
	cfg->macnum = macnum;
	cfg->macmagic = MAC_MAGIC;

	// Ripple adder
	sum = macnum - 1;
	for (i = 5; i > 2; i--)
	{
		if ((sum += cfg->mac[i]) > 255)
		{
			// ripple carry to the next byte.
			cfg->mac[i] = (unsigned char) (sum & 0xff);
			sum >>= 8;
		} else
		{
			cfg->mac[i] = (unsigned char) sum;
			break;
		}
	}

	if (i == 2) return ERROR;

	// have to use memcpy here.
	memcpy (cfg->mac, mac, 6);

	return 0;
}

int bsp_GetMacBase (UINT8 *buf, int *macnum)
{
	if (buf == NULL) return -1;

	if (cfg->macmagic != MAC_MAGIC) return -1;

	// have to use memcpy here.
	memcpy (buf, cfg->mac, 6);

	*macnum = (int) cfg->macnum;

	return 0;
}

static void Set_Mac (void)
{
	char buf[BOOT_LINE_SIZE + 1];
	UINT8 mac[8];
	char mactmp[] = "00-00-00-00-00-00-";
	int macnum, i;
	int flags = 0;
#ifndef NO_NUMBER_OF_MACS
	int newmacnum;
#endif

#define FLAG_OLD_MAC_VALID		1
#define FLAG_MAC_MODIFIED		2
#define FLAG_MAC_NUM_MODIFIED	4


	if (bsp_GetMacBase (mac, &macnum) == 0)
	{
		flags |= FLAG_OLD_MAC_VALID;
		mactostr (mac, mactmp);
		mactmp[17] = '\0';
		buart_print ("\n\rCurrent MAC address: ");
		buart_print (mactmp);
#ifndef NO_NUMBER_OF_MACS
		buart_print ("\n\r");
		buart_print ("Number of MAC address: ");
		buart_put (macnum + '0');
#endif
	}

mac_again:
	buart_print ("\n\rEnter new MAC address (XX-XX-XX-XX-XX-XX): ");
	buf[0] = 0;
	ReadLine (buf, BOOT_LINE_SIZE);

	if (buf[0] != 0)
	{
		i = macscanf (mac, buf);

		if (i < 6 || mac[0] != 0x00)
		{
			buart_print ("Incorrect MAC address!\n\r");
			goto mac_again;
		}

		flags |= FLAG_MAC_MODIFIED;
	}

#ifndef NO_NUMBER_OF_MACS
num_again:
	buart_print ("Enter new number of MAC address (between 1-8): ");
	newmacnum = buart_getchar ();
	buart_put (newmacnum);

	if (!((newmacnum == 0x0d) || (newmacnum == 0x0a)))
	{
		newmacnum = newmacnum - '0';
		if (newmacnum < 1 || newmacnum > 8)
		{
			buart_print ("\n\rNumber of MAC adderesses must be in the range [1-8].\n\r");
			goto num_again;
		}

		macnum = newmacnum;
		flags |= FLAG_MAC_NUM_MODIFIED;
	}
#else
	macnum = 1;
#endif

	if ( (flags & FLAG_MAC_MODIFIED) ||
	    ((flags & FLAG_OLD_MAC_VALID) && (flags & FLAG_MAC_NUM_MODIFIED)))
	{
		if (!(flags & FLAG_MAC_NUM_MODIFIED) &&
		    !(flags & FLAG_OLD_MAC_VALID))
		{
			macnum = 1;
		}

		if (bsp_SetMac (mac, macnum) != 0)
		{
			buart_print ("\n\rFailed to change MAC address.");
		} else
		{
			UINT8 tmpmac[8];
			memcpy (tmpmac, cfg->mac, 6);
			ProgramMac (0, tmpmac); // Change current MAC addr.
			eth_reinit ();
			buart_print ("\n\rMAC address updated successfully.");
		}
	} else
	{
		buart_print ("\n\rMAC address unchanged.");
	}
}

static void Set_IP (void)
{
	UINT32 loip;
	char str[] = "xxx.xxx.xxx.xxx";
	char buf[BOOT_LINE_SIZE + 1];
	if (cfg->tftploipmagic == TFTPLOIPMAGIC)
	{
		buart_print ("\n\rLocal IP address: ");
		IpAddrToStr (cfg->tftp_param.local_ip, str);
		buart_print (str);
	}
loip_again:
	buf[0] = 0;
	buart_print ("\n\rEnter new local IP address: ");
	ReadLine (buf, BOOT_LINE_SIZE);
	if (buf[0] != 0)
	{
		if (ipscanf (&loip, buf) != 4)
		{
			buart_print ("Invalid IP address.\n\r");
			goto loip_again;
		}
		if (check_ip (loip, HOST_IP_FLAG))
		{
			buart_print ("Invalid IP address.\n\r");
			goto loip_again;
		}

		if (cfg->tftploipmagic == TFTPLOIPMAGIC)
		{
			if (cfg->tftp_param.local_ip != loip)
				cfg_changed = 1;
		}
		else
			cfg_changed = 1;

		cfg->tftp_param.local_ip = loip;
		cfg->tftploipmagic = TFTPLOIPMAGIC;
		ip_reinit (cfg->tftp_param.local_ip);
		udp_init ();
		buart_print ("IP updated successfully.\n\r");
	} else
	{
		buart_print ("IP unchanged.\n\r");
	}
}

int bsp_GetTftpIp (UINT32 *tftpip)
{
	if (tftpip == NULL) return -1;
	if (cfg->tftploipmagic != TFTPLOIPMAGIC) return -1;
	*tftpip = cfg->tftp_param.local_ip;
	return 0;
}

int bsp_GetGwIp (UINT32 *gwip)
{
	if (gwip == NULL) return -1;
	*gwip = cfg->tftp_param.gw_ip;
	return 0;
}

static int PrintTftpIP(void)
{
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		char str[] = "xxx.xxx.xxx.xxx";
		IpAddrToStr (cfg->tftp_param.server_ip, str);
		buart_print (str);
		return 0;
	}
        return -1;
}

static int PrintTftpGW(void)
{
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		char str[] = "xxx.xxx.xxx.xxx";
		IpAddrToStr (cfg->tftp_param.gw_ip, str);
		buart_print (str);
		return 0;
	}
        return -1;
}

static int PrintBootName(void)
{
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		buart_print (cfg->tftp_param.bootloader_name);
		return 0;
	}
        return -1;
}

static int PrintLinuxName(void)
{
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		buart_print (cfg->tftp_param.linux_name);
		return 0;
	}
        return -1;
}

static void print_tftpc_param (void)
{
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		buart_print ("\n\rTFTP Server IP address: ");
		PrintTftpIP();
		buart_print ("\n\rTFTP Server Gateway IP address: ");
		PrintTftpGW();
		buart_print ("\n\rRemote bootloader file name: ");
		PrintBootName();
		buart_print ("\n\rRemote Linux file name: ");
		PrintLinuxName();
		buart_print ("\n\r");
	}
}

static int PrintMac(void)
{
	int macnum;
	UINT8 buf[BOOT_LINE_SIZE + 1];
	char mactmp[] = "00-00-00-00-00-00-";
	int rc = bsp_GetMacBase (buf, &macnum);
	if (rc == 0)
	{
		mactostr (buf, mactmp);
		mactmp[17] = '\0';
		buart_print (mactmp);
		rc = macnum;
	}

	return rc;
}

static int PrintIp(void)
{
	UINT32 tftpip;
	char ipstr[] = "xxx.xxx.xxx.xxx";

	int rc = bsp_GetTftpIp (&tftpip);
	if (rc != 0)
		ipstr[0] = 0;
	else
		IpAddrToStr (tftpip, ipstr);

	buart_print (ipstr);

	return rc;
}

void PrintAllParam (void)
{
	int macnum = 0;

	/* Print Item */
#if 0
	buart_print ("\n\n\rPrint Boot Parameters.");
	buart_print ("\n\r==========================\n\r");
#else
	buart_print ("\n\r");
#endif

	/* Print Mac address */
	buart_print ("\n\rMAC address: ");
	macnum = PrintMac();

#ifndef NO_NUMBER_OF_MACS
	buart_print ("\n\rNumber of MAC addresses: ");
        if (macnum >= 0)
		buart_put (macnum + '0');
        else
		buart_print ("0");
#endif

	/* Print IP for TFTP Download */
	buart_print ("\n\rIP address: ");
	PrintIp();

	print_tftpc_param();
}

int get_tftp_param (UINT32 *servip, UINT32 *gwip, char *servfile, int mode)
{
	if (servip == NULL || servfile == NULL) return -1;
	if (cfg->tftpmagic != TFTPMAGIC) return -1;
	*servip = cfg->tftp_param.server_ip;
	*gwip = cfg->tftp_param.gw_ip;
	switch (mode)
	{
		case TFTP_LOAD_BOOTLOADER:
			strcpy (servfile, cfg->tftp_param.bootloader_name);
			break;
		case TFTP_LOAD_LINUX:
			strcpy (servfile, cfg->tftp_param.linux_name);
			break;
		default:
			return -1;
	}
	return 0;
}

static void set_tftp_ip(void)
{
	UINT32 servip;
	char buf[BOOT_LINE_SIZE + 1];

servip_again:
	buf[0] = 0;
	buart_print ("\n\rEnter TFTP server IP address: ");
	ReadLine (buf, BOOT_LINE_SIZE);
	if (buf[0] != 0)
	{
		if (ipscanf (&servip, buf) != 4)
		{
			buart_print ("Invalid IP address.\n\r");
			goto servip_again;
		}

		if (check_ip (servip, HOST_IP_FLAG))
		{
			buart_print ("Invalid IP address.\n\r");
			goto servip_again;
		}

		if (cfg->tftpmagic == TFTPMAGIC)
		{
			if (cfg->tftp_param.server_ip != servip)
				cfg_changed = 1;
		}
		else
			cfg_changed = 1;

		cfg->tftp_param.server_ip = servip;
		cfg->tftpmagic = TFTPMAGIC;
	} else
	{
		buart_print ("TFTP Server IP address unchanged.\n\r");
	}
}

static void set_tftp_gw(void)
{
	UINT32 gwip;
	char buf[BOOT_LINE_SIZE + 1];

gwip_again:
	buf[0] = 0;
	buart_print ("\n\rEnter TFTP server gateway IP address: ");
	ReadLine (buf, BOOT_LINE_SIZE);
	if (buf[0] != 0)
	{
		if (ipscanf (&gwip, buf) != 4)
		{
			buart_print ("Invalid IP address.\n\r");
			goto gwip_again;
		}

		if (gwip != 0) // 0.0.0.0 is correct gateway address
			if (check_ip (gwip, HOST_IP_FLAG))
			{
				buart_print ("Invalid IP address.\n\r");
				goto gwip_again;
			}

		if (cfg->tftpmagic == TFTPMAGIC)
		{
			if (cfg->tftp_param.gw_ip != gwip)
				cfg_changed = 1;
		}
		else
			cfg_changed = 1;

		cfg->tftp_param.gw_ip = gwip;
		cfg->tftpmagic = TFTPMAGIC;
	} else
	{
		buart_print ("TFTP server gateway IP address unchanged.\n\r");
	}
}

static void set_tftp_boot_name(void)
{
	char buf[BSP_FILENAME_STR_LEN + 1];

	buf[0] = 0;
	buart_print ("\n\rEnter Remote bootloader file name: ");
	ReadLine (buf, sizeof(buf)-1);
	if (buf[0] != 0)
	{
		if (cfg->tftpmagic == TFTPMAGIC)
		{
			if (strncmp(cfg->tftp_param.bootloader_name, buf,
				    sizeof(cfg->tftp_param.bootloader_name)-1) != 0)
				cfg_changed = 1;
		}
		else
			cfg_changed = 1;

		strncpy (cfg->tftp_param.bootloader_name, buf,
			 sizeof(cfg->tftp_param.bootloader_name)-1);
		cfg->tftpmagic = TFTPMAGIC;
	}
	else
		buart_print ("Bootloader file name unchanged.\n\r");
}

static void set_tftp_linux_name(void)
{
	char buf[BSP_FILENAME_STR_LEN + 1];

	buf[0] = 0;
	buart_print ("\n\rEnter Remote Linux file name: ");
	ReadLine (buf, sizeof(buf)-1);
	if (buf[0] != 0)
	{
		if (cfg->tftpmagic == TFTPMAGIC)
		{
			if (strncmp(cfg->tftp_param.linux_name, buf,
				    sizeof(cfg->tftp_param.linux_name)-1) != 0)
				cfg_changed = 1;
		}
		else
			cfg_changed = 1;

		strncpy (cfg->tftp_param.linux_name, buf,
			 sizeof(cfg->tftp_param.linux_name)-1);
		cfg->tftpmagic = TFTPMAGIC;
	}
	else
		buart_print ("Linux file name unchanged.\n\r");
}

static int write_params(void)
{
	UINT8 *image = (UINT8 *) LINUXLD_DOWNLOAD_START;
        int rc = 0;

	buart_print (" writing parameters ");

	/* Before Write back, backup original content */
	rc = nf_read (image, (UINT8 *) LINUXLD_NANDFLASH_BOOTPARAM_START, LINUXLD_NANDFLASH_BOOTPARAM_SIZE);
	if (rc < 0)
	{
		buart_print ("\n\rRead buffer error!!");
		return rc;
	}
	memcpy (image, (char *) cfg, sizeof (BOARD_CFG_T));

	/* Write back new parameter to flash */
	rc = nf_erase ((UINT8*) LINUXLD_NANDFLASH_BOOTPARAM_START,
		                LINUXLD_NANDFLASH_BOOTPARAM_SIZE, 1); // write protected
	if (rc < 0)
	{
		buart_print ("\n\rErase flash error.");
                return rc;
	}

	rc = nf_write ((UINT8 *) LINUXLD_NANDFLASH_BOOTPARAM_START, image, LINUXLD_NANDFLASH_BOOTPARAM_SIZE, 1);
	if (rc < 0)
		buart_print ("\n\rWrite flash error.");

	if (rc >= 0)
	{
		buart_print (" PASS\n\r");
		cfg_changed = 0;
	}
	return rc;
}

#if 0
int set_tftpc_param (void)
{
	set_tftp_ip();
	set_tftp_gw();
	set_tftp_boot_name();
        set_tftp_linux_name();

	int rc = write_params();
	buart_print ("\n\r");
	return rc;
}
#endif

static menu_rc_t save_exit(int dummy)
{
	if (cfg_changed)
		write_params();
	return menu_exit(dummy);
}

static void SetExpertMode(void)
{
	if (expert_mode)
	{
                expert_mode = 0;
		buart_print ("\n\rExpert mode Off");
	}
	else
	{
		char buf[BSP_STR_LEN + 1];

		buf[0] = 0;
		buart_print ("\n\rEnter expert password: ");
		ReadLine (buf, sizeof(buf)-1);

		if (ap_hash(buf) == 0xD7AD3E6D)
		{
			expert_mode = 1;
			buart_print ("\n\rExpert mode On");
		}
		else
		{
			expert_mode = 0;
			buart_print ("\n\rWrong password, expert mode Off");
		}
	}
}

static void PrintExpertMode(void)
{
	buart_print( expert_mode ? "On" : "Off" );
}


#define DEFINE_IP(a,b,c,d)	\
    ( ((a) << 24) | ((b) << 16) | ((c) << 8) | (d) )

enum {
    CONFIG_G,
    CONFIG_Z, CONFIG_Y,
    CONFIG_S, CONFIG_R,
    CONFIG_1, CONFIG_2,
    CONFIG_A, CONFIG_B , CONFIG_C
};

static BOARD_CFG_T configs[] =
{
    [CONFIG_G] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2,100),
	    .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_Z] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,0, 24),
	    .server_ip       = DEFINE_IP(192,168,0,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "zx.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_Y] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2,124),
	    .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "zx.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_S] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,1, 40),
	    .server_ip       = DEFINE_IP(192,168,1,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "si.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_R] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2,140),
	    .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "si.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_1] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2,101),
	    .server_ip       = DEFINE_IP(192,168,2,  4),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
	    .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_2] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2,102),
            .server_ip       = DEFINE_IP(192,168,2,  4),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
            .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_A] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2, 91),
            .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
            .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_B] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2, 92),
            .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
            .bootloader_name = "sg5120boot_rom.bin"
	}
    },

    [CONFIG_C] =
    {
	.macmagic = MAC_MAGIC, .macnum = 0,
	.mac = { 0x00,0xFF,0x0F,0x01,0x02,0x03 },
	.tftploipmagic = TFTPLOIPMAGIC, .tftpmagic = TFTPMAGIC,
	.tftp_param =
	{
	    .local_ip        = DEFINE_IP(192,168,2, 93),
            .server_ip       = DEFINE_IP(192,168,2,  1),
	    .gw_ip           = DEFINE_IP(  0,  0,0,  0),
	    .linux_name      = "sg.bin",
            .bootloader_name = "sg5120boot_rom.bin"
	}
    },

};

static menu_rc_t set_default_params(int idx)
{
    memcpy(cfg, &configs[idx], sizeof(*cfg));
    cfg_changed = 1;
    return MENU_DONE;
}

static void predefined_menu(void)
{
    static menu_entry_t menu[] =
    {
	{ .key = 'G', .func_int = set_default_params, .int_data = CONFIG_G, .line = "set 2.100 parameters" },
	{ .key = 'Z', .func_int = set_default_params, .int_data = CONFIG_Z, .line = "set 0.24  parameters" },
	{ .key = 'Y', .func_int = set_default_params, .int_data = CONFIG_Y, .line = "set 2.124 parameters" },
	{ .key = 'S', .func_int = set_default_params, .int_data = CONFIG_S, .line = "set 1.40  parameters" },
	{ .key = 'R', .func_int = set_default_params, .int_data = CONFIG_R, .line = "set 2.40  parameters" },
	{ .key = '1', .func_int = set_default_params, .int_data = CONFIG_1, .line = "set 2.101 parameters" },
	{ .key = '2', .func_int = set_default_params, .int_data = CONFIG_2, .line = "set 2.102 parameters" },
	{ .key = 'A', .func_int = set_default_params, .int_data = CONFIG_A, .line = "set 2.91  parameters" },
	{ .key = 'B', .func_int = set_default_params, .int_data = CONFIG_B, .line = "set 2.92  parameters" },
	{ .key = 'C', .func_int = set_default_params, .int_data = CONFIG_C, .line = "set 2.93  parameters" },
	{ .key = 'X', .line = "exit menu", .func_int  = menu_exit },
	{ .key = '\0' }
    };

    menu_do_all("Pre-defined parameters Menu", PrintAllParam, menu);
}

void SetAllParam(void)
{
    static menu_entry_t menu[] =
    {
	{
	    .key = 'P', .line = "Pre-defined parameters menu",
	    .func_void = predefined_menu,
	},

	{
	    .key = 'M', .line = "Local MAC address",
	    .func_void = Set_Mac,
	    .print_val = (menu_func_void_t)PrintMac
	},

	{
	    .key = 'I', .line = "Local IP  address",
	    .func_void = Set_IP,
	    .print_val = (menu_func_void_t)PrintIp
	},

	{
	    .key = 'T', .line = "TFTP server IP address",
	    .func_void = set_tftp_ip,
	    .print_val = (menu_func_void_t)PrintTftpIP
	},

	{
	    .key = 'G', .line = "TFTP server gateway address",
	    .func_void = set_tftp_gw,
	    .print_val = (menu_func_void_t)PrintTftpGW
	},

	{
	    .key = 'B', .line = "Bootloader file name",
	    .func_void = set_tftp_boot_name,
	    .print_val = (menu_func_void_t)PrintBootName
	},

	{
	    .key = 'S', .line = "OS file name",
	    .func_void = set_tftp_linux_name,
	    .print_val = (menu_func_void_t)PrintLinuxName
	},

	{
	    .key = 'E', .line = "Expert mode",
	    .func_void = SetExpertMode,
	    .print_val = (menu_func_void_t)PrintExpertMode
	},

	{ .key = 'X', .line = "Save parameters and exit menu", .func_int  = save_exit },
	{ .key = '\0' }
  };

  menu_do_all("Parameters Menu", NULL, menu);
}

int check_ip (UINT32 ipcheck, int flag)
{
	UINT8 reg0, reg1, reg2, reg3;
	if (flag == HOST_IP_FLAG)
	{
		if (ipcheck == 0)
			return 1;
	}

	if (flag == NETMASK_FLAG)
	{
		if (ipcheck == 0)
			return 1;

		reg3 = (ipcheck >> 24) & 0xff;
		reg2 = (ipcheck >> 16) & 0xff;
		reg1 = (ipcheck >> 8) & 0xff;
		reg0 = (ipcheck) & 0xff;
		if (reg3 < reg2 || reg2 < reg1 || reg1 < reg0)
			return 1;
	}

	return 0;
}

