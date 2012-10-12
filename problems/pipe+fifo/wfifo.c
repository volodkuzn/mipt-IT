#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 1024

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }
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
        //char* str = "Harr!";
        //size_t str_size = strlen(str);
        close(mypipe[0]);
        //ssize_t wr_size = write(mypipe[1], str, str_size);
        //if (str_size != wr_size ) {
        //    fprintf(stderr, "Can't write to pipe\n");
        //    _exit(3);
        //}
        list_dir_to(argv[1], mypipe[1]);
        //close(mypipe[1]);
        _exit(0);
    } else {//reader
        //printf("I'm a parent.\n"); // It's not working without this string. I have no idea why.
        close(mypipe[1]);
        //wait();//It is changing mypipe[0] to 0 when there is no operator before
        //printf("I was waiting for a long time.\n");
        
        //make FIFO
        umask(0);
        char* filename = "temp.fifo";
        int errcode = mkfifo(filename, 0777);

        int fd = open(filename, O_WRONLY);
        ssize_t wsize;

        ssize_t str_size = -1;
        char* str = (char*) malloc(BUFSIZE * sizeof(char));
        while (str_size != 0) {
            str_size = read(mypipe[0], str, BUFSIZE);
            //str[str_size] = '\0';
            //printf("%s\n", str);
            wsize = write(fd, str, str_size);
            if (wsize != str_size) {
                fprintf(stderr, "Can't write all string");
            }
        }
        close(mypipe[0]);
        exit(0);
    }
    return 0;
}
