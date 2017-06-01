#include <ctype.h>
#include <adm5120.h>
#include <mips4kc.h>
#include <adm5120sw.h>
#include <param.h>
#include <buart.h>
#include <utils.h>
#include <string.h>

struct dslam_env
{
	unsigned long io_outen;
	unsigned long io_imask;
	unsigned long io_omask;
	unsigned long c_outen;
	unsigned long c_imask;
	unsigned long c_omask;
	unsigned long regnum;
};

static struct dslam_env chips[] = {
	{ GPIO6_OUTPUT_EN, GPIO6_INPUT_MASK, GPIO6_OUTPUT_HI, GPIO7_OUTPUT_EN, GPIO7_INPUT_MODE, GPIO7_OUTPUT_HI, 0xf2},
	{ GPIO1_OUTPUT_EN, GPIO1_INPUT_MASK, GPIO1_OUTPUT_HI, GPIO0_OUTPUT_EN, GPIO0_INPUT_MODE, GPIO0_OUTPUT_HI, 0xf2}
};

#define chips_num (sizeof(chips)/sizeof(struct dslam_env))

static unsigned long ic_readpreamble = 0x58;
static unsigned char ic_readpreamble_sz = 7; // 7 bit wide
static unsigned long ic_writepreamble = 0x54;
static unsigned char ic_writepreamble_sz = 7; // 7 bit wide
static unsigned long ic_pause = 0x2;
static unsigned char ic_pause_sz = 2; // 2 bit wide

void udelay (UINT32 us)
{
	volatile UINT32 j;
	for (j = 0; j < 2000 * us; j++);
}

static inline void enable_bits (unsigned long bits)
{
	ADM5120_SW_REG (GPIO_conf0_REG) |= bits;
}

static inline void disable_bits (unsigned long bits)
{
	ADM5120_SW_REG (GPIO_conf0_REG) &= (~bits);
}

static inline unsigned char read_bit (unsigned long bit)
{
	if (ADM5120_SW_REG (GPIO_conf0_REG) & bit)
		return 1;
	else
		return 0;
}

static inline void dslam_write_mode (struct dslam_env *env)
{
	// enable write on GPIO: MDIO=1(write mode) MDC=1 (write mode)
	enable_bits (env->io_outen | env-> io_omask | env->c_outen | env->c_omask);
}

static inline void dslam_read_mode (struct dslam_env *env)
{
	// enable read on GPIO: MDIO (read mode)
	disable_bits (env->io_outen);
	enable_bits (env->c_outen | env->c_omask);
}

static inline void dslam_empty_clock (struct dslam_env *env)
{
	// Empty clock ticks between read & write
	disable_bits (env->c_omask);
	udelay (1);
	enable_bits (env-> c_omask);
	udelay (1);
	disable_bits (env->c_omask);
	udelay (1);
	enable_bits (env-> c_omask);
	udelay (1);
}

static void dslam_write_bits (struct dslam_env *env, unsigned short outdata, unsigned char outbits)
{
	int i;
	udelay (1);
	for (i = outbits - 1; i >= 0; i--)
	{
		if ((outdata >> i) & 0x1)
		{
			enable_bits (env->io_omask);
		} else
		{
			disable_bits (env->io_omask);
		}
		disable_bits (env->c_omask);
		udelay (1);
		enable_bits (env-> c_omask);
		udelay (1);
	}
}

static void dslam_read_bits (struct dslam_env *env, unsigned long *indata, unsigned char inbits)
{
	int i;
	unsigned long in = 0;
	udelay (1);
	for (i = 0; i < inbits; i++)
	{
		disable_bits (env->c_omask);
		udelay (1);
		in |= read_bit (env->io_imask);
		enable_bits (env-> c_omask);
		in <<= 1;
		udelay (1);
	}
	// remove last shift
	in >>= 1;
	*indata = in;
}

