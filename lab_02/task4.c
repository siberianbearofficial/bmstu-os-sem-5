#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define COUNT 2
#define BUFFER_SIZE 64

int main(void)
{
    pid_t child_pid[COUNT];
    char *messages[COUNT] = {"aaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbb"};

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Can't pipe\n");
        exit(EXIT_FAILURE);
    }

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
            printf("Child: pid = %d, ppid = %d, gid = %d\n", getpid(), getppid(), getpgrp());
            printf("Child (pid: %d) sent message: %s\n", getpid(), messages[i]);
            close(fd[0]);
            write(fd[1], messages[i], strlen(messages[i]));

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
            printf("Child (pid: %d) exited with code %d\n", child_pid[i], WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child (pid: %d) received signal %d\n", child_pid[i], WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Child (pid: %d) received signal %d\n", child_pid[i], WSTOPSIG(status));
        }
    }

    printf("Messages from children: \n");

    for (size_t i = 0; i < COUNT; i++)
    {
        char buf[BUFFER_SIZE];
        close(fd[1]);
        read(fd[0], buf, strlen(messages[i]));
        printf("%s\n", buf);
    }

    return EXIT_SUCCESS;
}
