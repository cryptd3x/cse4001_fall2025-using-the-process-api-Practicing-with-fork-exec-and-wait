#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    printf("Hello world (pid:%d)\n", (int) getpid());

    int rc = fork();
    if(rc < 0){  
        fprintf(stderr, "fork failed\n");
        exit(1); 
    } else if(rc == 0){
       //child process
        printf("Child: about to close STDOUT\n");

        close(STDOUT_FILENO);

       //Try to print after closing stdout
        printf("This printf should not appear\n");
        fprintf(stdout, "This fprintf to stdout should not appear\n");

       //stderr should still work
        fprintf(stderr, "Child: This stderr message should appear\n");
            
    } else {
       //parent process
        wait(NULL);
        printf("Parent: child has finished\n");
    }

    return 0;
}  
