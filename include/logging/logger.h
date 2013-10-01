#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <syslog.h>

#include "logging/handlers.h"
#include "logging/list.h"

typedef struct logger
{
    list_t *handlers;
} logger_t;

logger_t *logger_new();
logger_t *logger_from_config(const char *path);
void logger_free(logger_t *logger);

int logger_add_handler(logger_t *logger, handler_t *handler, int priority);
int logger_emit(logger_t *logger, int priority, const char *format, ...);

#endif
