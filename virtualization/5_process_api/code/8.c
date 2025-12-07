/*
Write a program that creates a child process, and then in the child
closes standard output (STDOUT FILENO). What happens if the child
calls printf() to print some output after closing the descriptor?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Main process (pid:%d) started\n", (int)getpid());
    int pipefd[2]; // pipefd[0] is read, pipefd[1] is write
    
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(1);
    }

    // --- Create Child 1 ---
    int rc1 = fork();
    if (rc1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc1 == 0) { 
        // CHILD 1 (The Writer)
        
        // Close the read-end (unused by this child)
        close(pipefd[0]);

        char msg[] = "Hello from child 1";
        printf("Child 1 sending message...\n");
        write(pipefd[1], msg, sizeof(msg));
        
        // Close write-end to signal "I am done writing"
        close(pipefd[1]); 
        exit(0);
    } 

    // --- Create Child 2 ---
    int rc2 = fork();
    if (rc2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc2 == 0) { 
        // CHILD 2 (The Reader) -- FIXED VAR NAME HERE (rc2)
        
        // Close the write-end (unused by this child)
        close(pipefd[1]);

        char buffer[100];
        // Read blocks until data comes or pipe closes
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        
        if (bytes_read > 0) {
            // Safety: Ensure string is null-terminated
            // (Only necessary if sender didn't send the \0)
            // buffer[bytes_read] = '\0'; 
            printf("Child 2 received: [%s]\n", buffer);
        } else {
            printf("0 bytes read\n");
        }
        
        close(pipefd[0]);
        exit(0);
    } 
    
    // --- Parent Process ---
    
    // CRITICAL: Parent must close pipe ends!
    // Otherwise Child 2 will hang waiting for Parent to write.
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children
    wait(NULL);
    wait(NULL);
    
    printf("Parent done.\n");
    return 0;
}
