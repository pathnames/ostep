/* 
    Now write a program that uses wait() to wait for the child process
    to finish in the parent. What does wait() return? What happens if
    you use wait() in the child?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    printf("Main process (pid:%d) started\n", (int) getpid());

    int rc = fork();

    if (rc < 0) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1); 

    } else if (rc == 0) { 
        // CHILD PROCESS
        printf("Child (pid:%d) running. Calling wait()...\n", (int) getpid());
        
        // TEST: Calling wait() inside the child
        int child_wait_rc = wait(NULL);
        
        printf("Child wait() returned: %d (Error implies no children)\n", child_wait_rc);

    } else { 
        // PARENT PROCESS
        printf("Parent (pid:%d) waiting for child (pid:%d)...\n", (int) getpid(), rc);
        
        // STANDARD USAGE: Parent waits for child
        int parent_wait_rc = wait(NULL);
        
        printf("Parent wait() returned: %d\n", parent_wait_rc);
    }
    return 0;
}

/*
The wait() system call is designed to pause a parent process until one of its children terminates, at which point it returns the Process ID (PID) of the finished child to facilitate identification and cleanup. 
However, if wait() is called within the child process itself, it generally returns -1 immediately. 
This error occurs because the newly created child has not spawned any children of its own, leading the operating system to report that there are no child processes to wait for.
*/