/* 
    Write a program that calls fork() and then calls some form of
    exec() to run the program /bin/ls. See if you can try all of the
    variants of exec(), including (on Linux) execl(), execle(),
    execlp(), execv(), execvp(), and execvpe(). Why do
    you think there are so many variants of the same basic call?
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



int main() 
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1); 
    } else if (rc == 0) { // child
        printf("child pid: (%d)\n", (int) getpid());
        execl("/bin/ls", "ls", (char *) NULL);
    } else { // parent
        printf("parent pid: (%d)\n", (int) getpid());
        execl("/bin/ls", "ls", (char *) NULL);
    }
    return 0;
}

/*

Explanation of Variants:

    l vs. v:
    Functions ending in l take arguments as a list of separate strings (e.g., execl(path, arg0, arg1, ..., NULL)), while functions ending in v take arguments as a null-terminated array of strings (e.g., execv(path, argv)).
    p:
    Functions with p (e.g., execlp, execvp) search for the executable in the directories specified by the PATH environment variable if the provided file argument does not contain a slash (/).
    e:
    Functions with e (e.g., execle, execvpe) allow you to explicitly pass a custom environment block (an array of KEY=VALUE strings) to the new process, rather than inheriting the parent's environment.
*/