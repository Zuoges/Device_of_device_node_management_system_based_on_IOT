#ifndef _EUCLIDEAN_H_
#define _EUCLIDEAN_H_

#include "im1281b.h"

#define DIMENSION 3         //欧几里得距离维度
#define PRECISION 3.5       //欧几里得距离精度
#define ELECAPP_NUM 4
#define SOLDERING_STATE_NUM 3
#define CHARA_NUM 5

#define VLOTAGE 0
#define CURRENT 1
#define APOWER 2
#define AELECTRICITY 3
#define POWERFACTOR 4

#define ERRORELEC -1
#define CHARGER 0
#define LAMP 1
#define NOAPP 2
#define LAMP_CHARGER 3

#define SOLDERING_no 0
#define SOLDERING_down 1
#define SOLDERING_up 2
#define SOLDERING_steady 3

#define ELECAPP_off 0
#define ELECAPP_on 1

extern char elecapp_state_json[1024];

extern char soldering_state[100];
extern char elecapp_state[100];

extern int soldering_state_bit;
extern int lamp_state_bit;
extern int charger_state_bit;
extern int unknown_state_bit;

extern float min_d;
extern int min_i;
extern int min_s;
extern float d[SOLDERING_STATE_NUM][ELECAPP_NUM];

typedef struct
{
    float charger[5];
    float soldering[5];
    float powersupply[5];
}elecapp_t;

void status_parse(im1281b_data_t *data);
void euclidean_data_init();

#endif