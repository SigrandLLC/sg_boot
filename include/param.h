#define BSP_IFNAME_MAX_LEN	15
#define HOST_IP_FLAG		0
#define NETMASK_FLAG		1
#define GATEWAY_IP_FLAG		2
/******************************************************************************************/

void Set_Board_SerialNo (void);
void Set_Board_Version (void);
void Set_Mac (void);
void Set_If_config (void);
int check_ip (unsigned long, int);

void PrintAllParam (void);
void   SetAllParam (void);
int  set_boot_param (void);
void print_tftpc_param (void);
int  set_tftpc_param (void);
int  get_tftp_param (UINT32 *servip, UINT32 *gwip, char *servfile, int mode);
int  boot_param_init (void);
int  bsp_GetMacBase (UINT8 *buf, int *macnum);
int  bsp_GetTftpIp  (UINT32 *tftpip);
int  bsp_GetGwIp (UINT32 *gwip);

