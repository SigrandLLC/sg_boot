BYTE_ORDER = LITTLE_ENDIAN
FLASH_TYPE = NAND_FLASH

# Эти значения должны соответствовать содержимому linuxld.h
#LOADER_OFFSET = 0x80002000
#RUNTIME_OFFSET = 0x80003000
LOADER_OFFSET = 0x80800000
RUNTIME_OFFSET = 0x80801000

CROSS_TOOLS = /work/mipsisa32-elf-old/bin/mipsisa32-elf
AS	= $(CROSS_TOOLS)-as
CC	= $(CROSS_TOOLS)-gcc
LD	= $(CROSS_TOOLS)-ld
AR  = $(CROSS_TOOLS)-ar

RM	= rm
MV	= mv
CP	= mv
OBJ_TO_COPY = $(CROSS_TOOLS)-objcopy
OBJ_DUMP= $(CROSS_TOOLS)-objdump

CCBYTE_ORDER = little-endian
ENDIAN_FG = -EL
LIB_PATH = -L. -L./lib/el
EDIR = el

CC_FLAG 		= $(ENDIAN_FG) -Wcomment -O2
CPU_FLAG		= -mips32
INCLUDE_DIR = -I. -I./include

OBJ_DIR = ./build
BIN_DIR = ./bin
SRC_DIR = ./src
TFTPBOOT = /media/tftpboot/b

LD_FLAG = -X -N
LIBS = -lz -lc -lgcc -lnosys

BOOT_NAME = nand_bootinit
BOOT_NAME_RAM = nand_bootinit_ram
BOOT_OBJS = $(OBJ_DIR)/nand_bootinit.o
BOOT_OBJS_RAM = $(OBJ_DIR)/nand_bootinit_ram.o

EXEC_NAME = nand_bootmain
EXEC_OBJS = $(OBJ_DIR)/nand_ldrinit.o $(OBJ_DIR)/bloader.o $(OBJ_DIR)/linuxld.o\
		$(OBJ_DIR)/xmodem.o $(OBJ_DIR)/nand.o $(OBJ_DIR)/nand_ecc.o\
		$(OBJ_DIR)/cachelib.o $(OBJ_DIR)/dslam.o\
		$(OBJ_DIR)/irqlib.o $(OBJ_DIR)/timer.o\
		$(OBJ_DIR)/vector.o $(OBJ_DIR)/except.o $(OBJ_DIR)/utils.o\
		$(OBJ_DIR)/if_5120.o $(OBJ_DIR)/memlib.o\
		$(OBJ_DIR)/uartdrv.o $(OBJ_DIR)/tftp.o $(OBJ_DIR)/eth.o\
		$(OBJ_DIR)/skbuff.o $(OBJ_DIR)/arp.o $(OBJ_DIR)/ip.o\
		$(OBJ_DIR)/udp.o $(OBJ_DIR)/param.o $(OBJ_DIR)/nf.o

EXEC_NAME_RAM = nand_bootmain_ram
EXEC_OBJS_RAM = $(EXEC_OBJS)

ROM_NAME = nandloader
ROM_NAME_RAM = nandloader_ram


.PHONY : all rom_img rom_img_ram
all : rom_img rom_img_ram

rom_img: boot_img main_img
	cat $(OBJ_DIR)/$(BOOT_NAME).img $(OBJ_DIR)/$(EXEC_NAME).img > $(BIN_DIR)/$(ROM_NAME).img
	cp $(BIN_DIR)/$(ROM_NAME).img $(TFTPBOOT)

boot_img: $(BOOT_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext $(LOADER_OFFSET) \
			 	-Map $(OBJ_DIR)/$(BOOT_NAME).map -o $(OBJ_DIR)/$(BOOT_NAME).elf	\
				$(BOOT_OBJS) $(LIBS)
	$(OBJ_TO_COPY) -O binary $(OBJ_DIR)/$(BOOT_NAME).elf  $(OBJ_DIR)/$(BOOT_NAME).bin
	$(OBJ_TO_COPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME).bin \
				$(OBJ_DIR)/$(BOOT_NAME).img

main_img: $(EXEC_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext $(RUNTIME_OFFSET) \
			-Map $(OBJ_DIR)/$(EXEC_NAME).map -o $(OBJ_DIR)/$(EXEC_NAME).elf \
			$(EXEC_OBJS) $(LIBS)
	$(OBJ_TO_COPY) -O binary $(OBJ_DIR)/$(EXEC_NAME).elf  $(OBJ_DIR)/$(EXEC_NAME).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME).bin $(OBJ_DIR)/$(EXEC_NAME).img


rom_img_ram: boot_img_ram main_img_ram
	cat $(OBJ_DIR)/$(BOOT_NAME_RAM).img $(OBJ_DIR)/$(EXEC_NAME_RAM).img > $(BIN_DIR)/$(ROM_NAME_RAM).img

boot_img_ram: $(BOOT_OBJS_RAM)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext $(LOADER_OFFSET) \
			 	-Map $(OBJ_DIR)/$(BOOT_NAME_RAM).map -o $(OBJ_DIR)/$(BOOT_NAME_RAM).elf	\
				$(BOOT_OBJS_RAM) $(LIBS)
	$(OBJ_TO_COPY) -O binary $(OBJ_DIR)/$(BOOT_NAME_RAM).elf  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin
	$(OBJ_TO_COPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin \
				$(OBJ_DIR)/$(BOOT_NAME_RAM).img

main_img_ram: $(EXEC_OBJS_RAM)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext $(RUNTIME_OFFSET) \
			-Map $(OBJ_DIR)/$(EXEC_NAME_RAM).map -o $(OBJ_DIR)/$(EXEC_NAME_RAM).elf \
			$(EXEC_OBJS_RAM) $(LIBS)
	$(OBJ_TO_COPY) -O binary $(OBJ_DIR)/$(EXEC_NAME_RAM).elf  $(OBJ_DIR)/$(EXEC_NAME_RAM).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME_RAM).bin $(OBJ_DIR)/$(EXEC_NAME_RAM).img


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.S
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@


.PHONY : clean
clean:
	@$(RM) $(OBJ_DIR)/* $(BIN_DIR)/*
