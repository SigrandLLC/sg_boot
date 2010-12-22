#ifdef __cplusplus
extern "C" {
#endif

#ifndef	__MENU_H__
#define	__MENU_H__

enum MENU_RC { MENU_DONE = 0, MENU_EXIT, MENU_KEY_NOT_FOUND = -1 };
typedef enum MENU_RC menu_rc_t;

typedef void (*menu_func_void_t)(void);
typedef menu_rc_t (*menu_func_int_t)(int);

typedef struct
{
	const char *line;
	char key;

	menu_func_void_t func_void;

	int int_data;
	menu_func_int_t func_int;

	//void *data;
        //int (*func)(char key/*, void* data*/);
} menu_entry_t;

void      menu_print_header (const char *header);
void      menu_print_entries(const menu_entry_t *menu_array);
menu_rc_t menu_call(const menu_entry_t *menu_array);
menu_rc_t menu_exit(int);
void      menu_do_all(const char *header, menu_func_void_t func_header, const menu_entry_t *menu);


#endif

#ifdef __cplusplus
}
#endif
