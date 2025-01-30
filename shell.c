#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[])
{

        int id = fork();

    if (id < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (id == 0) {

        printf("Child process\n");

    } else {
        wait(NULL);
        printf("Parent procees\n");
        

    }

    return 0;
}
