#include "user/user.h"

int new_unit();

void give_to(int fd, int x) { write(fd, &x, sizeof(int)); }

void handle_from(int from) {
  int x;
  if (read(from, &x, sizeof(int)) != sizeof(int)) {
    return;
  }

  fprintf(1, "prime %d\n", x);
  int right = -1;
  int p = x;
  while (read(from, &x, sizeof(int)) == sizeof(int)) {
    if (x % p) {
      if (right == -1) {
        right = new_unit();
      }
      give_to(right, x);
    }
  }
  close(right);
  int status;
  wait(&status);
}

// create a new unit of the pipeline at the right side
// return fd of pipe to it
int new_unit() {
  int fd_pe[2];
  pipe(fd_pe);
  int pid = fork();
  if (pid == 0) {
    // child
    close(fd_pe[1]);
    handle_from(fd_pe[0]);
    exit(0);
  }

  // parent
  close(fd_pe[0]);
  return fd_pe[1];
}

int main(int argc, char **argv) {
  int right = new_unit();
  for (int i = 2; i <= 35; ++i) {
    give_to(right, i);
  }
  close(right);
  int status;
  wait(&status);
  exit(0);
}