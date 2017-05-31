#ifndef __NF_H__
#define __NF_H__

int nf_erase     (UINT8 *addr,                   UINT32 len, UINT8 wp);
int nf_write     (UINT8 *dst , const UINT8 *src, UINT32 len, UINT8 wp);
int nf_read      (UINT8 *dst , const UINT8 *src, UINT32 len);
int nf_write_boot(UINT8 *dst , const UINT8 *src, UINT32 len);

#endif

