#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info/info.h"
#include "util.h"

#define MAX(A, B)    ((A) > (B) ? (A) : (B))
#define MIN(A, B)    ((A) < (B) ? (A) : (B))
#define BFR_MAX      128

static int bright_set(const struct blled_dev *dev, int brightness);
static int bright_manage(const struct blled_dev *dev, const char *subcmd);
static void print_devs(const struct blled_dev *dev, size_t n);
static void print_usage();

static char bfr[BFR_MAX];

#include "config.h"

int bright_set(const struct blled_dev *dev, int brightness)
{
	FILE *f;

	if (esnprintf(bfr, sizeof(bfr), BRIGHT_CUR_FMT, dev->class, dev->name) < 0)
		return -1;
	if (!(f = fopen(bfr, "w")))
		return -1;
	if (fprintf(f, "%d", brightness) < 0) {
		fclose(f);
		return -1;
	}

	fclose(f);

	return 0;
}

int bright_manage(const struct blled_dev *dev, const char *subcmd)
{
	int val;
	char *aux;
	struct blled_info info;

	if (blled_getinfo(&info, dev) != 0)
		return -1;

	/* No subcommand given, print current and maximum brightness */
	if (!subcmd) {
		fprintf(stdout, "current brightness: %d\n", info.bright);
		fprintf(stdout, "maximum brightness: %d\n", info.bright_max);
		return 0;
	}

	/* Parse subcommand */
	val = strtol(subcmd, &aux, 10);
	if (errno || aux == subcmd) {
		fprintf(stderr, "error: Invalid subcommand '%s'\n", subcmd);
		return -1;
	}

	/* Compute new brightness */
	if (*aux == '%')
		val = val * info.bright_max / 100;
	switch (*subcmd) {
	case '+':
		info.bright = MIN(info.bright_max, info.bright + val);
		break;
	case '-':
		info.bright = MAX(0, info.bright + val);
		break;
	default:
		info.bright = MIN(info.bright_max, val);
		break;
	}

	/* Set new brightness */
	if (bright_set(dev, info.bright) != 0) {
		fprintf(stderr, "error: Cannot set brightness in '%s'\n", bfr);
		return -1;
	}

	return 0;
}

void print_devs(const struct blled_dev *dev, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		fprintf(stdout,
		        "Device:\n\talias: %s\n\tdevice: "BRIGHT_DEV_FMT"\n\n",
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
	size_t ndevs = LEN(devs);
	size_t i;

	/* No commands given, print list of devices */
	if (argc == 1) {
		print_devs(devs, ndevs);
		return 0;
	}

	/* If command is help, print usage */
	if (strcmp(argv[1], "help") == 0) {
		print_usage();
		return 0;
	}

	/* Manage device */
	for (i = 0; i < ndevs; i++) {
		if (strcmp(argv[1], devs[i].alias) != 0)
			continue;
		if (bright_manage(&devs[i], (argc == 2) ? NULL : argv[2]) == 0)
			return 0;
		return 1;
	}

	/* The given device was not specified in the configuration */
	fprintf(stderr, "error: Device with alias '%s' not found in configuration\n", argv[1]);
	return 1;
}
