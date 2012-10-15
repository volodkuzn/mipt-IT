#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#define BUFSIZE 1024

extern int errno;

int list_dir_to (char* path, int fd) { //Note: exec
    //redirecting stdout to fd
    dup2(fd, STDOUT_FILENO);//It's common way. But you can simply close(1) and open file for stdout again with open()
    
    //printf("Harr\n");
    if (close(fd) < 0) {
        fprintf(stderr, "File hasn't closed correctly");
        _exit(4);
    }

    //executing ls with redirected input
    int errcode = execlp("ls", "myls", path, NULL);
    fprintf(stderr, "Something went wrong. Error %d\n", errcode);
    return errcode;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(0);
    }
    //getting output of ls command
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
        close(mypipe[0]);
        list_dir_to(argv[1], mypipe[1]);
        _exit(0);
    }//parent process

    close(mypipe[1]);
    //int bufstr_size = -1;
    //char* bufstr = (char*) malloc(BUFSIZE * sizeof(char));
    int str_size = 0;
    int r_size = -1;
    char* str = (char*) malloc(BUFSIZE * sizeof(char));//а вдруг в буфер сразу не влезет
    char* p_str = str;
    while (r_size != 0) {
        r_size = read(mypipe[0], p_str, BUFSIZE - 1);
        p_str += r_size * sizeof(char);
        str_size += r_size;
        if (r_size != 0)
            str = (char *) realloc(str, str_size + BUFSIZE);
    }
    str[str_size] = '\0';
    close(mypipe[0]);
    //printf("%s\n", str);
    //printf("%d", str_size);
    
    //shared memory
    char* path = getenv("_");
    key_t shm_key = ftok(path, 1);
    if (shm_key < 0) {
        fprintf(stderr, "Can't generate key.\n");
        exit(0);
    }
    //int shmid = shmget(shm_key, BUFSIZE * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL);
    int shmid = shmget(shm_key, str_size * sizeof(char) + sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
    if (shmid < 0) {
        fprintf(stderr, "Can't create shared memory. Error:%d\n", errno);
        exit(0);
    }
    char *mem = shmat(shmid, NULL, 0);//unknown warning
    memcpy(mem, (char *) &str_size, sizeof(int));//dirty hack
    memcpy(mem + sizeof(int), str, str_size + 1);
    
    //printf("%s", mem + sizeof(int));
    shmdt(mem);
    free(str);
    str = NULL;
    exit(0);
}
