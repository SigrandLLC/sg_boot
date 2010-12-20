#ifdef __cplusplus
extern "C" {
#endif

#ifndef	__MEMLIB_H__
#define	__MEMLIB_H__

#include <zlib.h>

extern void *MemAlloc(UINT32 NumOfByte, UINT32 clear);
extern void *SharedMemAlloc(UINT32 NumOfByte, UINT32 clear);
voidpf zAlloc (voidpf opaque, uInt items, uInt size);
void zFree (voidpf opaque, voidpf address);

#endif

#ifdef __cplusplus
}
#endif
