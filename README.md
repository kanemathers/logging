logging
=======

A small, flexible logging framework for C libraries and applications.

Designed for use in large applications down to embedded systems.

Installing
----------

    $ git clone https://github.com/kanemathers/logging.git
    $ cd logging
    $ make
    $ sudo make install

Usage
-----

### Programmable

First thing you'll want to do is create your logger:

    logger_t *logger = logger_new();

Then create your handlers:

    handler_t *console = hconsole_new();
    handler_t *file    = hfile_new("/tmp/output.log");
    handler_t *syslog  = hsyslog_new("ident", LOG_LOCAL0);

Need to add those handlers to the logger. Specify a priority to filter the
output to that handler:

    logger_add_handler(logger, console, LOG_DEBUG);
    logger_add_handler(logger, file, LOG_DEBUG);
    logger_add_handler(logger, syslog, LOG_ERR);

And now you can start firing off log messages:

    logger_emit(logger, LOG_DEBUG, "syslog won't see this");
    logger_emit(logger, LOG_ERR, "all handlers will %s", "see this");


When you're done, cleanup:

    logger_free(logger);

### Config

You can also generate a logger instance from a config file:

    logger_t *logger = logger_from_config("./config.ini");

### Full Example

    #include <stdlib.h>
    #include <logging.h>

    int main()
    {
        logger_t *logger = logger_from_config("./config.ini");

        if (!logger)
            return EXIT_FAILURE;

        logger_emit(logger, LOG_DEBUG, "works!\n");

        logger_free(logger);

        return EXIT_SUCCESS;
    }

