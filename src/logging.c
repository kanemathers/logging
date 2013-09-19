#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logging.h"

static void emit_console(logger_t *logger, int priority, const char *message)
{
    FILE *stream = (priority >= LOG_NOTICE) ? stdout : stderr;

    fprintf(stream, "%s", message);
}

handler_t *handler_new(enum handler_type type, int priority)
{
    handler_t *handler = malloc(sizeof *handler);

    if (!handler)
        return NULL;

    switch (type)
    {
        case HANDLER_CONSOLE:
            handler->emit = emit_console;

            break;

        default:
            return NULL;
    }

    handler->priority = priority;
    handler->type     = type;

    return handler;
}

logger_t *logger_new()
{
    logger_t *logger = malloc(sizeof *logger);

    if (!logger)
        return NULL;

    logger->handlers = list_new();

    return logger;
}

int logger_add_handler(logger_t *logger, handler_t *handler)
{
    return list_push(logger->handlers, handler);
}

int logger_emit(logger_t *logger, int priority, const char *format, ...)
{
    list_t      *handlers = logger->handlers;
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

    while (handlers->head != NULL)
    {
        node    = handlers->head;
        handler = (handler_t *) node->data;

        if (priority <= handler->priority)
            handler->emit(logger, priority, message);

        handlers->head = node->next;
    }

    return 0;
}

int main()
{
    logger_t  *logger  = logger_new();
    handler_t *console = handler_new(HANDLER_CONSOLE, LOG_DEBUG);

    logger_add_handler(logger, console);
    logger_emit(logger, LOG_DEBUG, "test %s\n", "works!");

    return 0;
}
