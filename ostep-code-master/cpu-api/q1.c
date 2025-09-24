#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int x = 100; //Sets the variable before fork

    printf("Before fork: x = %d (pid:%d)\n", x, (int) getpid());

    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if(rc == 0){
       //child process
        printf("Child: x = %d (pid:%d)\n", x, (int) getpid());
        x = 200; //Changes x in child
        printf("Child after change: x = %d (pid:%d)\n", x, (int) getpid());
    } else {
       //parent process
        printf("Parent: x = %d (pid:%d)\n", x, (int) getpid());
        x = 300; //Changes x in parent
        printf("Parent after change: x = %d (pid:%d)\n", x, (int) getpid());
        wait(NULL); //Waits for child to finish
    }

    printf("Final x = %d (pid:%d)\n", x, (int) getpid());
    return 0;
}
