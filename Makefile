# Set V=something for verbose building messages
#V = 1
v = $(if $(V),,@)

EXTRA_DEFINE = -DNO_NUMBER_OF_MACS

BYTE_ORDER = LITTLE_ENDIAN
FLASH_TYPE = NAND_FLASH

# Эти значения должны соответствовать содержимому linuxld.h
#LOADER_OFFSET = 0x80002000
#RUNTIME_OFFSET = 0x80003000
LOADER_OFFSET = 0x80800000
RUNTIME_OFFSET = 0x80801000

CROSS_TOOLS = $(HOME)/gnutools/mipsisa32-elf/bin/mipsisa32-elf
AS	= $(CROSS_TOOLS)-as
CC	= $(CROSS_TOOLS)-gcc
LD	= $(CROSS_TOOLS)-ld
AR	= $(CROSS_TOOLS)-ar
OBJCOPY	= $(CROSS_TOOLS)-objcopy
OBJDUMP	= $(CROSS_TOOLS)-objdump

RM	= rm $(if $(V),-v) -f
RM_R	= rm $(if $(V),-v) -fr
MV	= mv $(if $(V),-v)
CP	= mv $(if $(V),-v)
MKDIR   = mkdir $(if $(V),-v)
MKDIR_P = mkdir $(if $(V),-v) -p

ifeq "$(BYTE_ORDER)" "LITTLE_ENDIAN"
CCBYTE_ORDER = little-endian
ENDIAN_FG = -EL
LIB_PATH = -L./lib/el
EDIR = el
else
$(error BYTE_ORDER != LITTLE_ENDIAN)
endif

CC_FLAG 	= $(ENDIAN_FG) -Wcomment -O2 -Wall
CPU_FLAG	= -mips32
INCLUDE_DIR	= -I. -I./include

ALL_C_FLAGS	= $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE)

OBJ_DIR = ./build
BIN_DIR = ./bin
SRC_DIR = ./src
TFTPBOOT = $(HOME)/tftpboot

 OBJ_DIR_STAMP =  $(OBJ_DIR)/.dir
 BIN_DIR_STAMP =  $(BIN_DIR)/.dir
TFTPBOOT_STAMP = $(TFTPBOOT)/.dir

LD_FLAG = -X -N
LIBS = -lz -lc -lgcc -lnosys

BOOT_NAME     = nand_bootinit
BOOT_NAME_RAM = nand_bootinit_ram
BOOT_OBJS     = $(OBJ_DIR)/nand_bootinit.o
BOOT_OBJS_RAM = $(OBJ_DIR)/nand_bootinit_ram.o

EXEC_NAME = nand_bootmain

exec_objs  = nand_ldrinit.o bloader.o linuxld.o xmodem.o nand.o nand_ecc.o
exec_objs += cachelib.o dslam.o irqlib.o timer.o vector.o except.o utils.o
exec_objs += if_5120.o memlib.o uartdrv.o tftp.o eth.o skbuff.o arp.o ip.o
exec_objs += udp.o param.o nf.o menu.o

EXEC_OBJS = $(addprefix $(OBJ_DIR)/,$(exec_objs))

EXEC_NAME_RAM = nand_bootmain_ram
EXEC_OBJS_RAM = $(EXEC_OBJS)


ALL_OBJS = $(BOOT_OBJS) $(BOOT_OBJS_RAM) $(EXEC_OBJS)


ROM_NAME = nandloader
RAM_NAME = nandloader_ram


     ROM_IMG = $(BIN_DIR)/$(ROM_NAME).img
    BOOT_IMG = $(OBJ_DIR)/$(BOOT_NAME).img
    MAIN_IMG = $(OBJ_DIR)/$(EXEC_NAME).img

     RAM_IMG = $(BIN_DIR)/$(RAM_NAME).img
BOOT_RAM_IMG = $(OBJ_DIR)/$(BOOT_NAME_RAM).img
MAIN_RAM_IMG = $(OBJ_DIR)/$(EXEC_NAME_RAM).img


.PHONY  : all %install
all     : $(ROM_IMG) $(RAM_IMG)
install : rom_img_install ram_img_install


rom_img_install : $(BOOT_IMG) $(MAIN_IMG) $(ROM_IMG) $(TFTPBOOT_STAMP)
	@echo "> Copying $@ to $(TFTPBOOT)"
	$(v)$(CP) $(ROM_IMG) $(TFTPBOOT)

$(ROM_IMG) : $(BOOT_IMG) $(MAIN_IMG) $(BIN_DIR_STAMP)
	@echo "> Constructing $@"
	$(v)cat $(BOOT_IMG) $(MAIN_IMG) > $@

