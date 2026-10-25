#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

struct systeminfo{
    int uptime;
    int load;
    int free_memory;
    int total_memory;
};

struct systeminfo* get_systeminfo(struct ubus_context *ctx);

#endif