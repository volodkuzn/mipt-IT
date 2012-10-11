#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }
    pid_t child_pid = fork();
    if (child_pid == 0) {
        int errcode = execlp("cat", argv[0], argv[1], NULL);
        if (errcode) {
            fprintf(stderr, "Unable to execute cat. Error:%d\n", errcode);
            _exit(2);
        }
    } else if (child_pid > 0) {
        wait();
        int errcode = execlp("rm", argv[0], argv[1], NULL);
        if (errcode) {
            fprintf(stderr, "Unable to execute rm. Error:%d\n", errcode);
            exit(3);
        }
    }
    exit(0);
}

