#ifndef __CONSOLE_H
#define __CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int console_init(void);
void console_putc(char c);
char console_getc(void);
void console_puts(char * str);
void console_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __CONSOLE_H */
