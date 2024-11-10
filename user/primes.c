#include "kernel/types.h"
#include "user/user.h"

#define IN  0
#define OUT 1

void primes(int in) __attribute__((noreturn));

/*
 * This is the sieve of Eratosthenes, using a process pipeline
 *
 * Primes Algorithm:
 * - read first_num
 * - print first_num
 * - while(1):
 *   - if num = read returns 0, exit
 *   - if num % first_num:
 *     - if not child:
 *       - create pipe
 *       - create child
 *     - write num
 */

void primes(int in) {
  int first_num, num;
  int pid = 0;
  int rc;
  int p[2];

  if (read(in, &first_num, sizeof(num)) == 0) {
    exit(0);
  }
  printf("prime %d\n", first_num);

  while (1) {
    if (read(in, &num, sizeof(num)) == 0) {
      if (pid) {
        close(p[OUT]);
        wait(0);
      }
      exit(0);
    }
    if (0 == (num % first_num)) {
      continue;
    }
    if (!pid) {
      rc = pipe(p);
      if (rc < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
      }
      pid = fork();
      if (pid == 0) {
        close(p[OUT]);
        close(in);
        primes(p[IN]);
      } else {
        close(p[IN]);
      }
    }

    write(p[OUT], &num, sizeof(num));
  }
}

int main(int argc, char **argv) {
  int p[2];
  int pid;
  int num;
  int rc;

  rc = pipe(p);
  if (rc < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  pid = fork();

  if (pid == 0) {
    close(p[OUT]);
    primes(p[IN]);
  } else {
    close(p[IN]);

    for (num = 2; num < 280; num++) {
      write(p[OUT], &num, sizeof(num));
    }

    close(p[OUT]);              /* close so that child will exit */
    wait(0);
  }

  exit(0);
}
