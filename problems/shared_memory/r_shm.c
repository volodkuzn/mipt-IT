#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#define BUFSIZE 1024

extern int errno;

int main(int argc, char* argv[]) {
    //shared memory
    char* path = "./w_shm";
    key_t shm_key = ftok(path, 1);
    if (shm_key < 0) {
        fprintf(stderr, "Can't generate key.\n");
        exit(0);
    }

    int shmid = shmget(shm_key, sizeof(int), 0600);//Нельзя просто так узнать размер памяти, а передавать ещё память с длиной строки лень
    if (shmid < 0) {
        fprintf(stderr, "Can't get shared memory. Error:%d\n", errno);
        exit(0);
    }
    int *_str_size = shmat(shmid, NULL, SHM_RDONLY);//unknown warning
    int str_size = *_str_size;
    shmdt(_str_size);
    //printf("%d\n", str_size);

    //shmid = shmget(shm_key, sizeof(int) + str_size * sizeof(char), 0600);
    //Но на самом деле это и не надо, так как shmid всё равно один и тот же)
    //if (shmid < 0) {
    //    fprintf(stderr, "Can't get shared memory. Error:%d\n", errno);
    //    exit(0);
    //}
    int *mem = shmat(shmid, NULL, SHM_RDONLY);//unknown warning
    char* str = mem + sizeof(int);
    printf("%s", str);
    shmdt(mem);
    int err = shmctl(shmid, IPC_RMID, NULL);
    if (err != 0) {
        fprintf(stderr, "Error while deleting shared memory\n");
        exit(1);
    }
    exit(0);
}