int write_reg (int sw_num, unsigned long reg_num, unsigned long reg_val)
{
	struct dslam_env *env;

	env = &chips[sw_num];

	/*char buf[10] = {0};
	buart_print ("\n\rWriting to 0x");
	ultoa (reg_num, buf);
	buart_print (buf);
	buart_print (" val: 0x");
	ultoa (reg_val, buf);
	buart_print (buf);*/

	dslam_write_mode (env);
	dslam_write_bits (env, ic_writepreamble, ic_writepreamble_sz);
	dslam_write_bits (env, reg_num, 8);
	dslam_write_bits (env, ic_pause, ic_pause_sz);
	dslam_write_bits (env, reg_val, 16);
	dslam_write_mode (env);

	/*buart_print ("; reading: 0x");
	ultoa (read_reg (sw_num, reg_num), buf);
	buart_print (buf);*/

	return 0;
}

unsigned long read_reg (int sw_num, unsigned long reg_num)
{
	struct dslam_env *env;
	unsigned long reg_val;

	env = &chips[sw_num];

	dslam_write_mode (env);
	dslam_write_bits (env, ic_readpreamble, ic_readpreamble_sz);
	dslam_write_bits (env, reg_num, 8);
	dslam_read_mode (env);
	dslam_empty_clock (env);
	dslam_read_bits (env, &reg_val, 16);
	dslam_write_mode (env);

	return reg_val;
}

static int write_reg_ver (int sw_num, unsigned long reg_num, unsigned long reg_val)
{
        UINT32 val;
	write_reg (sw_num, reg_num, reg_val);
	val = read_reg (sw_num, reg_num);
	if (val != reg_val)
	{
		char buf[10];
                memset(buf, 0, sizeof(buf));
		buart_print ("\n\rDSLAM register write verify failed: ");
		buart_print (" sw: 0x"  ); ultoa (sw_num , buf); buart_print (buf);
		buart_print (" reg: 0x" ); ultoa (reg_num, buf); buart_print (buf);
		buart_print (" val: 0x" ); ultoa (reg_val, buf); buart_print (buf);
		buart_print (" read: 0x"); ultoa (    val, buf); buart_print (buf);
                return -1;
	}
        return 0;
}

void store_default (UINT8 sw_num)
{
	UINT32 val;
	// udelay (10000);
	// this is need for poperly work ethernet modules
	write_reg_ver (sw_num, 0xfa, 0x0200);
	// trunk hash algorithm selection is source and dest
	write_reg_ver (sw_num, 0x40, 0x000c);

	write_reg_ver (sw_num, 0x3b, 0x0000);
	write_reg_ver (sw_num, 0x3c, 0x0000);
	write_reg_ver (sw_num, 0x01, 0x0432);
	write_reg_ver (sw_num, 0xff, 0x0080);

	val = read_reg (sw_num, 0x00);
	if (val == 0x1826) // IP1826D chip id
	    write_reg_ver (sw_num, 0xf9, 0x1038);
	else               // IP1826C chip id
	    write_reg_ver (sw_num, 0xf9, 0x1e38);

	write_reg_ver (sw_num, 0xd8, 0xffff);
	if (sw_num == 0)
		write_reg_ver (sw_num, 0xd9, 0x37ff);
	else
		write_reg_ver (sw_num, 0xd9, 0x17ff);

	// set addres aging time to minimal
	val = read_reg (sw_num, 0x44);
	write_reg_ver (sw_num, 0x44, val & 0xFC00);
	//	printk(KERN_NOTICE"default settings is set\n");
	//	printk(KERN_NOTICE"0xd8=%lx\n", read_reg(sw_num, 0xd8));
}

