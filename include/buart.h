#if __cplusplus
extern "C"
{
#endif

#ifndef _B_UART_H_
#define _B_UART_H_

	void buart_init (void);
	void buart_set_baud (int baud_rate);
	void buart_print (const char *str);
	void buart_put (char c);
	int  buart_get (int timeout);
	char buart_getchar(void);
	void ReadLine (char *buf, int num);

#endif

#if __cplusplus
}
#endif
