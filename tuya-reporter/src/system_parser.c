#include <libubus.h>
#include <syslog.h>

#include "system_parser.h"

enum{
    LOCALTIME,
    UPTIME,
    LOAD,
    MEMORY,
    SWAP,
    __MAX_FIELDS
};

static const struct blobmsg_policy sysinfo_policy[] = {
    [LOCALTIME] = { .name = "localtime", .type = BLOBMSG_TYPE_INT64 },
    [UPTIME] = { .name = "uptime", .type = BLOBMSG_TYPE_INT32 },
    [LOAD] = { .name = "load", .type = BLOBMSG_TYPE_ARRAY },
    [MEMORY] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
    [SWAP] = { .name = "swap", .type = BLOBMSG_TYPE_TABLE }
};

static void system_info_cb(struct ubus_request *reg, int type, struct blob_attr *msg){
    if(!msg){
        reg->priv = NULL;
        return;
    }

    struct blob_attr *tb[__MAX_FIELDS];
    blobmsg_parse(sysinfo_policy, __MAX_FIELDS, tb, blob_data(msg), blob_len(msg));

    struct systeminfo *info = (struct systeminfo *)reg->priv;

    if(tb[UPTIME]){
        info->uptime = blobmsg_get_u32(tb[UPTIME]);
    }

    if(tb[LOAD]){
        struct blob_attr *cur;
        size_t rem;
        blobmsg_for_each_attr(cur, tb[LOAD], rem) {
            if (blobmsg_type(cur) != BLOBMSG_TYPE_INT32) {
                continue;
            }
            info->load = blobmsg_get_u32(cur);
            break;
        }
    }

    if(tb[MEMORY]){
        struct blob_attr *cur;
        size_t rem;
        blobmsg_for_each_attr(cur, tb[MEMORY], rem) {
            const char* name = blobmsg_name ( cur );
            if ( strcmp ( "total", name ) == 0 ) {
                info->total_memory = blobmsg_get_u64(cur);
            } else if ( strcmp ( "free", name ) == 0 ) {
                info->free_memory = blobmsg_get_u64(cur);
            }
        }
    }
}

struct systeminfo *get_systeminfo(struct ubus_context *ctx){
    syslog(LOG_DEBUG, "Getting system information");

    uint32_t id;

    struct blob_buf buf = {0};

    int ret = ubus_lookup_id(ctx, "system", &id);
    if(ret){
        syslog(LOG_ERR, "Failed to look up ubus object: system");
        return NULL;
    }

    blob_buf_init(&buf, 0);

    struct systeminfo *info = malloc(sizeof(struct systeminfo));

    ubus_invoke(ctx, id, "info", buf.head, system_info_cb, info, 3000);
    syslog(LOG_DEBUG, "invoke done");

    blob_buf_free(&buf);

    return info;
}