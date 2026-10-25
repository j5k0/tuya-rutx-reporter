#include <syslog.h>
#include <time.h>

#include "globals.h"
#include "sdk_functions.h"
#include "tuya_handlers.h"

#include <tuya-sdk/tuya_cacert.h>
#include <tuya-sdk/tuyalink_core.h>
#include <tuya-sdk/tuya_error_code.h>

int sdk_init(tuya_mqtt_context_t* client, struct arguments args){
    int ret;

    syslog(LOG_DEBUG, "Initializing MQTT context");
    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = args.device_id,
        .device_secret = args.secret,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });

    if(ret != OPRT_OK)
        syslog(LOG_ERR, "Failed to initialize MQTT context");
    else
        syslog(LOG_ERR, "Initialized MQTT context");

    return ret;
}

void sdk_deinit(tuya_mqtt_context_t* client){
    int ret;

    syslog(LOG_DEBUG, "Deinitializing MQTT context");
    ret = tuya_mqtt_deinit(client);
    if(ret != OPRT_OK)
        syslog(LOG_ERR, "Failed to deinitialize MQTT context");
    else
        syslog(LOG_ERR, "Deinitialized MQTT context");
}

int sdk_connect(tuya_mqtt_context_t* client){
    int ret;

    syslog(LOG_DEBUG, "Connecting MQTT client to the server");
    ret = tuya_mqtt_connect(client);

    if(ret != OPRT_OK)
        syslog(LOG_ERR, "Failed to connect MQTT client to the server");
    else
        syslog(LOG_ERR, "Connected MQTT client to the server");


    return ret;
}

void sdk_disconnect(tuya_mqtt_context_t* client){
    int ret;

    syslog(LOG_DEBUG, "Disconnecting MQTT client from server");
    ret = tuya_mqtt_disconnect(client);
    if(ret != OPRT_OK)
        syslog(LOG_ERR, "Error while disconnecting from the server");
    else
        syslog(LOG_ERR, "Disconnected from the server");
}

void sdk_loop(tuya_mqtt_context_t* client, struct ubus_context *ctx, struct arguments args){
    time_t start = time(NULL);
    time_t now;

    syslog(LOG_DEBUG, "Starting MQTT loop");
    while(g_run){
        now = time(NULL);
        if(now - start >= REPORT_INTERVAL_S){
            send_report(client, ctx, args);
            start = now;
        }
        tuya_mqtt_loop(client);
    }
    syslog(LOG_DEBUG, "Finished MQTT loop");
}