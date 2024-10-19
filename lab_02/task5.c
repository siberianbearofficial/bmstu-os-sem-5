#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define COUNT 2
#define BUFFER_SIZE 64

static int flag = 0;

void signal_handler(int sig_numb)
{
    flag = 1;
    printf("\nSignal %d received. Sending messages is allowed.\n", sig_numb);
}

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

    signal(SIGINT, signal_handler);
    printf("Press Ctrl+C to allow sending messages\n");
    sleep(2);

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
            close(fd[0]);
            if (flag == 1)
            {
                printf("%d sent message: %s\n", getpid(), messages[i]);
                write(fd[1], messages[i], strlen(messages[i]));
            } else {
                printf("%d no signal\n", getpid());
            }

            return EXIT_SUCCESS;
        }
    }

    close(fd[1]);

    for (size_t i = 0; i < COUNT + 1; i++)
    {
        char buf[BUFFER_SIZE];
        for (int j = 0; j < BUFFER_SIZE; j++) {
            buf[j] = 0;
        }
        read(fd[0], buf, sizeof(buf));
        printf("Message received: %s\n", buf);
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

    return EXIT_SUCCESS;
}
