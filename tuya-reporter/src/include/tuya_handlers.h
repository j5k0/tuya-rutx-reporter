#ifndef TUYA_HANDLERS_H
#define TUYA_HANDLERS_H

#include "argument_parser.h"
#include <tuya-sdk/tuyalink_core.h>
#include <libubus.h>

void send_report(tuya_mqtt_context_t* context, struct ubus_context *ctx, struct arguments args);
void on_connected(tuya_mqtt_context_t* context, void* user_data);
void on_disconnect(tuya_mqtt_context_t* context, void* user_data);
void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg);

#endif