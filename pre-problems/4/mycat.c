#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define BUFSIZE 10

char *make_string(char *old_str) {
    char *new_str = (char *) malloc(strlen(old_str) * sizeof(char));
    if (new_str == NULL) {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(5);
    }
    //this should be more complicated version with '\n' '\"' and other special simbols 
    memcpy(new_str, old_str, strlen(old_str) * sizeof(char));
    return new_str;
}

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

int write_file(char* path, char* str) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0) {
        fprintf(stderr, "Unable to open file.\n");
        exit(6);
    }
    int str_size = strlen(str);
    int buf_size = write(fd, str, str_size);
    if (str_size != buf_size) {
        fprintf(stderr, "Unable to write string\n");
        exit(9);
    }
    if (close(fd) < 0) {
        fprintf(stderr, "File hasn't closed correctly");
        exit(8);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if ((argc < 2) || (argc > 4)) {
        fprintf(stderr, "Wrong number of arguments, must be 2 or 3\n");
        return 1;
    } else if (strcmp(argv[1], "r") == 0) {
        printf("Init reading...\n");
        if (argc != 3) {
            fprintf(stderr, "Error. Must be 1 additional argument with filename.\n");
            return 2;
        } else {
            read_file(argv[2]);
        }
    } else if (strcmp(argv[1], "w") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Error. Must be 2 additional arguments with filename and string in \"\"\n");
            return 3;
        } else {
            char* w_str = make_string(argv[3]); // You need to free memory
            printf("Init writing\n");
            write_file(argv[2], w_str);
            printf("String \"%s\" wrote succefully to \"%s\"\n", w_str, argv[2]);
            free(w_str);
        }
    } else {
        fprintf(stderr, "Error. Invalid first argument.\n");
        return 4;
    }
    return 0;
}
