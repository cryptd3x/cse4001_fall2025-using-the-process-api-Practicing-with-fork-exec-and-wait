#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int fd = open("./shared_file.txt", O_CREAT|O_WRONLY|O_TRUNC, 0644);
    if(fd < 0){
        fprintf(stderr, "open failed\n");
        exit(1);
    }

    printf("File opened with fd: %d (pid:%d)\n", fd, (int) getpid());

    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if(rc == 0){
       //child process
        char child_msg[] = "Child process writing\n";
        for (int i = 0; i < 5; i++){
            write(fd, child_msg, strlen(child_msg));
            usleep(1000); //Small delay
        }
        close(fd);
    } else {
       //parent process
        char parent_msg[] = "Parent process writing\n";
        for (int i = 0; i < 5; i++){
            write(fd, parent_msg, strlen(parent_msg));
            usleep(1000); //Small delay
        }
        wait(NULL);
        close(fd);
    }

    return 0;
}
