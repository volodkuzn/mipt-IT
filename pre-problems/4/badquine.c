#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if ((argc < 1) || (argc > 4)) {
        fpritntf(stderr, "Wrong number of arguments, must be 2 or 3\n");
        return 1;
    } else if (strcmp(argv[1], "r")) {
        printf("Init reading\n");
    }
    return 0;
}
