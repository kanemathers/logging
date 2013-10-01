#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logging/logger.h"
#include "logging/handlers.h"

logger_t *logger_new()
{
    logger_t *logger = malloc(sizeof *logger);

    if (!logger)
        return NULL;

    logger->handlers = list_new();

    if (!logger->handlers)
    {
        free(logger);

        return NULL;
    }

    return logger;
}

int logger_add_handler(logger_t *logger, handler_t *handler, int priority)
{
    handler->priority = priority;

    return list_push(logger->handlers, handler);
}

int logger_emit(logger_t *logger, int priority, const char *format, ...)
{
    list_node_t *node;
    handler_t   *handler;
    va_list      args;
    char        *message;

    va_start(args, format);

    message = malloc(vsnprintf(NULL, 0, format, args) + 1);

    va_end(args);

    if (!message)
        return -1;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    for (node = logger->handlers->head; node; node = node->next)
    {
        handler = (handler_t *) node->data;

        if (priority <= handler->priority)
            handler->emit(handler, priority, message);
    }

    return 0;
}

void logger_free(logger_t *logger)
{
    list_node_t *node;
    handler_t   *handler;

    for (node = logger->handlers->head; node; node = node->next)
    {
        handler = (handler_t *) node->data;

        handler->free(handler);
    }

    list_free(logger->handlers, NULL);
    free(logger);
}
