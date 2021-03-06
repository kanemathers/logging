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
#include <syslog.h>

#include "logging/handlers.h"

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

    handler->emit      = hsyslog_emit;
    handler->free      = hsyslog_free;
    handler->internals = NULL;

    /* TODO: be cool to init the logger with a name/ident for all handlers and
     * just forward that through as the `ident`.
     */
    openlog(ident, LOG_ODELAY, facility);

    return handler;
}
