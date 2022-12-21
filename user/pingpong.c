#include "user/user.h"

int main(int argc, char **argv) {
    int pipe_fd[2];
    char buf[1];
    pipe(pipe_fd);

    int pid = fork();
    if (pid == 0) {
        close(pipe_fd[0]);
        read(pipe_fd[1], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(pipe_fd[0], " ", 1);
        exit(0);
    }

    // parent
    close(pipe_fd[1]);
    write(pipe_fd[0], " ", 1);
    read(pipe_fd[0], buf, 1);
    fprintf(1, "%d: received pong\n", getpid());
    exit(0);
}