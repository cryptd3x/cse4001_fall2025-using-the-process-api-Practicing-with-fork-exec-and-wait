1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?

### Solution (q1.c):

```c
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
```

**Answer:** The child process starts with the same value (100) as the parent because `fork()` creates a complete copy of the parent's memory space. When each process changes a variable, they are modifying their own independent copies. As such, the changes in one process cannot affect the other process's copy of said variable.

2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently?

### Solution (q2.c):

```c
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
```

**Answer:** Yes, both child and parent can access the same file descriptor. File descriptors are shared between parent and child processes after `fork()`. When writing concurrently, the writes may be interleaved in the file, and both processes share the same file offset pointer, so writes from both processes will appear in the file.

3. Write another program using `fork()`. The child process should print "hello"; the parent process should print "goodbye". You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

### Solution (q3.c):

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if(rc == 0){
       //child process
        printf("hello\n");
    } else {
       //parent process
        sleep(1); //Give child time to run first
        printf("goodbye\n");
    }
    
    return 0;
}
```

**Answer:** It's difficult to guarantee the child process always prints first without using `wait()`. Using `sleep()` in the parent process can help but does not guarantee it. Ultimately, the OS scheduler decides the execution order, so there's no reliable way to ensure ordering without proper synchronization mechanisms like `wait()`.

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`.

### Solution (q4.c):

```c
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
        printf("Testing execvp:\n");
        char *args[] = {"ls", "-h", NULL};
        execvp("ls", args);
        printf("execvp failed\n");
        exit(1);
    } else if(rc4 > 0){
        wait(NULL);
    }
    
    return 0;
}
```

**Answer:** There are many varieties of the exec call because they provide different ways to specify arguments environment handling:
- `execl()` - takes in arguments as a list
- `execlp()` - searches the PATH for the executable
- `execle()` - allows for a custom environment
- `execv()` - takes in arguments as an array
- `execvp()` - combines array arguments with the PATH search
- `execvpe()` - combines the array arguments, PATH search, and custom environment

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

### Solution (q5.c):

```c
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
        printf("Hello, I am child (pid:%d)\n", (int) getpid());
        
       //Test wait() in child
        int child_wait_result = wait(NULL);
        printf("Child wait() returned: %d\n", child_wait_result);
        
        sleep(1);
        exit(42); //Exit with specific code
    } else {
       //parent process
        int status;
        int wait_result = wait(&status);
        printf("Hello, I am parent of %d\n", rc);
        printf("Parent wait() returned: %d\n", wait_result);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}
```

**Answer:** `wait()` returns the process ID of the child that terminated, or -1 if there are no children or an error occurred. If you use `wait()` in the child process, it returns -1 because the child has no children of its own to wait for.

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

### Solution (q6.c):

```c
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
```

**Answer:** `waitpid()` is useful when you may need to:
- Wait for a specific child process (by PID)
- Use options like WNOHANG (non-blocking wait)
- Have multiple children and want to wait for specific ones
- Need more control over which child to wait for in complex programs

7. Write a program that creates a child process, and then in the child closes standard output (STDOUT_FILENO). What happens if the child calls `printf()` to print some output after closing the descriptor?

### Solution (q7.c):

```c
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
```

**Answer:** When STDOUT_FILENO is closed in the child, calls to `printf()` and other functions that write to stdout will fail silently or have no visible effect. The output simply disappears because there's no valid file descriptor for stdout. However, stderr (STDERR_FILENO) remains open and functional.

##Compilation and Execution

To compile and run these programs, I first call bash to create a new shell, since I use zsh.

Then, I generate the executables and run them, one by one.

ex.
```bash 
gcc q1.c -o q1 && ./q1
gcc q2.c -o q2 && ./q2
gcc q3.c -o q3 && ./q3
gcc q4.c -o q4 && ./q4
gcc q5.c -o q5 && ./q5
gcc q6.c -o q6 && ./q6
gcc q7.c -o q7 && ./q7
```

Alternatively, the Makefile could be modified and utilized, I simply chose not to do so.

##Summary of Key Concepts

Question 1: Each process has a unique memory space after calling fork()
Question 2: File descriptors will be shared between the parent and child processes
Question 3: The OS controls the order of execution which makes synchronization important
Question 4: Multiple variations of exec() provide flexibility for program execution
Question 5: The wait() and waitpid() calls allow the parent to synchronize with children
Question 6: Closing file descriptors alter program behavior
Question 7: Closing file descriptors alter the std I/O streams
