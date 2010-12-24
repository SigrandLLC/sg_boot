#include <ctype.h>
#include <string.h> // memset
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <except.h>
#include <linuxld.h>
#include <memlib.h>

/*void *MemAlloc (UINT32 NumOfByte, UINT32 clear);
void *SharedMemAlloc (UINT32 NumOfByte, UINT32 clear);*/


/*********  Global Variables ********/
#define MEM_ALIGN				4
#define SHARED_MEM_ALIGN		16

#define MEM_HEAP_PAEND			LINUXLD_MEMPOOL_START+LINUXLD_MEMPOOL_SIZE


void *_heap_h;

UINT32 AvilMemHead;


/**********************************************************************************/
/* MemAlloc:																	  */

/**********************************************************************************/
void *MemAlloc (UINT32 NumOfByte, UINT32 clear)
{
	int s;
	UINT32 MemStart, MemEnd;

	/*char buf[10] = {0}; static UINT32 totallallocated = 0;
	totallallocated += NumOfByte;
	buart_print ("\n\rCall to MemAlloc for 0x");
	ultoa (NumOfByte, buf);
	buart_print (buf);
	buart_print (" bytes. Total allocated 0x");
	ultoa (totallallocated, buf);
	buart_print (buf);
	buart_print (" bytes.");*/

	// Critical section starts. Disable the interrupts
	s = mips_int_lock ();

	MemEnd = AvilMemHead + NumOfByte;

	if (MemEnd & (MEM_ALIGN - 1))
		MemEnd = (MemEnd + MEM_ALIGN - 1) & ~(MEM_ALIGN - 1);

	if (MemEnd >= MEM_HEAP_PAEND)
	{
		mips_int_unlock (s);
		return NULL;
	}

	MemStart = AvilMemHead;
	AvilMemHead = MemEnd;

	// Critical section ends. Restore the interrupts mode
	mips_int_unlock (s);

	MemStart = MIPS_KSEG0A (PA2VA (MemStart));

#ifndef RTL_SIMULATION
	if (clear)
	{
		memset ((void *) MemStart, 0, NumOfByte);
	}
#endif

	return (void *) MemStart;
}



/**********************************************************************************/
/* SharedMemAlloc:																  */

/**********************************************************************************/
void *SharedMemAlloc (UINT32 NumOfByte, UINT32 clear)
{
	int s;
	UINT32 MemStart, MemEnd;


	// Critical section starts. Disable the interrupts
	s = mips_int_lock ();

	// Align the start address to cache line boundry
	if (AvilMemHead & (SHARED_MEM_ALIGN - 1))
		AvilMemHead = (AvilMemHead + SHARED_MEM_ALIGN - 1) & ~(SHARED_MEM_ALIGN - 1);

	MemEnd = AvilMemHead + NumOfByte;

	// !! Always keep the shared memory block aligned on cache line boundry
	if (MemEnd & (SHARED_MEM_ALIGN - 1))
		MemEnd = (MemEnd + SHARED_MEM_ALIGN - 1) & ~(SHARED_MEM_ALIGN - 1);

	if (MemEnd >= MEM_HEAP_PAEND)
	{
		mips_int_unlock (s);
		return NULL;
	}

	MemStart = AvilMemHead;
	AvilMemHead = MemEnd;

	// Critical section ends. Restore the interrupts mode
	mips_int_unlock (s);

	MemStart = MIPS_KSEG1A (PA2VA (MemStart));

#ifndef RTL_SIMULATION
	if (clear)
	{
		memset ((void *) MemStart, 0, NumOfByte);
	}
#endif

	return (void *) MemStart;
}

voidpf zAlloc (voidpf opaque, uInt items, uInt size)
{
	(void)opaque;
	return MemAlloc (items * size, 1);
}

void zFree (voidpf opaque, voidpf address)
{
	(void)opaque;
	(void)address;
	return;
}

/****************************************************************************/
/*  memlib_init:															*/

/****************************************************************************/
void memlib_init (void)
{
	int s;
	_heap_h = (void *) LINUXLD_MEMPOOL_START;
	s = mips_int_lock ();

	// map VA to PA
	AvilMemHead = KVA2PA ((UINT32) _heap_h);

	if (AvilMemHead & (MEM_ALIGN - 1))
	{
		AvilMemHead = (AvilMemHead + MEM_ALIGN - 1) & ~(MEM_ALIGN - 1);
	}

	mips_int_unlock (s);
}
