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
        printf("Hello, I am the child (pid:%d)\n", (int) getpid());
        sleep(2);
        exit(42);
    } else {
       //parent process
        int status;
        printf("Parent waiting for specific child %d\n", rc);

       //Wait for specific child with WNOHANG option
        int wait_result = waitpid(rc, &status, 0);

        printf("Hello, I am parent of %d\n", rc);
        printf("waitpid() returned: %d\n", wait_result);
        if(WIFEXITED(status)){
            printf("Child exit status: %d\n", WEXITSTATUS(status));
        }
    }
     
    return 0;
}  
