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
