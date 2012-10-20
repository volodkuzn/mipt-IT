#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int list_dir_to (char* path, int fd) { //Note: exec
    //redirecting stdout to fd
    dup2(fd, STDOUT_FILENO);//It's common way. But you can simply close(1) and open file for stdout again with open()
    
    //printf("Harr\n");
    if (close(fd) < 0) {
        fprintf(stderr, "File hasn't closed correctly");
        exit(4);
    }

    //executing ls with redirected input
    int errcode = execlp("ls", "myls", path, NULL);
    fprintf(stderr, "Something went wrong. Error %d\n", errcode);
    return errcode;
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments. Must be 1.\n");
        exit(1);
    }
    //making tmpfile
    char* path = "./tmpfile";
    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        fprintf(stderr, "Unable to create temp file.\n");
        exit(2);
    }
    char* str = "List of files:\n";
    size_t str_size = strlen(str);
    size_t buf_size = write(fd, str, str_size);
    if (str_size != buf_size) {
        fprintf(stderr, "Unable to write string\n");
        exit(3);
    }
    int errcode = list_dir_to(argv[1], fd);
}
