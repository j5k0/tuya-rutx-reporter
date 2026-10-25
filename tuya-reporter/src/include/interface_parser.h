#ifndef INTERFACE_PARSER_H
#define INTERFACE_PARSER_H

#include <arpa/inet.h>
#include <netdb.h>

#define MAX_INTERFACES 32

struct interface {
    char name[NI_MAXHOST];
    int has_ipv4;
    char ipv4[INET_ADDRSTRLEN];
    int mask;
};

struct interface_list{
    struct interface list[MAX_INTERFACES];
    int n;
};

void get_networkint(struct interface_list *list, struct ubus_context *ctx);

#endif