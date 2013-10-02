#include <stdio.h>
#include <stdlib.h>

#include "logging/logger.h"
#include "logging/handlers.h"

static void hconsole_emit(handler_t *handler, int priority, const char *message)
{
    FILE *stream = (priority >= LOG_NOTICE) ? stdout : stderr;

    flockfile(stream);
    fprintf(stream, "%s", message);
    funlockfile(stream);
}

static void hconsole_free(handler_t *handler)
{
    free(handler);
}

handler_t *hconsole_new()
{
    handler_t *handler = malloc(sizeof *handler);

    if (!handler)
        return NULL;

    handler->emit      = hconsole_emit;
    handler->free      = hconsole_free;
    handler->internals = NULL;

    return handler;
}
