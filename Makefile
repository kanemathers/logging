PROJECT=logging
SOURCES=src/list.c \
		src/logging.c \
		src/ini.c \
		src/config.c \
		src/handler_console.c \
		src/handler_syslog.c \
		src/handler_file.c
INCFLAGS=-I$(shell pwd)/include
CFLAGS=-Wall \
		-Wextra \
		-Werror \
		-Wno-unused-parameter \
		-pedantic
OFILES=$(SOURCES:.c=.o)
CC=gcc
prefix=/usr

.PHONY: clean logging

logging: $(OFILES)
	ar rcs lib$(PROJECT).a $(OFILES)

	$(CC) -shared $(OFILES) -o lib$(PROJECT).so

install: logging
	install lib$(PROJECT)* $(prefix)/lib
	cp -r ./include/* $(prefix)/include

.c.o:
	$(CC) $(INCFLAGS) -c $(CFLAGS) -fPIC $< -o $@

clean:
	rm -rf $(OFILES) liblogging*
