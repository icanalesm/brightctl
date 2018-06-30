include config.mk

SRC = brightctl.c

OBJ = ${SRC:.c=.o}

all: options brightctl

options:
	@echo brightctl build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CC     = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk config.h

brightctl: ${OBJ}
	${CC} -o $@ ${OBJ}

clean:
	rm -f brightctl ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f brightctl ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/brightctl

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/brightctl

.PHONY: all options clean install uninstall
