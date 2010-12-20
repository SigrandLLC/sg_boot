#if __cplusplus
extern "C"
{
#endif

#ifndef _B_UART_H_
#define _B_UART_H_

	void buart_init ();
	void buart_set_baud (int baud_rate);
	void buart_print (char *str);
	void buart_put (char c);
	int buart_get (int timeout);

#endif

#if __cplusplus
}
#endif
