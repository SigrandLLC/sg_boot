#ifdef __cplusplus
extern "C" {
#endif

#ifndef	__MENU_H__
#define	__MENU_H__

typedef void (*func_void_t)(void);

typedef struct
{
	const char *line;
	char key;

	func_void_t func_void;

        int int_data;
        int (*func_int )(int );

	//void *data;
        //int (*func)(char key/*, void* data*/);
} menu_entry_t;

void menu_print(const menu_entry_t *menu_array);
int  menu_call (const menu_entry_t *menu_array);


#endif

#ifdef __cplusplus
}
#endif
