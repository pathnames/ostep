/*
Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
*/
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("./p2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);    
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1); 
    } else if (rc == 0) { // child (new process)
        char *child_msg = "Child writing to shared fd\n";
        // Write directly to the inherited file descriptor
        write(fd, child_msg, strlen(child_msg)); 
    } else { // parent goes down this path
        char *parent_msg = "Parent writing to shared fd\n";
        // Write directly to the same file descriptor
        write(fd, parent_msg, strlen(parent_msg));
    }
    return 0;
}

/*
Notes: 
What happens when they are writing to the file concurrently?

Shared Offset: Since fork() duplicates the file descriptor table, both the parent and child fd point to the same entry in the system-wide open file table. This means they share the file offset.

The Result: When the Parent writes, the file offset moves forward. When the Child writes, it writes starting at that new offset.

Outcome: The output will generally be appended cleanly (e.g., "Parent...Child..." or "Child...Parent..."). They will not overwrite each other's data (unless you explicitly used lseek to move the pointer back). However, the order of the lines depends on the scheduler.
*/