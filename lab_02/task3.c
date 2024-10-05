#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COUNT 2

int main(void)
{
    pid_t child_pid[COUNT];
    char *commands[COUNT] = {"./app1.out", "./app2.out"};
    char *args1[] = {"./app1.out", NULL};
    char *args2[] = {"./app2.out", NULL};
    char **args[COUNT] = {args1, args2};

    for (size_t i = 0; i < COUNT; i++)
    {
        child_pid[i] = fork();

        if (child_pid[i] == -1)
        {
            perror("Can't fork\n");
            exit(EXIT_FAILURE);
        }

        if (child_pid[i] == 0)
        {
            printf(
                "Child: pid = %d, ppid = %d, gid = %d\n", getpid(), getppid(), getpgrp()
            );

            if (execvp(commands[i], args[i]) == -1)
            {
                perror("Can't exec\n");
                exit(1);
            }

            return EXIT_SUCCESS;
        }

        printf("Parent. Children id: %d\n", child_pid[i]);
    }

    for (size_t i = 0; i < COUNT; i++)
    {
        int status;
        waitpid(child_pid[i], &status, 0);

        if (WIFEXITED(status))
        {
            printf(
                "Child (pid: %d) exited with code %d\n", child_pid[i], WEXITSTATUS(status)
            );
        }
        else if (WIFSIGNALED(status))
        {
            printf(
                "Child (pid: %d) received signal %d\n", child_pid[i], WTERMSIG(status)
            );
        }
        else if (WIFSTOPPED(status))
        {
            printf(
                "Child (pid: %d) received signal %d\n", child_pid[i], WSTOPSIG(status)
            );
        }
    }

    return EXIT_SUCCESS;
}
