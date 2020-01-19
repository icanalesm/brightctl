include config.mk

REQ =\
	info/blled\
	util\
	brightctl

all: options brightctl

options:
	@echo brightctl build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CC     = ${CC}"

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

brightctl: ${REQ:=.o}
	${CC} ${REQ:=.o} -o $@

clean:
	rm -f brightctl ${REQ:=.o}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f brightctl ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/brightctl

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/brightctl

.PHONY: all options clean install uninstall
