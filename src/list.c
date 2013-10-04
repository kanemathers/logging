/* Copyright (c) 2012 Forest Belton (apples)
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
*
* Modified by Kane Mathers - 2013
*/

#include <stdlib.h>

#include "logging/list.h"

list_t *list_new()
{
    list_t *list = malloc(sizeof *list);

    if (!list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void list_free(list_t *list, void (*free_func)(void *))
{
    void        *data;
    list_node_t *node;

    while (list->head != NULL)
    {
        node = list->head;
        data = node->data;

        if (free_func != NULL)
            free_func(data);

        list->head = node->next;

        free(node);
    }
}

int list_push(list_t *list, void *data)
{
    list_node_t *node = malloc(sizeof *node);

    if (!node)
        return -1;

    node->data = data;
    node->next = NULL;

    if (!list->head)
        list->head = node;
    else
        list->tail->next = node;

    list->tail = node;

    return 0;
}
