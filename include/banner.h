#define	CPU_NO				"5120"
#define	CPU_CLOCK			"175MHz"
#if SDRAM_BANK0_SIZE == 4
	#define	SDRAM_SIZE		"8MB"
#elif SDRAM_BANK0_SIZE == 8
	#define	SDRAM_SIZE		"16MB"
#elif SDRAM_BANK0_SIZE == 16
	#define	SDRAM_SIZE		"32MB"
#elif SDRAM_BANK0_SIZE == 32
	#define	SDRAM_SIZE		"64MB"
#elif SDRAM_BANK0_SIZE == 64
	#define	SDRAM_SIZE		"128MB"
#elif SDRAM_BANK0_SIZE == 128
	#define	SDRAM_SIZE		"64MB"
#elif SDRAM_BANK0_SIZE == 256
	#define	SDRAM_SIZE		"512MB"
#endif
#define	FLASH_TYPE			"NAND"
#ifdef K9F5608U0B
	#define	FLASH_SIZE		"32MB"
#endif
#ifdef K9F6408U0C
	#define	FLASH_SIZE		"8MB"
#endif
#define	BOOT_SYSTEM			"Linux-5120"

