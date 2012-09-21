#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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
    fseek(fd, 0, SEEK_END);
    int str_size = ftell(fd);
    rewind(fd);
    char* str = (char *) malloc(sizeof(char) * str_size);
    if (str == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(7);
    }
    int result = read(str, 1, str_size, fd);
    if (result != str_size) {
        fprintf(stderr, "Unable to read file\n");
    }
    str[str_size - 1] = '\0';
    printf("%s\n", str);
    free(str);
    return 0;
}

int write_file(char* path, char* str) {
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
            char* w_str = make_string(argv[2]); // You need to free memory
            write_file(argv[2], w_str);
            free(w_str);
        }
    } else {
        fprintf(stderr, "Error. Invalid first argument.\n");
        return 4;
    }
    return 0;
}
