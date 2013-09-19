#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <syslog.h>

#include "list.h"

typedef struct logger
{
    list_t *handlers;
} logger_t;

enum handler_type
{
    HANDLER_CONSOLE,
    HANDLER_SYSLOG,
    HANDLER_FILE,
};

typedef struct handler
{
    int               priority;
    enum handler_type type;

    void              (*emit)(logger_t *logger, int priority,
                              const char *message);
} handler_t;

logger_t *logger_new();
int logger_add_handler(logger_t *logger, handler_t *handler);
int logger_emit(logger_t *logger, int priority, const char *format, ...);

handler_t *handler_new(enum handler_type type, int priority);

#endif
