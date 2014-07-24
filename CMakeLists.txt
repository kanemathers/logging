cmake_minimum_required(VERSION 2.6)
project("SIMPLE_C_LOGGING")

include_directories(${SIMPLE_C_LOGGING_SOURCE_DIR}/include)

add_library(simpleCLogging
    src/config.c
    src/handler_file.c
    src/ini.c
    src/logging.c
    src/handler_console.c
    src/handler_syslog.c
    src/list.c
)
