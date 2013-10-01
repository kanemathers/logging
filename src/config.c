#include <string.h>

#include "logging/logger.h"
#include "logging/handlers.h"
#include "logging/ini.h"

struct config
{
    int  console_enabled;
    int  console_priority;

    int  syslog_enabled;
    int  syslog_priority;
    char syslog_ident[100];

    int  file_enabled;
    int  file_priority;
    char file_path[1024];
};

static int priority_from_string(const char *str)
{
    if (strcmp(str, "emerg") == 0)
        return LOG_EMERG;
    else if (strcmp(str, "alert") == 0)
        return LOG_ALERT;
    else if (strcmp(str, "crit") == 0)
        return LOG_CRIT;
    else if (strcmp(str, "err") == 0)
        return LOG_ERR;
    else if (strcmp(str, "warning") == 0)
        return LOG_WARNING;
    else if (strcmp(str, "notice") == 0)
        return LOG_NOTICE;
    else if (strcmp(str, "info") == 0)
        return LOG_INFO;
    else if (strcmp(str, "debug") == 0)
        return LOG_DEBUG;

    return LOG_DEBUG;
}

static int ini_handler(void *arg, const char *section, const char *name,
                       const char *value)
{
    struct config *config = (struct config *) arg;
    char          *handler;
    char          *state;

    if (strcmp(section, "logging") == 0 && strcmp(name, "handlers") == 0)
    {
        for (handler = strtok_r((char *) value, ",", &state); handler;
             handler = strtok_r(NULL, ",", &state))
        {
            if (strcmp(handler, "console") == 0)
                config->console_enabled = 1;
            else if (strcmp(handler, "syslog") == 0)
                config->syslog_enabled = 1;
            else if (strcmp(handler, "file") == 0)
                config->file_enabled = 1;
        }
    }
    else if (strcmp(section, "handler_console") == 0)
    {
        if (strcmp(name, "priority") == 0)
            config->console_priority = priority_from_string(value);
    }
    else if (strcmp(section, "handler_syslog") == 0)
    {
        if (strcmp(name, "priority") == 0)
            config->syslog_priority = priority_from_string(value);
        else if (strcmp(name, "ident") == 0)
            snprintf(config->syslog_ident, sizeof config->syslog_ident, "%s",
                    value);
    }
    else if (strcmp(section, "handler_file") == 0)
    {
        if (strcmp(name, "priority") == 0)
            config->file_priority = priority_from_string(value);
        else if (strcmp(name, "path") == 0)
            snprintf(config->file_path, sizeof config->file_path, "%s", value);
    }

    return 1;
}

logger_t *logger_from_config(const char *path)
{
    logger_t      *logger = logger_new();
    handler_t     *handler;
    struct config  config;

    memset(&config, 0, sizeof config);

    if (!logger)
        return NULL;

    /* parse the ini shit into the `config` struct to give us something
     * programable to play with */
    if (ini_parse(path, ini_handler, &config) < 0)
        return NULL;

    /* righto. got the config. now setup the logger */
    if (config.console_enabled)
    {
        handler = hconsole_new();

        if (handler)
            logger_add_handler(logger, handler, config.console_priority);
    }

    if (config.syslog_enabled)
    {
        handler = hsyslog_new(config.syslog_ident, LOG_LOCAL0);

        if (handler)
            logger_add_handler(logger, handler, config.syslog_priority);
    }

    if (config.file_enabled)
    {
        handler = hfile_new(config.file_path);

        if (handler)
            logger_add_handler(logger, handler, config.file_priority);
    }

    return logger;
}
