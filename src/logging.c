#include <stdlib.h>
#include <syslog.h>

#include "logging.h"

static int emit_console(struct log_record *record)
{
    return 0;
}

static int emit_syslog(struct log_record *record)
{
    return 0;
}

static int emit_file(struct log_record *record)
{
    return 0;
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

int main()
{
    struct logger     *logger  = logger_new();
    struct handler    *console = handler_new(HANDLER_CONSOLE, LOG_DEBUG);
    struct log_record  record  = {LOG_DEBUG, "tesing"};

    logger_add_handler(logger, console);

    return 0;
}
