#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv)
{
  int ppid;                     /* parent pid */
  int cpid;                     /* child pid */
  int p1[2], p2[2];             /* pipe fds */
  char b[2] = {0};
  int n;

  ppid = getpid();

  n = pipe(p1);
  if (n < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }
  n = pipe(p2);
  if (n < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  cpid = fork();
  if (cpid == 0) {
    /*
     * Read a byte from pipe and then write it back.
     */
    cpid = getpid();
    n = read(p1[0], b, 1);
    printf("%d: received ping\n", cpid);
    n = write(p2[1], b, 1);
  } else {
    b[0] = 'x';
    n = write(p1[1], b, 1);
    n = read(p2[0], b, 1);
    printf("%d: received pong\n", ppid);
  }
  exit(0);
}
