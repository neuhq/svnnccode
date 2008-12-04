#ifndef	TIAN_ERROR_H
#define	TIAN_ERROR_H

/* Nonfatal error related to a system call.
 * Print a message and return. */

#define	ERR_RET(s, ...) do{err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_ret(s, ##__VA_ARGS__);}while(0)
#define	ERR_SYS(s, ...) do{err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_sys(s, ##__VA_ARGS__);}while(0)
#define	ERR_DUMP(s, ...) do{err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_dump(s, ##__VA_ARGS__);}while(0)
#define	ERR_MSG(s, ...) do{err_msg("FILE %s FUNC %s LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_msg(s, ##__VA_ARGS__);}while(0)
#define	ERR_QUIT(s, ...) do{err_msg("FILE %s FUNC %s LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_quit(s, ##__VA_ARGS__);}while(0)
void
err_ret(const char *fmt, ...);

/* Fatal error related to a system call.
 * Print a message and terminate. */

void
err_sys(const char *fmt, ...);

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */

void
err_dump(const char *fmt, ...);

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */

void
err_msg(const char *fmt, ...);

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */

void
err_quit(const char *fmt, ...);

/* Print a message and return to caller.
 * Caller specifies "errnoflag". */


#endif
