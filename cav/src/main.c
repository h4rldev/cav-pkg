#include "../../libcav/include/log.h"
#include "../include/cli.h"

int main(int argc, char *argv[]) {
  int res = cav_logtofile(NULL, true);
  if (res == 0)
    return process_args(&argc, &argv);
  else
    return -1;
}
