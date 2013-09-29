#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <syslog.h>

#include "handlers.h"
#include "list.h"

typedef struct logger
{
    list_t *handlers;
} logger_t;

logger_t *logger_new();
void logger_free(logger_t *logger);

int logger_add_handler(logger_t *logger, handler_t *handler, int priority);
int logger_emit(logger_t *logger, int priority, const char *format, ...);

#endif
