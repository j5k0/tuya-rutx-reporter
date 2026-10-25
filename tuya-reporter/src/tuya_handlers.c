
#include <tuya-sdk/cJSON.h>
#include <tuya-sdk/tuyalink_core.h>
#include <tuya-sdk/tuya_log.h>

#include <stdlib.h>
#include <syslog.h>

#include "globals.h"
#include "report_builder.h"
#include "interface_parser.h"
#include "system_parser.h"
#include "tuya_handlers.h"

void send_report(tuya_mqtt_context_t* context, struct ubus_context *ctx, struct arguments args){
    struct systeminfo *sysinfo = get_systeminfo(ctx);
    struct interface_list *list = (struct interface_list *)malloc(sizeof(struct interface_list));
    get_networkint(list, ctx);
    if(sysinfo != NULL){
        char *string = build_report(sysinfo, list);
        tuyalink_thing_property_report(context, args.device_id, string);
        free(string);
        free(sysinfo);
        free(list);
    }
}

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_INFO, "Connected to Tuya cloud");
    // TY_LOGI("on connected");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_INFO, "Disconnected from Tuya cloud");
    // TY_LOGI("on disconnect");
}

static void log_tuya_action(cJSON *params, char *action){
    if(action_log != NULL){
        cJSON *text_obj = cJSON_GetObjectItem(params, "input");
        if(text_obj != NULL){
            fprintf(action_log, "%s\n", text_obj->valuestring);
        }
        else
            syslog(LOG_ERR, "Failed to parse action: %s", action);
    }
}

static void parse_tuya_action(char *json){
    cJSON *data = cJSON_Parse(json);
    if(data != NULL){
        cJSON *params = cJSON_GetObjectItem(data, "inputParams");
        cJSON *action = cJSON_GetObjectItem(data, "actionCode");
        if(strcmp(action->valuestring, "message") == 0)
            log_tuya_action(params, action->valuestring);
    }
    else{
        syslog(LOG_ERR, "Failed to parse json %s", json);
    }
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    syslog(LOG_INFO, "Received message id: %s type: %d code %d", msg->msgid, msg->type, msg->code);
    // TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_ACTION_EXECUTE:
            syslog(LOG_INFO, "Action message msgid: %s msg: %s", msg->msgid, msg->data_string);
            parse_tuya_action(msg->data_string);
            break;
        default:
            break;
    }
    printf("\r\n");
}