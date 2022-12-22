#include "kernel/param.h"
#include "user/user.h"

// return number of char reads
// or -1 on error
int getline(char *p, int sz) {
  char ch;
  int count = 0;
  while (read(0, &ch, sizeof(ch)) == sizeof(ch)) {
    if (count >= sz)
      return -1;

    p[count] = ch;
    if (ch == '\n') {
      p[count] = '\0';
      break;
    }

    ++count;
  }

  fprintf(1, "readline: %s\n", p);
  return count;
}

void xarg(char **argv, int narg, char *buf) {
  if (narg + 1 >= MAXARG) {
    fprintf(2, "too many arguments");
    exit(1);
  }

  int pid = fork();
  if (pid == 0) {
    char *a[MAXARG];
    for (int i = 0; i < narg; ++i) {
      a[i] = argv[i];
    }
    a[narg] = buf;
    a[narg + 1] = 0;

    // int flag = 1;
    // for (char *p = buf; *p != '\0'; ++p) {
    //   if (*p != ' ' && flag) {
    //     a[narg++] = p;
    //   }

    //   if (narg >= MAXARG) {
    //     fprintf(2, "too many arguments");
    //     exit(1);
    //   }

    //   flag = (*p == ' ');
    // }
    // a[narg] = 0;

    // for (int i = 0; i < narg; ++i) {
    //   fprintf(1, ": %s\n", a[i]);
    // }
    exec(a[0], a);
  }

  int status;
  wait(&status);
  return;
}

int main(int argc, char **argv) {
  const int buf_sz = 1024;
  char buf[buf_sz];
  while (1) {
    int r = getline(buf, buf_sz);
    if (r == -1) {
      fprintf(2, "line too long");
      exit(1);
    }
    if (r == 0) {
      break;
    }

    xarg(argv + 1, argc - 1, buf);
  }
  exit(0);
}