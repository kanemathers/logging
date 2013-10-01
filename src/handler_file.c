#include <stdlib.h>
#include <stdio.h>

#include "handlers.h"

static void hfile_emit(handler_t *handler, int priority, const char *message)
{
    FILE *file = (FILE *) handler->internals;

    fprintf(file, "%s", message);
}

static void hfile_free(handler_t *handler)
{
    FILE *file = (FILE *) handler->internals;

    fclose(file);
    free(handler);
}

handler_t *hfile_new(const char *path)
{
    handler_t *handler = malloc(sizeof *handler);
    FILE      *file    = fopen(path, "a");

    if (!handler)
        return NULL;

    if (!file)
    {
        free(handler);

        return NULL;
    }

    handler->emit      = hfile_emit;
    handler->free      = hfile_free;
    handler->internals = file;

    return handler;
}
