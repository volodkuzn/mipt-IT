#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFSIZE 100

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
        // COMMENT PA: а всегда ли нужно ставить признак конца строки?
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

int main(int argc, char* argv[]) {
    char *filename = "temp.fifo";
    read_file(filename);
    int errcode = execlp("rm", argv[0], filename, NULL);
    return 0;
}
