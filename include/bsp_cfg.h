#ifndef __BSP_CFG_H__
#define __BSP_CFG_H__

#define BSP_STR_LEN		79
#define BSP_MAX_MAC_NUM		8
#define BSP_MAX_IF_CNT		BSP_MAX_MAC_NUM
#define BSP_FILENAME_STR_LEN	255

typedef struct IF_CFG_S
{
	char ifname[BSP_IFNAME_MAX_LEN + 1];
	UINT32 ip;
	UINT32 mask;
	UINT32 gateway;
} IF_CFG_T;

typedef struct BTMODE_CFG_S
{
	UINT32 btmode;
	UINT32 dlmethod;
} BTMODE_CFG_T;

typedef struct TFTP_CFG_S
{
	UINT32 local_ip;
	UINT32 server_ip;
	UINT32 gw_ip;
	char linux_name     [BSP_FILENAME_STR_LEN + 1];
	char bootloader_name[BSP_FILENAME_STR_LEN + 1];
} TFTP_CFG_T;

typedef struct BOARD_CFG_S
{
	UINT32 blmagic;
	char bootline[BOOT_LINE_SIZE + 1];
	UINT32 macmagic;
	UINT8 mac[8];
	UINT32 macnum;

	UINT32 idmagic;
	UINT8 serial[BSP_STR_LEN + 1];

	UINT32 vermagic;
	UINT8 ver[BSP_STR_LEN + 1];

	UINT32 ifmagic;
	IF_CFG_T ifcfg[8];

	UINT32 btmagic;
	BTMODE_CFG_T bootmode;

	UINT32 tftploipmagic;
	UINT32 tftpmagic;
	TFTP_CFG_T tftp_param;

}
BOARD_CFG_T;

#define BL_MAGIC			0x6c62676d
#define MAC_MAGIC			0x636d676d
#define VER_MAGIC			0x7276676d
#define ID_MAGIC			0x6469676d
#define IF_MAGIC			0x6669676d

#define BT_MAGIC			0x6c74676d

#define TFTPMAGIC			0x6c76676d
#define TFTPLOIPMAGIC			0x6c76676c

#endif

