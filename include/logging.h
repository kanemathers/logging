#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <syslog.h>

#include "list.h"

struct log_record
{
    int         priority;
    const char *message;
};

enum handler_type
{
    HANDLER_CONSOLE,
    HANDLER_SYSLOG,
    HANDLER_FILE,
};

struct handler
{
    int               priority;
    enum handler_type type;

    int               (*emit)(struct log_record *record);
};

struct logger
{
    list_t *handlers;
};

struct logger  *logger_new();
struct handler *handler_new(enum handler_type type, int priority);

int logger_add_handler(struct logger *logger, struct handler *handler);

#endif
