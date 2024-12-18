#ifndef LOG_H
#define LOG_H

#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_PATH "/var/log/cav/"

typedef enum { Fatal, Error, Warning, Info, Debug, Trace } logLevels;

/* * log: create a log_message with specified level, prints out log message if
 * level is current.
 * - level: set level of log, available values: Trace, Debug, Info, Warning,
 * Error & Fatal.
 * - fmt: specify message format example: "Hello %s".
 * - ...: variadic args, will be used after format.
 */

int cav_logtofile(char *file, bool on);
int cav_log(logLevels level, char *fmt, ...);
#endif // !LOG_H
