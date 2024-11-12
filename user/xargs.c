#include "kernel/types.h"
#include "user/user.h"

/*
 * xargs <args>
 *
 * Read each line from stdin. Execute <args> <line n> for each line
 * from stdin.
 */

int main(int argc, char **argv) {
  char buf[512];
  int pid;
  char *args[32];
  int i, idx = 0;
  int len;

  if (argc < 2) {
    fprintf(2, "usage: xargs <prog> <args>\n");
  }

  /*
   * Construct new argv for exec
   */
  for (i = 1; i < argc; i++) {
    args[idx++] = argv[i];
  }
  args[idx] = 0;
  args[idx+1] = 0;

  while (1) {
    buf[0] = 0;
    gets(buf, sizeof(buf));
    if (!buf[0]) {
      break;
    }
    /*
     * gets() returns ending newline which needs to be stripped.
     */
    len = strlen(buf);
    if (buf[len-1] == '\n') {
      buf[len-1] = 0;
    }
    /*
     * If nothing left after stripping newline, continue.
     */
    if (!buf[0]) {
      continue;
    }
    pid = fork();
    if (pid == 0) {
      args[idx] = buf;
      exec(argv[1], args);
    } else {
      wait(0);
    }
  }
  exit(0);
}

