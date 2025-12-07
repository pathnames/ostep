/*
Write a program that creates a child process, and then in the child
closes standard output (STDOUT FILENO). What happens if the child
calls printf() to print some output after closing the descriptor?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    printf("Main process (pid:%d) started\n", (int)getpid());
    
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        // CHILD PROCESS
        printf("Child about to close STDOUT...\n");
        
        // Close the file descriptor 1 (Standard Output)
        close(STDOUT_FILENO);
        
        // Attempt to print
        int ret = printf("Child (pid:%d) doing work after close...\n", (int) getpid());
        
        // Check the result
        // We write to STDERR (fd 2) because fd 1 is closed.
        if (ret < 0) {
            fprintf(stderr, "Child: printf failed! Return val: %d, Error: %s\n", 
                    ret, strerror(errno));
        } else {
            // Note: Sometimes printf buffers and doesn't fail immediately until flush
            fprintf(stderr, "Child: printf executed but output is lost.\n");
        }
    }   
    else {
        // PARENT PROCESS
        // Wait for child so terminal prompt doesn't mix with output
        wait(NULL);
    }
    return 0;
}

/*
When a process calls close(STDOUT_FILENO), it deletes the link between its file descriptor 1 and the terminal window. 
Although the C standard library's printf buffer (stdout stream) still exists in the program's memory, any attempt to flush that buffer to the OS—whether triggered by a newline character or the program exiting—will result in a write error. 
Consequently, the output is silently discarded, and if checked, errno is set to EBADF (Bad File Descriptor).
*/