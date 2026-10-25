#ifndef REPORT_BUILDER_H
#define REPORT_BUILDER_H

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/sysinfo.h>
#include "system_parser.h"
#include "interface_parser.h"

char *build_report(struct systeminfo *info, struct interface_list *list);

#endif