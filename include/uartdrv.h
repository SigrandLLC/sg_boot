/*****************************************************************************
;
;    Project : Am5106/5107
;    Creator : David Weng
;    File    : uartdrv.h
;    Abstract:
;
;*****************************************************************************/


#ifndef  __UARTDRV_H__
#define  __UARTDRV_H__

#include <ctype.h>
#include <buart.h>

#define UART_PORT0			0
#define UART_PORT1			1


#define UART_NOT_INIT		0
#define UART_RUNNING		1

#ifndef BOOT_LINE_SIZE
#define BOOT_LINE_SIZE 31
#endif

typedef struct _UART_RATE_ENTRY_
{
	int rate;
	UINT32 div;
} UART_RATE_ENTRY, *PUART_RATE_ENTRY;

typedef struct _UART_OBJ_
{
	UINT32 status;
	UINT32 io_base;
	UINT32 ctrl;

	int baudrate;
} UART_OBJ, *PUART_OBJ;

#endif