$(BOOT_IMG) : $(BOOT_OBJS) $(OBJ_DIR_STAMP)
	@echo "> Linking $@"
	$(v)$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext $(LOADER_OFFSET) \
			 	-Map $(OBJ_DIR)/$(BOOT_NAME).map -o $(OBJ_DIR)/$(BOOT_NAME).elf	\
				$(BOOT_OBJS) $(LIBS)
	$(v)$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME).elf  $(OBJ_DIR)/$(BOOT_NAME).bin
	$(v)$(OBJCOPY) -I binary -O binary --pad-to 0x1000    $(OBJ_DIR)/$(BOOT_NAME).bin $@

$(MAIN_IMG) : $(EXEC_OBJS) $(OBJ_DIR_STAMP)
	@echo "> Linking $@"
	$(v)$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext $(RUNTIME_OFFSET) \
			-Map $(OBJ_DIR)/$(EXEC_NAME).map -o $(OBJ_DIR)/$(EXEC_NAME).elf \
			$(EXEC_OBJS) $(LIBS)
	$(v)$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME).elf $@


ram_img_install : $(BOOT_RAM_IMG) $(MAIN_RAM_IMG) $(RAM_IMG) $(TFTPBOOT_STAMP)
	@echo "> Copying $@ to $(TFTPBOOT)"
	$(v)$(CP) $(RAM_IMG) $(TFTPBOOT)

$(RAM_IMG) : $(BOOT_RAM_IMG) $(MAIN_RAM_IMG) $(BIN_DIR_STAMP)
	@echo "> Constructing $@"
	$(v)cat $(BOOT_RAM_IMG) $(MAIN_RAM_IMG) > $(RAM_IMG)

$(BOOT_RAM_IMG) : $(BOOT_OBJS_RAM) $(OBJ_DIR_STAMP)
	@echo "> Linking $@"
	$(v)$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext $(LOADER_OFFSET) \
			 	-Map $(OBJ_DIR)/$(BOOT_NAME_RAM).map -o $(OBJ_DIR)/$(BOOT_NAME_RAM).elf	\
				$(BOOT_OBJS_RAM) $(LIBS)
	$(v)$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME_RAM).elf $(OBJ_DIR)/$(BOOT_NAME_RAM).bin
	$(v)$(OBJCOPY) -I binary -O binary --pad-to 0x1000       $(OBJ_DIR)/$(BOOT_NAME_RAM).bin $@

$(MAIN_RAM_IMG) : $(EXEC_OBJS_RAM) $(OBJ_DIR_STAMP)
	@echo "> Linking $@"
	$(v)$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext $(RUNTIME_OFFSET) \
			-Map $(OBJ_DIR)/$(EXEC_NAME_RAM).map -o $(OBJ_DIR)/$(EXEC_NAME_RAM).elf \
			$(EXEC_OBJS_RAM) $(LIBS)
	$(v)$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME_RAM).elf $@


%/.dir :
	@echo "> Creating directory $@"
	$(v)$(MKDIR_P)        $(dir $@)
	$(v)touch                   $@



$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(OBJ_DIR_STAMP)
	@echo "> Compiling $< to $@"
	$(v)$(CC) $(ALL_C_FLAGS)  -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.S $(OBJ_DIR_STAMP)
	@echo "> Compiling $< to $@"
	$(v)$(CC) $(ALL_C_FLAGS) -c $< -o $@


.PHONY : clean
clean:
	@echo "> Cleaning $(OBJ_DIR) $(BIN_DIR)"
	$(v)$(RM_R)       $(OBJ_DIR) $(BIN_DIR)


#+ dependencies generation
deps = $(patsubst %.o,%.dep,$(ALL_OBJS))
#$(warning deps: $(deps))

dep_gen_cmd_tail = -M $(ALL_C_FLAGS) -w $< > $@.tmp.dep \
	&& sed '\''s!$(subst .,\.,$*.o) *:!$(patsubst %.dep,%.o,$@) $(patsubst %.dep,%.lo,$@) $@ $(patsubst %.dep,%.s,$@):!g'\'' \
	   < $@.tmp.dep > $@.tmp2.dep \
	&& $(MV) $@.tmp2.dep $@ && $(RM) $@.tmp.dep; } \
	|| $(RM) $@ $@.tmp.dep $@.tmp.dep

  c_deps_gen_cmd = $(SHELL) -ec '{ $(CC)  -D__GENDEPS__ $(dep_gen_cmd_tail)'


$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.dep

$(OBJ_DIR)/%.dep: $(SRC_DIR)/%.c $(OBJ_DIR_STAMP)
	@echo "> Generating dependencies for $<"; \
		$(c_deps_gen_cmd)

ifeq "$(deps)" ""
  DONT_INCLUDE_DEPS = defined
endif

ifneq "$(filter %clean dist% none,$(MAKECMDGOALS))" ""
  DONT_INCLUDE_DEPS = defined
endif

ifndef DONT_INCLUDE_DEPS
  -include $(deps)
endif
#- dependencies generation