int IsDslamBoardPresent (void)
{
#if 1
	UINT32 val = read_reg (0, 0x01);
	if ( (val & 0x1f) == 0x12)
	{
		//buart_print ("\n\rDSLAM presented");
		return 1;
	}
        else
	{
		//buart_print ("\n\rDSLAM absent");
		return 0;
	}
#else
	int i, reg = 0;
	ADM5120_SW_REG (GPIO_conf0_REG) |= 0xC0C00000;
	udelay (1);
	for (i = 14; i >= 0; i--)
	{
		if ((0x0058 >> i) & 0x1)
		{
			ADM5120_SW_REG (GPIO_conf0_REG) |= 0x40000000;
		} else
		{
			ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x40000000;
		}
		ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x80000000;
		udelay (1);
		ADM5120_SW_REG (GPIO_conf0_REG) |= 0x80000000;
		udelay (1);
	}
	ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x00400000;
	ADM5120_SW_REG (GPIO_conf0_REG) |= 0x80800000;

	ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x80000000;
	udelay (1);
	ADM5120_SW_REG (GPIO_conf0_REG) |= 0x80000000;
	udelay (1);
	ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x80000000;
	udelay (1);
	ADM5120_SW_REG (GPIO_conf0_REG) |= 0x80000000;
	udelay (1);
	udelay (1);
	for (i = 0; i < 16; i++)
	{
		ADM5120_SW_REG (GPIO_conf0_REG) &= ~0x80000000;
		udelay (1);
		reg |= (ADM5120_SW_REG (GPIO_conf0_REG) & 0x00004000) ? 1 : 0;
		ADM5120_SW_REG (GPIO_conf0_REG) |= 0x80000000;
		reg <<= 1;
		udelay (1);
	}
	reg >>= 1;
	ADM5120_SW_REG (GPIO_conf0_REG) |= 0xC0C00000;

	if (reg == 0xffff)
		return 0; // No DSLAM board
	return 1; // DSLAM board present
#endif
}

static int SetupVLAN (int unit, unsigned long portmask)
{
	unsigned long reg, shiftcnt;

	if (unit < 0 || unit > 6)
		return -1;

	if (unit <= 3)
	{
		shiftcnt = 8 * unit;
		reg = ADM5120_SW_REG (VLAN_G1_REG) & ~(VLAN_PORT_MASK << shiftcnt);
		reg |= (portmask & VLAN_PORT_MASK) << shiftcnt;
		ADM5120_SW_REG (VLAN_G1_REG) = reg;
	} else
	{
		shiftcnt = 8 * (unit - 4);
		reg = ADM5120_SW_REG (VLAN_G2_REG) & ~(VLAN_PORT_MASK << shiftcnt);
		reg |= (portmask & VLAN_PORT_MASK) << shiftcnt;
		ADM5120_SW_REG (VLAN_G2_REG) = reg;
	}

	return 0;
}

int ProgramVlanMac (int vlan, UINT8 *Mac, int clear)
{
	unsigned long Reg0, Reg1;

	if (vlan < 0 || vlan >= MAX_VLAN_GROUP)
		return -1;

	Reg0 = (((unsigned char) Mac[1] << 24) | ((unsigned char) Mac[0] << 16)) | (vlan << SW_MAC_VLANID_SHIFT)
		| SW_MAC_WRITE | SW_MAC_VLANID_EN;

	if (!clear)
		Reg0 |= SW_MAC_AGE_VALID;

	Reg1 = ((unsigned char) Mac[5] << 24) | ((unsigned char) Mac[4] << 16) |
		((unsigned char) Mac[3] << 8) | (unsigned char) Mac[2];

	ADM5120_SW_REG (MAC_wt1_REG) = Reg1;
	ADM5120_SW_REG (MAC_wt0_REG) = Reg0;

	while (!(ADM5120_SW_REG (MAC_wt0_REG) & SW_MAC_WRITE_DONE));

	return 0;
}

void store_vlan2port(void)
{
	UINT32 vlanGrp = 0xff;
	UINT32 vlan_val = 0;
	int macnum = 0;
	UINT8 mac[8] = {0};

	bsp_GetMacBase (mac, &macnum);

	vlan_val |= 1;
	vlan_val |= 1 << 1;
	vlan_val |= 1 << 2;
	vlan_val |= 1 << 3;
	vlan_val |= 1 << 4;
	vlan_val |= 1 << 5;
	vlanGrp &= 1 << CPU_PORT;
	vlanGrp |= vlan_val & 0x7F;

	// Apply changes
	SetupVLAN (0, vlanGrp);
	ProgramVlanMac (0, mac, SW_MAC_AGE_VALID);
}
