/* 
    Write another program using fork(). The child process should
    print “hello”; the parent process should print “goodbye”. You should
    try to ensure that the child process always prints first; can you do
    this without calling wait() in the parent?
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigchild_handler(int signum) 
{
    printf("goodbye\n");
    exit(0);
}

int main() 
{
    // Register BEFORE fork so we don't miss the signal if child is fast
    signal(SIGCHLD, sigchild_handler);

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1); 
    } else if (rc == 0) { // child
        printf("hello\n");
        exit(0); // Make sure child exits to trigger SIGCHLD
    } else { // parent
        // pause() waits efficiently for ANY signal to arrive
        pause(); 
    }
    return 0;
}

/*
This solution achieves the desired ordering without wait() by utilizing the SIGCHLD signal, which the operating system automatically sends to a parent process when a child terminates. 
By registering the signal handler before calling fork(), the program eliminates the race condition where a fast-executing child might exit before the parent is ready to listen. 
Instead of consuming CPU cycles with a while loop, the parent calls pause() to efficiently put itself to sleep until the child prints "hello" and exits. 
This exit action triggers the signal that wakes the parent to execute the handler and print "goodbye."
*/