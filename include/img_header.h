#if __cplusplus
extern "C"
{
#endif


#ifndef IMG_HEADER_H
#define IMG_HEADER_H

	typedef enum _march
	{
		_unknown = 0,
		_armv4,
		_mips32,
		_march_limit
	} march_t;

	enum IMAGE_TYPE_ID_E
	{
		ILLEGAL_IMAGE_TYPE,
		BOOT_IMAGE_TYPE,
		RUNTIME_IMAGE_TYPE,
		NV_IMAGE_TYPE,
		MAX_LOADER_TYPE
	};

#define FILE_CORRUPT				0x80


#define SKIP_HEADER_OFFSET			4

#define ARM_BRANCH_OPCODE			0xEA000000
#define ARM_BRANCH_OPCODE_MASK		0xFF000000
#define ARM_BRANCH_OFFSET_MASK		0xFFFFFF
	// #define SKIP_HEADER				0xea000006

#define MIPS_BRANCH_OPCODE			0x10000000
#define MIPS_BRANCH_OPCODE_MASK		0xFFFF0000
#define MIPS_BRANCH_OFFSET_MASK		0xFFFF

#define VENDOR_ID					ADMTEK
#define PRODUCT_ID					1
#define HEADER_VER					"002"

	typedef struct
	{
		unsigned long start0;
		unsigned long start1;
		march_t arch_id;
		char version[4];
		unsigned long type;
		unsigned long comp_file_len;
		unsigned long comp_file_sum;
		unsigned long header_sum;
	}
	IMAGE_HEADER;


#define IMGVFY_IGNORE_COOKIE		0x1
#define IMGVFY_IGNORE_SIG			0x2

	//<< WORKSPACE_INFO
#define COMPANY_NAME     "ADMtek Inc."
#define MODEL_NAME       "renaissance"
	//>> WORKSPACE_INFO

	/******************************
	  Function declaration
	 ******************************/
	int image_verify (char *buf, int *type, int flags);
	int imagefile_verify (char *pFilename, int *ilen, int *type, int flags);
	int img_head (char *head, int headlen, int type, char *img, int imglen);
	int img_cookie (char *cookie, int cookielen);
	int img_headlen (char *img);
	int imgfile_headlen (const char *fname);

#endif /* IMG_HEADER_H */

#if __cplusplus
}
#endif
