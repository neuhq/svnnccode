#ifndef	TIAN_ERROR_H
#define	TIAN_ERROR_H

/* Nonfatal error related to a system call.
 * Print a message and return. */

#define	ERR_RET(s, ...) err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_ret(s, ##__VA_ARGS__)
#define	ERR_SYS(s, ...) err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_SYS(s, ##__VA_ARGS__)
#define	ERR_DUMP(s, ...) err_msg("FILE %s FUNC %s() LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_DUMP(s, ##__VA_ARGS__)
#define	ERR_MSG(s, ...) err_msg("FILE %s FUNC %s LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_msg(s, ##__VA_ARGS__)
#define	ERR_QUIT(s, ...) err_msg("FILE %s FUNC %s LINE %i",\
	   							__FILE__, __func__, __LINE__);\
						err_QUIT(s, ##__VA_ARGS__)
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
