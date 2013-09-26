#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logging.h"

logger_t *logger_new()
{
    logger_t *logger = malloc(sizeof *logger);

    if (!logger)
        return NULL;

    logger->handlers = list_new();

    return logger;
}

int logger_add_handler(logger_t *logger, handler_t *handler, int priority)
{
    handler->priority = priority;

    return list_push(logger->handlers, handler);
}

int logger_emit(logger_t *logger, int priority, const char *format, ...)
{
    list_t      *handlers = logger->handlers;
    list_node_t *head     = handlers->head;
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
            handler->emit(priority, message);

        handlers->head = node->next;
    }

    /* This feels hacky. Check this when you're not high */
    handlers->head = head;

    return 0;
}

int main()
{
    logger_t  *logger  = logger_new();
    handler_t *syslog  = hsyslog_new("logging", LOG_LOCAL0);
    handler_t *console = hconsole_new();

    logger_add_handler(logger, syslog, LOG_DEBUG);
    logger_add_handler(logger, console, LOG_DEBUG);

    logger_emit(logger, LOG_ERR, "test %s\n", "works!");

    return 0;
}
