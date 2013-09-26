#ifndef LOGGING_HANDLERS_H
#define LOGGING_HANDLERS_H

typedef struct handler
{
    int    priority;

    void (*emit)(struct handler *handler, int priority, const char *message);
    void (*free)(struct handler *handler);
} handler_t;

handler_t *hconsole_new();
handler_t *hsyslog_new(const char *ident, int facility);

#endif
