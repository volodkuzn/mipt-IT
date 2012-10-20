#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFSIZE 256

int read_file(char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Unable to open file.\n");
        exit(6);
    }
    //It can be read by character but it's more useful to put it in memory
    char* str = (char *) malloc(sizeof(char) * BUFSIZE);
    int str_size = 0;
    if (str == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(7);
    }
    str_size = read(fd, str, (BUFSIZE - 1) * sizeof(char));
    while (str_size != 0) {
        str[str_size] = '\0';
        printf("%s", str);
        str_size = read(fd, str, (BUFSIZE - 1) * sizeof(char));
 //       printf("  %d\n", str_size);
        }
    printf("\n");
    free(str);
    if (close(fd) < 0) {
        fprintf(stderr, "File hasn't closed correctly");
        exit(8);
    }
    return 0;
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }
    pid_t child_pid = fork();
    if (child_pid == 0) {
//        int errcode = execlp("cat", argv[0], argv[1], NULL);
//        if (errcode) {
//            fprintf(stderr, "Unable to execute cat. Error:%d\n", errcode);
//             _exit(2);
//        }
        read_file(argv[1]);
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

