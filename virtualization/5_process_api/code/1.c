/*
Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to some-
thing (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?
*/
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int x;
    x = 100;
    printf("Value of x:(%d) in parent pid:(%d)\n", x, (int) getpid());

    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1); 
    } else if (rc == 0) { // child (new process)
        printf("Value of x:(%d) in child pid:(%d)\n", x, (int) getpid());
        x = 2;
        printf("Value of modified x:(%d) in child pid:(%d)\n", x, (int) getpid());
    } else { // parent goes down this path
        x = 1;
        printf("Value of x:(%d) in parent pid:(%d)\n", x, (int) getpid());
    }
    return 0;
}

/*
Notes: 
Terminal Output is 
Value of x:(100) in parent pid:(9649)
Value of x:(1) in parent pid:(9649)
Value of x:(100) in child pid:(9650)
Value of modified x:(2) in child pid:(9650)

This program illustrates the concept of address space isolation in operating systems by using the fork() system call to create a child process that is a duplicate of the parent with its own distinct memory segment. 
The child process inherits the initial state of the parent so it starts with x equal to 100, but the two processes remain completely independent. 
Consequently, when the parent modifies x to 1 and the child modifies it to 2, neither process affects the value stored by the other. 
*/