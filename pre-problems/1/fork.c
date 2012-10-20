#include <stdio.h>
#include <unistd.h>
#define PRINT_PID \
	mypid = getpid(); \
	myppid = getppid(); \
	printf("My pid: %d\n", mypid); \
	printf("My parent's pid: %d\n", myppid); \

int main(void) {
	pid_t mypid = 0;
	pid_t myppid = 0;
    PRINT_PID
	printf("Forking...\n");	
	pid_t chpid = fork();
	if (chpid < 0) {
		fprintf(stderr, "Cannot create child\n");
		return -1;
	} else if (chpid == 0) {
        printf("\nI'm a child..\n");
        PRINT_PID
        if (myppid == 1)
            fprintf(stderr, "Oops. My parent has already died.\n");
    } else {
        printf("I'm a parent. My child is %d.\n", chpid);
        PRINT_PID
        int i;
        for (i = 0; i < 10000000; i++);
        //sleep(1);
    }
	return 0;
}
