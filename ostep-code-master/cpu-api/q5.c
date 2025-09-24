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

       //Test wait() in child
        int child_wait_result = wait(NULL);
        printf("Child wait() returned: %d\n", child_wait_result);

        sleep(1);
        exit(42); //Exit with specific code
    } else {
       //parent process
        int status;
        int wait_result = wait(&status);
        printf("Hello, I am the parent of %d\n", rc);
        printf("Parent wait() returned: %d\n", wait_result);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
