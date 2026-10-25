#include "argument_parser.h"
#include <string.h>

char doc[] = "Tuya Reporter";

char args_doc[] = "";

struct argp_option options[] = {
    {"daemon", 'D', 0, 0, "Run the program as a daemon"},
    {"device", 'd', "ID", 0, "Device ID (Required)"},
    {"product", 'p', "ID", 0, "Product ID (Required)"},
    {"secret", 's', "SECRET", 0, "Device secret (Required)"},
    { 0 }
};

static int check_empty(char *string){
    int len = strlen(string);
    for(int i=0; i<len; i++){
        if(string[i] != ' ' && string[i] != '\n' && string[i] != '\t')
            return 0;
    }
    return 1;
}

error_t parse_opt(int key, char *arg, struct argp_state *state){
    struct arguments *arguments = (struct arguments*)state->input;
    switch(key){
        case 'D':
            arguments->daemon = 1;
            break;
        case 'd':
            if(check_empty(arg) == 0)
                arguments->device_id = arg;
            break;
        case 'p':
            if(check_empty(arg) == 0)
                arguments->product_id = arg;
            break;
        case 's':
            if(check_empty(arg) == 0)
                arguments->secret = arg;
            break;
        case ARGP_KEY_END:
            if(arguments->device_id == NULL)
                argp_error(state, "argument --device=ID is required");
            if(arguments->product_id == NULL)
                argp_error(state, "argument --product=ID is required");
            if(arguments->secret == NULL)
                argp_error(state, "argument --secret=SECRET is required");
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

struct argp argp = {
    options, 
    parse_opt, 
    args_doc, 
    doc
};
