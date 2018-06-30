# brightctl

Utility for controlling brightness of `backlight` and `leds` devices through the ACPI interface (devices in `/sys/class`).

## Configuration

`brightctl` is configured via the `config.h` and `config.mk` files. `brightctl` must be re-compiled and re-installed when a new configuration is set.

### `config.h`

This file is where the devices controlled by `brightctl` are specified. The following information is required to specify a device:

* *alias*: nickname for the device. This alias is the identifier used to control the device with `brightctl`.
* *class*: class where the device is within `/sys/class`, i.e. `backlight` or `leds`.
* *name*: name of the device in `/sys/class/<class>`.

### `config.mk`

This file is where the compilation and installation options are specified. By default, `gcc` is used to compile and installation is done under `/usr/local`.

## Installation

```
git clone https://github.com/icanalesm/brightctl.git
cd brightctl
```

Set the configuration in `config.h` and `config.mk`.

Compile and install

```
make
sudo make install
```

`brightctl` controls brightness by writing to device files which most users do not have permissions to write to. One option to allow user *user* to execute `brightctl` is to configure `sudo` so that *user* is not required a password. This can be done by executing

```
sudo visudo -f /etc/sudoers.d/brightness
```

and adding the following line to this file

```
user ALL=NOPASSWD:/usr/local/bin/brightctl
```

replacing `/usr/local` if the configuration in `config.mk` is different.

## Usage

```
Usage:
  brightctl [command]

Commands:
  <alias> [subcommand]    Execute the given subcommand on the device corresponding to alias.
                          If no subcommand is given, show the current and maximum brightness.
  help                    Print this help.

Subcommands:
  [+|-]<value>[%]         If '+' or '-' are given, increase or decrease the current brightness
                            a) by value%% of maximum brightness, if %% is given, or
                            b) by value, otherwise.
                          If neither '+' nor '-' are given, set the current brightness to
                            a) value%% of maximum brightness, if %% is given, or
                            b) value, otherwise.
```

