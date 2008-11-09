#ifndef	__SVNNCCODE_SIM2TRI__
#define __SVNNCCODE_SIM2TRI__

int stinit(FILE* file);
int s2t(const char* s, char* t);
int t2s(const char* s, char* t);
int file_s2t(FILE* s, FILE* t);
int file_t2s(FILE* s, FILE* t);
inline int utf8_length(const char* s);

#endif
