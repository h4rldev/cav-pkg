#include "../include/log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cav_log(logLevels level, char *fmt, ...) {
  va_list args;

  int get_log = 0;
  char fmt_buf[1024] = {0};

  logLevels current_level = Error;

  char *env = getenv("CAV_LOG_LEVEL");
  if (env != NULL) {
    get_log = atoi(env);
    if (get_log > 5)
      return -1;

    current_level = get_log;
  }

  FILE *current_file = stdout;
  if (current_level > 4) {
    current_file = stderr;
  }

  char *prefix;
  switch (level) {
  case Trace:
    prefix = "[TRACE]:";
    break;
  case Debug:
    prefix = "[DEBUG]:";
    break;
  case Info:
    prefix = "[INFO]:";
    break;
  case Warning:
    prefix = "[WARN]:";
    break;
  case Error:
    prefix = "[ERROR]:";
    break;
  case Fatal:
    prefix = "[FATAL]:";
    break;
  default:
    cav_log(Error, "Congratulations, you managed to make the log "
                   "function error, you're a mess uhhhhhhh Daniel!\n");
    return -1;
  }

  snprintf(fmt_buf, strlen(prefix) + strlen(fmt) + 6, "CAV %s %s", prefix, fmt);

  size_t fmt_buf_len = strlen(fmt_buf);

  // Insert a \n if none is available :3
  if (fmt_buf[fmt_buf_len - 1] != '\n') {
    fmt_buf[fmt_buf_len] = '\n';
    fmt_buf[fmt_buf_len + 1] = '\0';
  }

  va_start(args, fmt);
  if (level <= current_level)
    vfprintf(current_file, fmt_buf, args);
  va_end(args);

  return 0;
}
