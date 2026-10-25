#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <argp.h>

extern struct argp argp;
error_t parse_opt(int key, char *arg, struct argp_state *state);
extern char doc[32];
extern char args_doc[32];
extern struct argp_option options[5];
struct arguments{
    char *device_id;
    char *product_id;
    char *secret;
    int daemon;
};

#endif