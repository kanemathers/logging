/* Copyright (c) 2013 Kane Mathers
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>

#include "logging/handlers.h"

static void hfile_emit(handler_t *handler, int priority, const char *message)
{
    FILE *file = (FILE *) handler->internals;

    flockfile(file);
    fprintf(file, "%s", message);
    funlockfile(file);
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
