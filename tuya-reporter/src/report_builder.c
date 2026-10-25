#include "report_builder.h"
#include <tuya-sdk/cJSON.h>

char *build_report(struct systeminfo *info, struct interface_list *list){
    cJSON *report = cJSON_CreateObject();

    cJSON *uptime = cJSON_AddObjectToObject(report, "uptime");
    cJSON_AddNumberToObject(uptime, "value", info->uptime);
    cJSON_AddNumberToObject(uptime, "time", info->uptime);

    double cpu_load = 1.0 * info->load / (1 << SI_LOAD_SHIFT);

    cJSON *cpu = cJSON_AddObjectToObject(report, "cpu");
    cJSON_AddNumberToObject(cpu, "value", cpu_load);
    cJSON_AddNumberToObject(cpu, "time", info->uptime);

    cJSON *free_ram = cJSON_AddObjectToObject(report, "free_ram");
    cJSON_AddNumberToObject(free_ram, "value", info->free_memory);
    cJSON_AddNumberToObject(free_ram, "time", info->uptime);

    cJSON *total_ram = cJSON_AddObjectToObject(report, "total_ram");
    cJSON_AddNumberToObject(total_ram, "value", info->total_memory);
    cJSON_AddNumberToObject(total_ram, "time", info->uptime);

    cJSON *interfaces = cJSON_AddArrayToObject(report, "network_interfaces");

    for(int i=0; i<list->n; i++){
        cJSON *interface = cJSON_CreateObject();
        cJSON_AddStringToObject(interface, "name", list->list[i].name);
        if(list->list[i].has_ipv4){
            cJSON_AddStringToObject(interface, "ipv4_address", list->list[i].ipv4);
            cJSON_AddNumberToObject(interface, "netmask", list->list[i].mask);
        }
        else{
            cJSON_AddNullToObject(interface, "ipv4_address");
            cJSON_AddNullToObject(interface, "netmask");
        }
        cJSON_AddItemToArray(interfaces, interface);
    }

    char *string = cJSON_Print(report);
    cJSON_Delete(report);
    return string;
}