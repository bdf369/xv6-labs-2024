#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/*
 * find <dir> <name>
 *
 * List all files with name <name>, starting at <dir> and any subdirs.
 *
 * find(dir, file_name):
 * - if dir type is not DIR: error
 * - for name in dir:
 *   - if name == file_name:
 *     - print name
 *   - if name is type DIR:
 *     - find(name, file)
 */

void
find(char *dir, char *filename) {
  int fd;
  int rc;
  struct dirent de;
  struct stat st;
  int len;
  char buf[512], *p;

  fd = open(dir, O_RDONLY);
  if (fd < 0) {
    fprintf(2, "open failed: %s\n", dir);
    return;
  }

  rc = fstat(fd, &st);
  if (rc < 0) {
    fprintf(2, "fstat failed: %s\n", dir);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "not a directory: %s\n", dir);
    return;
  }

  strcpy(buf, dir);
  len = strlen(buf);
  p = &buf[len];
  *p++ = '/';
  *p = 0;

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    }
    if ((0 == strcmp(de.name, ".")) || (0 == strcmp(de.name, ".."))) {
      continue;
    }
    strcpy(p, de.name);
    if (stat(buf, &st) < 0) {
      fprintf(2, "stat failed: %s\n", buf);
      continue;
    }
    if (0 == strcmp(filename, de.name)) {
      printf("%s\n", buf);
    }
    if (st.type == T_DIR) {
      find(buf, filename);
    }
  }
}

int
main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(2, "usage: find <dir> <filename>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
