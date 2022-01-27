#ifndef _SERVER_RECEIVE_H_
#define _SERVER_RECEIVE_H_

typedef struct
{
    char code[5];
    char state[5];
    char state_uuid[10];
    char state_local[10];
    char state_state[5];
    char state_last[20];
    char state_user[20];
    char state_start[20];
    char state_finish[20];
    char time[20];
    char control[100];
    char reserved[5];
} server_receive_data_t;

extern server_receive_data_t http_rec;

typedef struct
{
    char *deviceID;
    int state;
    char *user;
    float voltage;
    float current;
    float power;
    float electricity;
} transmit_server_data_t;

server_receive_data_t server_char_parse( char *json_char);
void server_data_build(char *TX);
//char *server_data_build(transmit_server_data_t transmit_data);

#endif /* _SERVER_RECEIVE_H_ */