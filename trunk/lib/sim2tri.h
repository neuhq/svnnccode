#ifndef	__SVNNCCODE_SIM2TRI__
#define __SVNNCCODE_SIM2TRI__

int stinit(const char* path);
int s2t(const char* s, char* t);
int t2s(const char* s, char* t);
int file_s2t(const char* spath, const char* tpath);
int file_t2s(const char* spath, const char* tpath);
inline int utf8_length(const char* s);

#endif
