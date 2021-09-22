#include <stdio.h>
#include <signal.h>
#include <limits.h>
#include <wait.h>

void catch_child(int signo)
{
    printf("Child exit!\n");               // this call may reenter printf/puts! BAD! DEADLOCK
    while (waitpid(-1, NULL, WNOHANG) > 0) // reap all children
        continue;
}

int main()
{
    char buf[1024];
    int i;
    signal(SIGCHLD, catch_child);
    for (i = 0; i < 1000000; ++i)
    {
        if (fork() == 0)
        { // in child, exit immediately
            exit(0);
        }
        // in parent
        sprintf(buf, "Child #%d started\n", i);
        printf("%s", buf);
    }
    return 0;
}