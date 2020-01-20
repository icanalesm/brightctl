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
