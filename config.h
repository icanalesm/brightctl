/* ACPI backlight devices */
static struct device devs[] = {
	/* alias     class         name */
	{ "screen",  "backlight",  "acpi_video0" },
	{ "kbd",     "leds",       "smc::kbd_backlight" },
};
