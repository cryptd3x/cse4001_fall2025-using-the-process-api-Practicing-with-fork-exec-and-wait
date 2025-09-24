#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    printf("Testing different exec variants:\n");

   //Tests execl
    int rc1 = fork();
    if(rc1 == 0){
        printf("Testing execl:\n");
        execl("/bin/ls", "ls", "-l", NULL);
        printf("execl failed\n");
        exit(1);
    } else if(rc1 > 0){
        wait(NULL);
    }
     
   //Tests execlp
    int rc2 = fork();
    if(rc2 == 0){
        printf("Testing execlp:\n");
        execlp("ls", "ls", "-a", NULL);
        printf("execlp failed\n");
        exit(1);
    } else if(rc2 > 0){
        wait(NULL);
    }
     
   //Tests execv
    int rc3 = fork();
    if(rc3 == 0){
        printf("Testing execv:\n");
        char *args[] = {"ls", "-l", "-a", NULL};
        execv("/bin/ls", args);
        printf("execv failed\n");
        exit(1);
    } else if(rc3 > 0){
        wait(NULL);
    }
     
   //Tests execvp
    int rc4 = fork();
    if(rc4 == 0){
        printf("Testng execvp:\n");
        char *args[] = {"ls", "-h", NULL};
        execvp("ls", args);
        printf("execvp failed\n");
        exit(1);
    } else if(rc4 > 0){
        wait(NULL);
    }
     
    return 0;
}  
