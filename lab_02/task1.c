#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT 5
#define SLEEP_TIME 10

void print_children()
{
    printf("Child: pid = %d, ppid = %d, gid = %d\n", getpid(), getppid(), getpgrp());
    sleep(  SLEEP_TIME);
    printf("Child: pid = %d, ppid = %d, gid = %d\n", getpid(), getppid(), getpgrp());
}

void print_parent(int pid)
{
    printf("Parent. Children id: %d\n", pid);
}

int loop(int *child_pid)
{
    for (size_t i = 0; i < COUNT; i++)
    {
        child_pid[i] = fork();

        switch (child_pid[i])
        {
            case -1:
            {
                return EXIT_FAILURE;
            }
            case 0:
            {
                print_children();
                return EXIT_SUCCESS;
            }
            default:
            {
                print_parent(child_pid[i]);
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

int main(void)
{
    pid_t child_pid[COUNT];

    int exit_code = loop(child_pid);

    if (exit_code != EXIT_SUCCESS)
    {
        perror("Can't fork\n");
    }

    return exit_code;
}
