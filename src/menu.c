#include <ctype.h>
#include <uartdrv.h>
#include <menu.h>

void menu_print(const menu_entry_t *menu)
{
	for ( ; menu->line != NULL; ++menu)
	{
		buart_print("\n\r [");
		buart_put(menu->key0);
		buart_print("] ");
		buart_print(menu->line);
	}
}

int menu_call(const menu_entry_t *menu)
{
	char c = buart_getchar ();
	buart_put (c);

	for ( ; menu->line != NULL; ++menu)
	{
		if (c == menu->key0 || c == menu->key1)
		{
			if (menu->func_void != NULL)
				menu->func_void();

			if (menu->func_int != NULL)
				menu->func_int(menu->int_data);
		}
	}

	return -1;
}

