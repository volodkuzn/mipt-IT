#include <unistd.h>
#include <stdio.h>

int main(void) {
    char* const catarg[4] = {"cat", "-b", "badquine.c", NULL};
    int errcode = execvp("cat", catarg);
    printf("Is it working? Error: %d\n", errcode);
    return 0;
}
