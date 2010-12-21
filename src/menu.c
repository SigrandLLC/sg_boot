#include <ctype.h>
#include <uartdrv.h>
#include <menu.h>

extern inline char toupper(char c)
{
    return c >= 'a' && c <= 'z' ? c - 32 : c;
}

void menu_print(const menu_entry_t *menu)
{
	for ( ; menu->line != NULL; ++menu)
	{
		buart_print("\n\r [");
		buart_put(menu->key);
		buart_print("] ");
		buart_print(menu->line);
	}
}

int menu_call(const menu_entry_t *menu)
{
	char c = buart_getchar ();
	buart_put (c);
	c = toupper(c);

	for ( ; menu->key != '\0'; ++menu)
	{
		if (toupper(menu->key) == c)
		{
			if (menu->func_void != NULL)
				menu->func_void();

			if (menu->func_int != NULL)
				menu->func_int(menu->int_data);
		}
	}

	return -1;
}

