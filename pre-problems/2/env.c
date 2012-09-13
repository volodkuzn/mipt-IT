#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

extern char **environ;

int main (int argc, char *argv[]) {
	if (argc > 1) {
		printf("First argument: %s\n", argv[1]);
	} else {
		fprintf(stderr, "Error: no arguments\n");
		return -1;
	}
	char **p = environ;
	while (*p != NULL) {
		//printf("%s (%p)\n", *p, *p);
		printf("%s\n", *p);
		p++;
	}
	return 0;
}

//Another variant
//int main (int argc, char *argv[], char *envp[]) {
