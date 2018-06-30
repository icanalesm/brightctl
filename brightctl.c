#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRIGHTCUR     "brightness"
#define BRIGHTMAX     "max_brightness"
#define DEVPATHFMT    "/sys/class/%s/%s/"
#define LENGTH(X)     (sizeof X / sizeof X[0])
#define MAX(A, B)     ((A) > (B) ? (A) : (B))
#define MIN(A, B)     ((A) < (B) ? (A) : (B))

struct device {
	char *alias;
	char *class;
	char *name;
};

static int get_brightness(long *b, const char *filename);
static int set_brightness(long b, const char *filename);
static int manage_brightness(const struct device *dev, const char *subcmd);
static void print_devs(const struct device *dev, size_t n);
static void print_usage();

#include "config.h"

int get_brightness(long *b, const char *filename)
{
	FILE *f;
	int aux;

	if (!(f = fopen(filename, "r")))
		return 0;
	aux = fscanf(f, "%ld", b);
	fclose(f);

	return (aux == EOF) ? 0 : aux;
}

int set_brightness(long b, const char *filename)
{
	FILE *f;
	int aux;

	if (!(f = fopen(filename, "w")))
		return -1;
	aux = fprintf(f, "%ld", b);
	fclose(f);

	return aux;
}

int manage_brightness(const struct device *dev, const char *subcmd)
{
	long bgt, max, val;
	char buf[128], *aux;

	/* Get max brightness */
	aux = buf + sprintf(buf, DEVPATHFMT, dev->class, dev->name);
	strcat(buf, BRIGHTMAX);
	if (!get_brightness(&max, buf)) {
		fprintf(stderr, "error: Cannot read brightness from '%s'\n", buf);
		return 0;
	}

	/* Get current brightness */
	*aux = '\0';
	strcat(buf, BRIGHTCUR);
	if (!get_brightness(&bgt, buf)) {
		fprintf(stderr, "error: Cannot read brightness from '%s'\n", buf);
		return 0;
	}

	/* No subcommand given, print current and maximum brightness */
	if (!subcmd) {
		fprintf(stdout, "current brightness: %ld\n", bgt);
		fprintf(stdout, "maximum brightness: %ld\n", max);
		return 1;
	}

	/* Parse subcommand */
	val = strtol(subcmd, &aux, 10);
	if (errno || aux == subcmd) {
		fprintf(stderr, "error: Invalid subcommand '%s'\n", subcmd);
		return 0;
	}

	/* Compute new brightness */
	if (*aux == '%')
		val = val * max / 100;
	switch (*subcmd) {
	case '+':
		bgt = MIN(max, bgt + val);
		break;
	case '-':
		bgt = MAX(0, bgt + val);
		break;
	default:
		bgt = MIN(max, val);
		break;
	}

	/* Set new brightness */
	if (!set_brightness(bgt, buf)) {
		fprintf(stderr, "error: Cannot set brightness in '%s'\n", buf);
		return 0;
	}

	return 1;
}

void print_devs(const struct device *dev, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		fprintf(stdout, "Device:\n\talias: %s\n\tdevice: "DEVPATHFMT"\n\n",
	       	        dev[i].alias, dev[i].class, dev[i].name);
}

void print_usage()
{
	puts(
	"Usage:\n"
	"  brightctl [command]\n"
	"\n"
	"Commands:\n"
	"  <alias> [subcommand]    Execute the given subcommand on the device corresponding to alias.\n"
	"                          If no subcommand is given, show the current and maximum brightness.\n"
	"  help                    Print this help.\n"
	"\n"
	"Subcommands:\n"
	"  [+|-]<value>[%]         If '+' or '-' are given, increase or decrease the current brightness\n"
	"                            a) by value%% of maximum brightness, if %% is given, or\n"
	"                            b) by value, otherwise.\n"
	"                          If neither '+' nor '-' are given, set the current brightness to\n"
	"                            a) value%% of maximum brightness, if %% is given, or\n"
	"                            b) value, otherwise.\n"
	);
}

int main(int argc, char *argv[])
{
	size_t ndevs = LENGTH(devs);
	size_t i;

	/* No commands given, print list of devices */
	if (argc == 1) {
		print_devs(devs, ndevs);
		exit(EXIT_SUCCESS);
	}

	/* If command is help, print usage */
	if (strcmp(argv[1], "help") == 0) {
		print_usage();
		exit(EXIT_SUCCESS);
	}

	/* Manage device */
	for (i = 0; i < ndevs; i++) {
		if (strcmp(argv[1], devs[i].alias) != 0)
			continue;
		if (manage_brightness(&devs[i], (argc == 2) ? NULL : argv[2]))
			exit(EXIT_SUCCESS);
		exit(EXIT_FAILURE);
	}

	/* The given device was not specified in the configuration */
	fprintf(stderr, "error: Device with alias '%s' not found in configuration\n", argv[1]);
	exit(EXIT_FAILURE);
}
