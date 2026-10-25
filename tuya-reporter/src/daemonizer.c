#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <syslog.h>
#include <fcntl.h>

#include "daemonizer.h"
#include "globals.h"

static int create_pid_file(const char *pid_file){

    pid_t my_pid = getpid();
    char my_pid_str[10];

    snprintf(my_pid_str, 10, "%d", my_pid);
    my_pid_str[9] = '\0';

    snprintf(g_pidpath, sizeof(g_pidpath), "%s/%s", "/tmp", PID_NAME);

    syslog(LOG_DEBUG, "Creating pid file: %s", g_pidpath);
    if((g_pidfd = open(g_pidpath, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1){
        syslog(LOG_ERR, "Failed to open pid file: %s", g_pidpath);
        return 1;
    }

    syslog(LOG_DEBUG, "Locking pid file: %s", g_pidpath);
    if(flock(g_pidfd, LOCK_EX | LOCK_NB) == -1){
        syslog(LOG_ERR, "Failed to lock pid file: %s", g_pidpath);
        return 1;
    }

    if(write(g_pidfd, my_pid_str, strlen(my_pid_str)) == -1){
        syslog(LOG_ERR, "Failed to write pid to pid file: %s", g_pidpath);
        return 1;
    }

    return 0;
}

static int double_fork(){
    syslog(LOG_DEBUG, "Detaching process from the terminal");
    pid_t pid = fork();
    if(pid < 0){
        syslog(LOG_ERR, "fork() failed");
        return 1;
    }
    if(pid > 0)
        exit(0);
    if(setsid() < 0){
        syslog(LOG_ERR, "setsit() failed");
        return 1;
    }

    pid = fork();
    if(pid < 0){
        syslog(LOG_ERR, "fork() failed");
        return 1;
    }
    if(pid > 0){
        exit(0);
    }
    return 0;
}

static int redirect_streams(){
    if(close(STDIN_FILENO) == -1){
        syslog(LOG_ERR, "Failed to close standard stream");
        return 1;
    }
    if(close(STDOUT_FILENO) == -1){
        syslog(LOG_ERR, "Failed to close standard stream");
        return 1;
    }
    if(close(STDERR_FILENO) == -1){
        syslog(LOG_ERR, "Failed to close standard stream");
        return 1;
    }

    int fd = open("/dev/null", O_RDWR);
    if(fd == -1){
        syslog(LOG_ERR, "Failed to open: /dev/null");
        return 1;
    }

    if(dup2(fd, STDIN_FILENO) == -1){
        syslog(LOG_ERR, "Failed to redirect standard stream");
        return 1;
    }
    if(dup2(fd, STDOUT_FILENO) == -1){
        syslog(LOG_ERR, "Failed to redirect standard stream");
        return 1;
    }
    if(dup2(fd, STDERR_FILENO) == -1){
        syslog(LOG_ERR, "Failed to redirect standard stream");
        return 1;
    }
    return 0;
}

int daemonize(){
    syslog(LOG_DEBUG, "Daemonizing the program");

    if(double_fork() == 1)
        return 1;

    umask(0);

    if(chdir("/") == -1){
        syslog(LOG_ERR, "chdir() failed");
        return 1;
    }

    if(redirect_streams() == 1)
        return 1;

    return create_pid_file(PID_NAME);
}