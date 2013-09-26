export CC 		:= gcc
export CFLAGS 	:= -I$(shell pwd)/include -Wall -Wextra -Werror -Wno-unused-parameter -std=c99 -pedantic
#export LDFLAGS 	:= -lpthread -lncurses
export CFILES 	:= src/logging.c \
					src/handler_console.c \
					src/handler_syslog.c \
					src/list.c
export OFILES 	:= $(CFILES:.c=.o)

.PHONY: clean logging

logging: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(OFILES) logging
