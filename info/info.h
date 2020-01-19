#define BRIGHT_DEV_FMT    "/sys/class/%s/%s"
#define BRIGHT_CUR_FMT    "/sys/class/%s/%s/brightness"
#define BRIGHT_MAX_FMT    "/sys/class/%s/%s/max_brightness"

struct blled_info {
	int bright;
	int bright_max;
};

struct blled_dev {
	char *alias;
	char *class;
	char *name;
};

int blled_getinfo(struct blled_info *info, const struct blled_dev *dev);
