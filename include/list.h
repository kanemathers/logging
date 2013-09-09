#ifndef LOGGING_LIST_H
#define LOGGING_LIST_H

typedef struct list_node
{
    void             *data;
    struct list_node *next;
} list_node_t;

typedef struct list
{
    list_node_t *head;
    list_node_t *tail;
} list_t;

list_t *list_new();
void list_free(list_t *list, void (*free_func)(void *));

int list_push(list_t *list, void *data);

#endif
