#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int ticks = 0;

  if (argc < 2) {
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }

  ticks = atoi(argv[1]);

  if (ticks < 0) {
    fprintf(2, "error: sleep time must be 0 or more, got %d\n", ticks);
    exit(1);
  }

  sleep(ticks);
  exit(0);
}
