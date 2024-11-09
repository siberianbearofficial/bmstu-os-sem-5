#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

#define CONFFILE "/etc/my_daemon.conf"
#define LOCKFILE "/var/run/my_daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define SLEEP_TIME 10

sigset_t mask;

void reread(void)
{
    int fd;
    long uid;
    char uname[128];
    char buf[128];

    fd = open(CONFFILE, O_RDONLY);
    if (fd == -1)
    {
        syslog(LOG_ERR, "unable to open configuration file: %s", strerror(errno));
        return;
    }

    ssize_t rbytes = read(fd, buf, 128 - 1);
    if (rbytes == -1)
    {
        syslog(LOG_ERR, "unable to read configuration file: %s", CONFFILE);
        close(fd);
        return;
    }

    buf[rbytes] = '\0';

    if (sscanf(buf, "%ld %s", &uid, uname) == 2)
    {
        syslog(LOG_INFO, "Configuration read: username = %s, userID = %ld", uname, uid);

        if (setuid(uid) == -1)
            syslog(LOG_ERR, "Failed to set userID to %ld: %s", uid, strerror(errno));
        else
            syslog(LOG_INFO, "Successfully set userID to %ld", uid);
    }
    else
        syslog(LOG_ERR, "unable to read configuration file: %s", CONFFILE);

    close(fd);
}

int lockfile(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;

    return fcntl(fd, F_SETLK, &fl);
}

int already_running(void)
{
    int fd;
    char buf[16];
    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);

    if (fd < 0)
    {
        syslog(LOG_ERR, "Cannot open %s: %s", LOCKFILE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (lockfile(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            return 1;
        }

        syslog(LOG_ERR, "Cannot set a lock on %s: %s", LOCKFILE, strerror(errno));

        exit(EXIT_FAILURE);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long) getpid());
    write(fd, buf, strlen(buf) + 1);

    return 0;
}

void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        printf("%s: it is impossible to get the maximum descriptor number ", cmd);
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1)
    {
        printf("%s: fork function call error", cmd);
        exit(EXIT_FAILURE);
    }

    if (pid != 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
    {
        printf("%s: setsid function call error", cmd);
    }

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        printf("%s: cannot ignore SIGHUP signal", cmd);
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0)
    {
        printf("%s: cannot be made the current working directory /", cmd);
        exit(EXIT_FAILURE);
    }

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;

    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "invalid file descriptors %d %d %d", fd0, fd1, fd2);
        exit(EXIT_FAILURE);
    }
}

void *thr_fn(void *arg)
{
    int signumb;

    for (;;)
    {
        if (sigwait(&mask, &signumb) != 0)
        {
            syslog(LOG_ERR, "sigwait() error");
            exit(EXIT_FAILURE);
        }
        switch (signumb)
        {
            case SIGHUP:
                syslog(LOG_INFO, "Reading config file.");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO, "Signal SIGTERM received. Exiting");
                exit(EXIT_SUCCESS);
            default:
                syslog(LOG_INFO, "Unknown signal received: %d\n", signumb);
        }
    }

    return EXIT_SUCCESS;
}

int main(void)
{
    time_t cur_time;
    struct tm *tinfo;
    int err;
    pthread_t tid;
    struct sigaction sa;

    daemonize("my_daemon");

    if (already_running() != 0)
    {
        syslog(LOG_ERR, "my_daemon is already running.\n");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        printf("Cannot recovery action SIG_DFL for SIGHUP\n");
        exit(EXIT_FAILURE);
    }

    sigfillset(&mask);

    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
    {
        printf("Cannot block signals");
        exit(err);
    }

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != EXIT_SUCCESS)
    {
        printf("Cannot create thread");
        exit(err);
    }

    for (;;)
    {
        time(&cur_time);
        tinfo = localtime(&cur_time);
        syslog(LOG_NOTICE, "Time: %.2d:%.2d:%.2d\n", tinfo->tm_hour, tinfo->tm_min, tinfo->tm_sec);
        sleep(SLEEP_TIME);
    }

    return EXIT_SUCCESS;
}
