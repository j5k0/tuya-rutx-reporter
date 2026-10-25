#include "libubox/blob.h"
#include "libubox/blobmsg_json.h"
#include <libubus.h>
#include <syslog.h>
#include <tuya-sdk/cJSON.h>

#include "interface_parser.h"

static void parse_array(cJSON *array, struct interface_list *list){
    cJSON *interface;
    cJSON_ArrayForEach(interface, array){
        if(list->n >= MAX_INTERFACES)
            break;

        cJSON *name = cJSON_GetObjectItem(interface, "interface");
        if(strcmp(name->valuestring, "loopback") == 0)
            continue;

        strncpy(list->list[list->n].name, name->valuestring, NI_MAXHOST);

        cJSON *ipv4_array = cJSON_GetObjectItem(interface, "ipv4-address");
        cJSON *ipv4 = NULL;
        if(ipv4_array != NULL)
            ipv4 = cJSON_GetArrayItem(ipv4_array, 0);
        if(ipv4 != NULL){
            cJSON *address = cJSON_GetObjectItem(ipv4, "address");
            cJSON *mask = cJSON_GetObjectItem(ipv4, "mask");
            list->list[list->n].has_ipv4 = 1;
            strncpy(list->list[list->n].ipv4, address->valuestring, INET_ADDRSTRLEN);
            list->list[list->n].mask = mask->valueint;
        }

        list->n = list->n + 1;
    }
}

static void networkint_cb(struct ubus_request *reg, int type, struct blob_attr *msg){
    if(!msg){
        reg->priv = NULL;
        return;
    }
    char *json = blobmsg_format_json(msg, true);
    cJSON *data = cJSON_Parse(json);
    if(data != NULL){
        cJSON *array = cJSON_GetObjectItem(data, "interface");
        parse_array(array, reg->priv);
    }
}

void get_networkint(struct interface_list *list, struct ubus_context *ctx){
    syslog(LOG_DEBUG, "Getting system information");

    uint32_t id;

    struct blob_buf buf = {0};

    int ret = ubus_lookup_id(ctx, "network.interface", &id);
    if(ret){
        syslog(LOG_ERR, "Failed to look up ubus object: network.interfac");
        return;
    }

    blob_buf_init(&buf, 0);

    ubus_invoke(ctx, id, "dump", buf.head, networkint_cb, list, 3000);
    syslog(LOG_DEBUG, "invoke done");

    blob_buf_free(&buf);

}