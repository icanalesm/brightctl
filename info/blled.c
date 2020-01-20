#include "info.h"
#include "../util.h"

#define BRIGHT_CUR_FMT    "/sys/class/%s/%s/brightness"
#define BRIGHT_MAX_FMT    "/sys/class/%s/%s/max_brightness"
#define BFR_MAX           128

int blled_getinfo(struct blled_info *info, const struct blled_dev *dev)
{
	int bright, bright_max;
	char bfr[BFR_MAX];

	if (esnprintf(bfr, sizeof(bfr), BRIGHT_CUR_FMT, dev->class, dev->name) < 0)
		return -1;
	if (epscanf(bfr, "%d", &bright) != 1)
		return -1;

	if (esnprintf(bfr, sizeof(bfr), BRIGHT_MAX_FMT, dev->class, dev->name) < 0)
		return -1;
	if (epscanf(bfr, "%d", &bright_max) != 1)
		return -1;

	info->bright = bright;
	info->bright_max = bright_max;

	return 0;
}
