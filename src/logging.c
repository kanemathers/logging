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
