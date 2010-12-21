
typedef void (*mips_int_hdl)(int intnum,UINT32 parm0, UINT32 parm1);

int mips_int_connect(int intnum, mips_int_hdl hdl, UINT32 parm0, UINT32 parm1);
int mips_int_disconnect(int intnum);
int mips_int_enable(int intnum);
int mips_int_disable(int intnum);
int mips_int_lock(void);
void mips_int_unlock(int s);
void panic (void);
void install_exception (void);
