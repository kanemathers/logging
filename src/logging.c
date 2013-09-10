#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>

#include "logging.h"

static void emit_console(struct log_record *record)
{
    fprintf(stderr, "%s\n", record->message);
}

static void emit_syslog(struct log_record *record)
{
}

static void emit_file(struct log_record *record)
{
}

struct handler *handler_new(enum handler_type type, int priority)
{
    struct handler *handler = malloc(sizeof *handler);

    if (!handler)
        return NULL;

    switch (type)
    {
        case HANDLER_CONSOLE:
            handler->emit = emit_console;
            break;

        case HANDLER_SYSLOG:
            handler->emit = emit_syslog;
            break;

        case HANDLER_FILE:
            handler->emit = emit_file;
            break;

        default:
            return NULL;
    }

    handler->priority = priority;
    handler->type     = type;

    return handler;
}

struct logger *logger_new()
{
    struct logger *logger = malloc(sizeof *logger);

    if (!logger)
        return NULL;

    logger->handlers = list_new();

    return logger;
}

int logger_add_handler(struct logger *logger, struct handler *handler)
{
    return list_push(logger->handlers, handler);
}

void logger_emit(struct logger *logger, struct log_record *record)
{
    list_t         *handlers = logger->handlers;
    list_node_t    *node;
    struct handler *handler;

    while (handlers->head != NULL)
    {
        node    = handlers->head;
        handler = (struct handler *) node->data;

        if (record->priority <= handler->priority)
            handler->emit(record);

        handlers->head = node->next;
    }
}

int main()
{
    struct logger     *logger  = logger_new();
    struct handler    *console = handler_new(HANDLER_CONSOLE, LOG_DEBUG);
    struct log_record  record  = {LOG_DEBUG, "tesing"};

    logger_add_handler(logger, console);

    logger_emit(logger, &record);

    return 0;
}
