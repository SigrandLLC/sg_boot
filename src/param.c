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
/*****************************************************************************************/
void Set_Board_SerialNo (void);
void Set_Board_Version (void);
void Set_Mac (void);
void Set_TFTP_IP (void);

/*****************************************************************************************/
// lnitial boot parameter
static BOARD_CFG_T *cfg;

/*****************************************************************************************/
/*		Parameter Init																	 */

/*****************************************************************************************/
int boot_param_init (void)
{
	int err = -1;
	// Read the Network configuration data.
	cfg = (BOARD_CFG_T *) MemAlloc (sizeof (BOARD_CFG_T), TRUE);
	if (cfg != NULL)
		err = nf_read ((char *) cfg, (char *) LINUXLD_NANDFLASH_BOOTPARAM_START, sizeof (BOARD_CFG_T));
	return err;
}

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
	Set_TFTP_IP ();

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

	return 0;
	/* Wait any key */
	//buart_print("\n\r\n\rPlease press any key to continue...");
	//buart_getchar();
}

int bsp_SetMac (UINT8 *mac, int macnum)
{
	int i;
	UINT32 sum;

	if ((mac == NULL) || (macnum < 1) || (macnum > BSP_MAX_MAC_NUM))
		return -1;

	// Allow only unicast mac address
	if (mac[0] != 0) return -1;

	cfg->macmagic = MAC_MAGIC;

	// have to use memcpy here.
	memcpy (cfg->mac, mac, 6);
	cfg->mac[7] = cfg->mac[8] = 0;
	cfg->macnum = macnum;

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

void Set_Mac (void)
{
	char buf[BOOT_LINE_SIZE + 1];
	char mac[8];
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
			memcpy (mactmp, cfg->mac, 6);
			ProgramMac (0, mactmp); // Change current MAC addr.
			eth_reinit ();
			buart_print ("\n\rMAC address updated successfully.");
		}
	} else
	{
		buart_print ("\n\rMAC address unchanged.");
	}
}

void Set_TFTP_IP (void)
{
	UINT32 loip;
	char str[] = "xxx.xxx.xxx.xxx";
	char buf[BOOT_LINE_SIZE + 1];
	if (cfg->tftploipmagic == TFTPLOIPMAGIC)
	{
		buart_print ("\n\rIP address: ");
		IpAddrToStr (cfg->tftp_param.local_ip, str);
		buart_print (str);
	}
loip_again:
	buf[0] = 0;
	buart_print ("\n\rEnter new IP address: ");
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

void PrintBspParam (void)
{
	UINT32 tftpip;
	unsigned char buf[BOOT_LINE_SIZE + 1];
	unsigned char mactmp[] = "00-00-00-00-00-00-";
	unsigned char ipstr[] = "xxx.xxx.xxx.xxx";
	int macnum;

	/* Print Item */
	buart_print ("\n\n\rPrint Boot Parameters.");
	buart_print ("\n\r==========================\n\r");

	/* Print Mac address */
	if (bsp_GetMacBase (buf, &macnum) != 0)
	{
		buart_print ("\n\rMAC address: ");
#ifndef NO_NUMBER_OF_MACS
		buart_print ("\n\rNumber of MAC addresses: 0");
#endif
	} else
	{
		mactostr (buf, mactmp);
		mactmp[17] = '\0';
		buart_print ("\n\rMAC address: ");
		buart_print (mactmp);
#ifndef NO_NUMBER_OF_MACS
		buart_print ("\n\rNumber of MAC addresses: ");
		buart_put (macnum + '0');
#endif
	}

	/* Print IP for TFTP Download */
	if (bsp_GetTftpIp (&tftpip) != 0)
	{
		ipstr[0] = 0;
	} else
		IpAddrToStr (tftpip, ipstr);
	buart_print ("\n\rIP address: ");
	buart_print (ipstr);
}

void print_tftpc_param (void)
{
	char str[] = "xxx.xxx.xxx.xxx";
	if (cfg->tftpmagic == TFTPMAGIC)
	{
		buart_print ("\n\n\rTFTP Server IP address: ");
		IpAddrToStr (cfg->tftp_param.server_ip, str);
		buart_print (str);
		buart_print ("\n\rTFTP Server Gateway IP address: ");
		IpAddrToStr (cfg->tftp_param.gw_ip, str);
		buart_print (str);
		buart_print ("\n\rRemote bootloader file name: ");
		buart_print (cfg->tftp_param.bootloader_name);
		buart_print ("\n\rRemote Linux file name: ");
		buart_print (cfg->tftp_param.linux_name);
		buart_print ("\n\r");
	}
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

int set_tftpc_param (void)
{
	UINT32 servip, gwip;
	char buf[BSP_FILENAME_STR_LEN + 1];
	char *image = (char *) LINUXLD_DOWNLOAD_START;

servip_again:
	buf[0] = 0;
	buart_print ("\n\r\n\rEnter TFTP Server IP address: ");
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
		cfg->tftp_param.server_ip = servip;
	} else
	{
		buart_print ("TFTP Server IP address unchanged.\n\r");
	}
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
		cfg->tftp_param.gw_ip = gwip;
	} else
	{
		buart_print ("TFTP server gateway IP address unchanged.\n\r");
	}

	buf[0] = 0;
	buart_print ("Enter Remote bootloader file name: ");
	ReadLine (buf, sizeof(buf)-1);
	if (buf[0] != 0)
		strncpy (cfg->tftp_param.bootloader_name, buf,
			 sizeof(cfg->tftp_param.bootloader_name)-1);
	else
		buart_print ("Bootloader file name unchanged.\n\r");

	buf[0] = 0;
	buart_print ("Enter Remote Linux file name: ");
	ReadLine (buf, sizeof(buf)-1);
	if (buf[0] != 0)
		strncpy (cfg->tftp_param.linux_name, buf,
			 sizeof(cfg->tftp_param.linux_name)-1);
	else
		buart_print ("Linux file name unchanged.\n\r");

	cfg->tftpmagic = TFTPMAGIC;

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

	buart_print ("\n\r");
	return 0;
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

