#include <stddef.h>

#define LEN(X)    (sizeof(X) / sizeof((X)[0]))                                          

void error(const char *fmt, ...);

int esnprintf(char *buffer, size_t bfsz, const char *fmt, ...);

int epscanf(const char *path, const char *fmt, ...);

