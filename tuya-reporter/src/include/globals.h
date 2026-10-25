#ifndef GLOBALS_H
#define GLOBALS_H

#include <linux/limits.h>
#include <stdio.h>
#include <signal.h>
#include <libubus.h>

extern sig_atomic_t g_run;
extern char g_pidpath[PATH_MAX];
extern int g_pidfd;
extern FILE *action_log;

#endif
