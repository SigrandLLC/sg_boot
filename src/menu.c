#include <ctype.h>
#include <uartdrv.h>
#include <menu.h>

extern inline char toupper(char c)
{
    return c >= 'a' && c <= 'z' ? c - 32 : c;
}

void menu_print_entries(const menu_entry_t *menu)
{
	for ( ; menu->line != NULL; ++menu)
	{
		buart_print("\n\r [");
		buart_put(menu->key);
		buart_print("] ");
		buart_print(menu->line);
		if (menu->print_val)
		{
			buart_print(" [ ");
			menu->print_val();
			buart_print(" ] ");
		}
	}
}

menu_rc_t menu_call(const menu_entry_t *menu)
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
			{
				menu_rc_t rc = menu->func_int(menu->int_data);
                                return rc;
			}

                        return MENU_DONE;
		}
	}

	return MENU_KEY_NOT_FOUND;
}

menu_rc_t menu_exit (int data)
{
        (void)data;
        return MENU_EXIT;
}

void menu_print_header(const char *header)
{
	buart_print ("\n\r");
	buart_print ("\n\r"); buart_print (header);
	buart_print ("\n\r=====================================");
}

void menu_do_all(const char *header, menu_func_void_t func_header, const menu_entry_t *menu)
{
	while (1)
	{
		menu_print_header(header);
		if (func_header) func_header();
		menu_print_entries(menu);
		buart_print("\n\rEnter your option: ");

		if (menu_call(menu) == MENU_EXIT)
			return;
	}
}

