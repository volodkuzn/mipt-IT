#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 1024

int main(void) {
    int mypipe[2];
    if (pipe(mypipe) < 0) {
        fprintf(stderr, "Can't create pipe\n");
        exit(1);
    }

    pid_t cpid = fork();
    if (cpid < 0) {
        fprintf(stderr, "Can't create child\n");
        exit(2);
    } else if (cpid == 0) { // child is not reader child is writer
        char* str = "Harr!";
        size_t str_size = strlen(str);
        ssize_t wr_size = write(mypipe[1], str, str_size);
        if (str_size != wr_size ) {
            fprintf(stderr, "Can't write to pipe\n");
            _exit(3);
        }
        _exit(0);
    } else {//reader
        printf("I'm a parent.\n"); // It's not working without this string. I have no answer why.
        wait();
        //printf("I was waiting for a long time.\n");
        char* str = (char*) malloc(BUF_SIZE * sizeof(char));
        ssize_t str_size = read(mypipe[0], str, BUF_SIZE);
        str[str_size] = NULL;
        printf("%s\n", str);
        exit(0);
    }
    return 0;
}
