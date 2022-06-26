#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Device_Info.h"
#include"gpio.h"
#include"cJSON.h"
#include"im1281b.h"
#include "Euclidean.h"

#include"server_receive_transmit.h"

//{"code":0,"id":1,"location":"422.1","type":1,"state":2,"serverTime":1612610971003,"user":"Zuoge","start":1111,"end":1234}
//{code:0,state:{uuid:1,local:"422.1",state:0,last:1632240859000,user:0,start:null,finish:null},time:1632240862967}

/**解析服务器发来的json**/
server_receive_data_t server_char_parse( char * json_char)
{   
    server_receive_data_t server_data = {0};

    cJSON * root = NULL;
    cJSON * item = NULL;

    char * index = strchr(json_char , '{');

    
    //strcpy(json_char , index);
    //printf("%s\n" , index);

    root = cJSON_Parse(index);

    if(!root)
    {
        printf("cJSON Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
        printf("cJSON is:\r\n%s\n", cJSON_PrintUnformatted(root)); //无格式方式打印json
        
        item = cJSON_GetObjectItem(root, "time");
        sprintf(server_data.time , "%s" , cJSON_Print(item));
        item = cJSON_GetObjectItem(root, "code");
        sprintf(server_data.code , "%s" , cJSON_Print(item));
        item = cJSON_GetObjectItem(root, "state");
        sprintf(server_data.state , "%s" , cJSON_Print(item));   

        if(strcmp(server_data.state , "0") == 0)
        {
            item = cJSON_GetObjectItem(root, "reserved");
            sprintf(server_data.reserved , "%s" , cJSON_Print(item)); 

            if(strcmp(server_data.reserved , "0") == 0)
            {
                sprintf(server_data.state_state , "0");  
            }
            else if(strcmp(server_data.reserved , "1") == 0)
            {
                sprintf(server_data.state_state , "1");
                item = cJSON_GetObjectItem(root, "userAccount");
                sprintf(server_data.state_uuid , "%s" , item->valuestring);
                item = cJSON_GetObjectItem(root, "userName");
                sprintf(server_data.state_user , "%s" , item->valuestring); 
            }
        }
        else if(strcmp(server_data.state , "1") == 0)
        {
            item = cJSON_GetObjectItem(root, "control");
            sprintf(server_data.control , "%s" , cJSON_Print(item));
            item = cJSON_GetObjectItem(root, "userAccount");
            //sprintf(server_data.state_uuid , "%s" , cJSON_Print(item));
            sprintf(server_data.state_uuid , "%s" , item->valuestring);
            item = cJSON_GetObjectItem(root, "userName");
            //sprintf(server_data.state_user , "%s" , cJSON_Print(item));  
            sprintf(server_data.state_user , "%s" , item->valuestring); 
            sprintf(server_data.state_state , "2");
        }

    }
    if(root)
    cJSON_Delete(root);
    return server_data;
}

/**构建发送至服务器的json包**/
void server_data_build(char *TX)
{
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();
    cJSON_AddItemToObject(root, "uuid", cJSON_CreateNumber(DEV_UUID));
    cJSON_AddItemToObject(root, "local", cJSON_CreateNumber(DEV_LOCAL));
    cJSON_AddItemToObject(root, "state", cJSON_CreateNumber(relay_state));
    cJSON_AddItemToObject(root, "user", next);
    cJSON_AddItemToObject(next, "name", cJSON_CreateString("NULL"));
    cJSON_AddItemToObject(next, "id", cJSON_CreateString("NULL"));
    cJSON_AddItemToObject(root, "vlotage", cJSON_CreateNumber(im1281b_data->voltage));
    cJSON_AddItemToObject(root, "current", cJSON_CreateNumber(im1281b_data->current));
    cJSON_AddItemToObject(root, "apower", cJSON_CreateNumber(im1281b_data->power));
    cJSON_AddItemToObject(root, "aelectricity", cJSON_CreateNumber(im1281b_data->electricity));
    cJSON_AddItemToObject(root, "powerfactor", cJSON_CreateNumber(im1281b_data->power_factor));
    //cJSON_AddItemToObject(root, "analysis", cJSON_CreateString(elecapp_state));
    cJSON_AddItemToObject(root, "analysis", item);
    cJSON_AddItemToObject(item, "charger", cJSON_CreateNumber((double)charger_state_bit));
    cJSON_AddItemToObject(item, "lamp", cJSON_CreateNumber((double)lamp_state_bit));
    cJSON_AddItemToObject(item, "soldering", cJSON_CreateNumber((double)soldering_state_bit));
    cJSON_AddItemToObject(item, "unknown", cJSON_CreateNumber((double)unknown_state_bit));

    char* pstr = cJSON_Print(root);

    sprintf(TX, "%s", pstr);
    free(pstr);
    //printf("package ok \r\n");
    // if(next)
    //     cJSON_Delete(next);
    if(root)
    {
        //cJSON_Delete(next);
        cJSON_Delete(root);
    }
}

void analysis_data_build(char* data)
{

}