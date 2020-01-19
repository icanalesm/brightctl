#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

void error(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "tstat: ");
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	va_end(va);
}

int esnprintf(char *buffer, size_t bfsz, const char *fmt, ...)
{
	va_list va;
	int ret;

	va_start(va, fmt);
	ret = vsnprintf(buffer, bfsz, fmt, va);
	va_end(va);
	
	if (ret < 0)
		error("esnprintf: %s", strerror(errno));
	else if ((size_t)ret >= bfsz)
		error("esnprintf: Output truncated");

	return ret;
}

int epscanf(const char *path, const char *fmt, ...)
{
	FILE *f;
	va_list va;
	int ret;

	if (!(f = fopen(path, "r"))) {
		error("epscanf '%s': %s", path, strerror(errno));
		return EOF;
	}

	va_start(va, fmt);
	ret = vfscanf(f, fmt, va);
	va_end(va);
	fclose(f);

	if (ret < 0)
		error("epscanf: Error while assigning values");

	return ret;
}

