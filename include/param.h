#define PRINT_PARAM			"\n\n\rPrint Boot Parameters.\n\r==========================\n\r"
#define SET_PARAM			"\n\n\rSet Boot Parameters.\n\r==========================\n\r"
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
