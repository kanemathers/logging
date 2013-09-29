#include <stdlib.h>
#include <syslog.h>

#include "handlers.h"

static void hsyslog_emit(handler_t *handler, int priority, const char *message)
{
    syslog(priority, "%s", message);
}

static void hsyslog_free(handler_t *handler)
{
    closelog();
    free(handler);
}

handler_t *hsyslog_new(const char *ident, int facility)
{
    handler_t *handler = malloc(sizeof *handler);

    if (!handler)
        return NULL;

    handler->emit = hsyslog_emit;
    handler->free = hsyslog_free;

    /* TODO: be cool to init the logger with a name/ident for all handlers and
     * just forward that through as the `ident`.
     */
    openlog(ident, LOG_ODELAY, facility);

    return handler;
}
