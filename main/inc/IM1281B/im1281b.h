#ifndef _IM1281B_H_
#define _IM1281B_H_

typedef struct
{
    float voltage;
    float current;
    float power;
    float electricity;
    float power_factor;
    float co2;
} im1281b_data_t;

extern im1281b_data_t *im1281b_data;

void im1281b_init();

#endif