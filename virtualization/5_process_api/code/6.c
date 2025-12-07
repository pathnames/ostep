/*
Write a slight modification of the previous program, this time us-
ing waitpid() instead of wait(). When would waitpid() be
useful?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
        printf("Child (pid:%d) doing work...\n", (int)getpid());
        sleep(1); // Simulate a task taking time
        printf("Child done.\n");
    }
    else {
        // PARENT PROCESS
        printf("Parent (pid:%d) waiting specifically for child (pid:%d)...\n",
               (int)getpid(), rc);

        // waitpid(pid_to_wait_for, status_pointer, options)
        // Passing 'rc' means we wait ONLY for this specific child.
        int result = waitpid(rc, NULL, 0);

        printf("Parent confirmed child %d finished.\n", result);
    }
    return 0;
}

/*
When is waitpid() useful?


waitpid() is useful (and superior to wait()) in two specific scenarios:

    Waiting for a specific child: If a parent has created 10 children but only needs the results from one specific process before continuing, wait() will fail because it returns whenever any child finishes. waitpid() allows the parent to specify exactly which PID to wait for.

    Non-blocking checks (Polling): wait() puts the parent to sleep (blocks) until a child finishes. waitpid() has an option flag called WNOHANG. This allows the parent to check "Did the child finish yet?" If the answer is no, the function returns immediately (instead of sleeping), allowing the parent to do other work in the background while the child runs.
waitpid() is useful (and superior to wait()) in two specific scenarios:

    Waiting for a specific child: If a parent has created 10 children but only needs the results from one specific process before continuing, wait() will fail because it returns whenever any child finishes. waitpid() allows the parent to specify exactly which PID to wait for.

    Non-blocking checks (Polling): wait() puts the parent to sleep (blocks) until a child finishes. waitpid() has an option flag called WNOHANG. This allows the parent to check "Did the child finish yet?" If the answer is no, the function returns immediately (instead of sleeping), allowing the parent to do other work in the background while the child runs.
waitpid() is useful (and superior to wait()) in two specific scenarios:

    Waiting for a specific child: If a parent has created 10 children but only needs the results from one specific process before continuing, wait() will fail because it returns whenever any child finishes. waitpid() allows the parent to specify exactly which PID to wait for.

    Non-blocking checks (Polling): wait() puts the parent to sleep (blocks) until a child finishes. waitpid() has an option flag called WNOHANG. This allows the parent to check "Did the child finish yet?" If the answer is no, the function returns immediately (instead of sleeping), allowing the parent to do other work in the background while the child runs.
*/