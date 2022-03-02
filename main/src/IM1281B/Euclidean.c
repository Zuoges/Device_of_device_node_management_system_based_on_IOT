#include <stdio.h>
#include <math.h>
#include "im1281b.h"
#include "lvgl_gui.h"
#include "server_receive_transmit.h"
#include "Euclidean.h"

char elecapp_state[100] = "";
char soldering_state[100] = "";

float elecapp[SOLDERING_STATE_NUM][ELECAPP_NUM][CHARA_NUM];
float elecapp_SD[SOLDERING_STATE_NUM][CHARA_NUM];

int soldering_angle = 0;
int soldering_ground = 0;
int soldering_plateau = 0;
int soldering_have = 0;

int soldering_state_bit = SOLDERING_no;
int lamp_state_bit = ELECAPP_off;
int charger_state_bit = ELECAPP_off;
int unknown_state_bit = ELECAPP_off;

void Charger_img_set(uint8_t state);
void Lamp_img_set(uint8_t state);
void Solder_img_set(uint8_t state);
void Unknown_img_set(uint8_t state);

/***电烙铁状态判别***/
void soldering_parse(im1281b_data_t *data)
{
    static float old_p;
    static float now_p;
    
    static int up;
    static int down;

    float dp = 0;
    now_p = data->power;
    dp = now_p - old_p;

    if(dp > 15)
    {
        if(soldering_have == 0)
            soldering_have = 1;
        if(up == 0)
        {
            up = 1;
            down = 0;
            soldering_ground = 0;
        }
    }
    else if(dp < -15)
    {
        if(down == 0)
        {
            down = 1;
            up = 0;
            if(soldering_plateau < 3 *5) //3*5
            {
                soldering_angle++;
            }
            soldering_plateau = 0;
        }
    }
    else if(dp >= -10 && dp <= 10)
    {
        if(down == 1)
        {
            soldering_ground++;
        }
        else if(up == 1)
        {
            soldering_plateau++;
        }
    }

    if(soldering_ground > 15) //6*5
    {
        if(soldering_ground > 30 *5) //30*5
        {
            //printf("soldering off\r\n");
            sprintf(soldering_state , "电烙铁关闭");
            soldering_state_bit = SOLDERING_no;
        }
        else
        {
            //printf("soldering down\r\n");
            sprintf(soldering_state , "电烙铁降温中");
            soldering_state_bit = SOLDERING_down;
        }
        soldering_angle = 0;
    }
    else if(soldering_plateau > 10) //3*5
    {
        //printf("soldering up\r\n");
        sprintf(soldering_state , "电烙铁加热中");
        soldering_state_bit = SOLDERING_up;
        soldering_angle = 0;
    }
    else if(soldering_angle > 0)
    {
        //printf("soldering no up no down\r\n");
        sprintf(soldering_state , "电烙铁恒温中");
        soldering_state_bit = SOLDERING_steady;
    }
    else
    {
        //printf("soldering error\r\n");
        //sprintf(soldering_state , "电烙铁错误");
    }
    Solder_img_set(soldering_state_bit);
    old_p = now_p;
}

    float min_p = 0;
    int min_j = 0;
    float dd[4][ELECAPP_NUM] = {0};
    int down_num = 0;
    int down_down = 0;
    float min_d = 0;
    int min_i = 0;
    int min_s = 0;
    float d[SOLDERING_STATE_NUM][ELECAPP_NUM] = {0};

void status_parse(im1281b_data_t *data)
{
    soldering_parse(data);

    int i,j;



    if(soldering_state_bit == SOLDERING_up)
    {
        if(data->power_factor > 0.8)
        {
            for(i = 0 ; i < ELECAPP_NUM ; i++)
            {
                //欧几里得度量
                d[SOLDERING_up][i] = sqrt( (data->current - elecapp[soldering_state_bit][i][CURRENT])*(data->current - elecapp[soldering_state_bit][i][CURRENT]) + (data->power_factor - elecapp[soldering_state_bit][i][POWERFACTOR])*(data->power_factor - elecapp[soldering_state_bit][i][POWERFACTOR]) + (data->power - elecapp[soldering_state_bit][i][APOWER])*(data->power - elecapp[soldering_state_bit][i][APOWER]) );
                //标准化欧几里得度量
                // d[SOLDERING_up][i] = sqrt( ((data->current - elecapp[soldering_state_bit][i][CURRENT])/elecapp_SD[soldering_state_bit][CURRENT]) * ((data->current - elecapp[soldering_state_bit][i][CURRENT])/elecapp_SD[soldering_state_bit][CURRENT]) + 
                //                             ((data->power_factor - elecapp[soldering_state_bit][i][POWERFACTOR])/elecapp_SD[soldering_state_bit][POWERFACTOR]) * ((data->power_factor - elecapp[soldering_state_bit][i][POWERFACTOR])/elecapp_SD[soldering_state_bit][POWERFACTOR]) + 
                //                             ((data->power - elecapp[soldering_state_bit][i][APOWER])/elecapp_SD[soldering_state_bit][APOWER]) * ((data->power - elecapp[soldering_state_bit][i][APOWER])/elecapp_SD[soldering_state_bit][APOWER]) );
                //printf("d[up][%d]= %.4f\n" , i , d[SOLDERING_up][i]);
            }   
            min_i = 0;
            min_d = d[SOLDERING_up][min_i];
            for(i = 0 ; i < ELECAPP_NUM ; i++)
            {
                if(d[SOLDERING_up][i] <= min_d)
                {
                    min_i = i;
                    min_d = d[SOLDERING_up][min_i];
                }
            }
            if(min_d > PRECISION)
                min_i = ERRORELEC;
            //printf("min_d[up][%d]= %.4f\n" , min_i , min_d);  
        }   
    }
    else if(soldering_state_bit == SOLDERING_steady)
    {
        if(data->power_factor < 0.6 && data->power < 25)
        {
            dd[down_num][CURRENT] = data->current;
            dd[down_num][APOWER] = data->power;
            dd[down_num][POWERFACTOR] = data->power_factor;
            //printf("dd[%d]\ndd=%d\n" , down_num , down_down);
            down_num++;
            
            if(down_num >= 4 && down_down == 0)
            {
                down_num = 0;
                down_down = 1;
                min_j = 0;
                min_p = dd[0][APOWER];
                for(j = 0 ; j < 4 ; j++)
                {
                    if(dd[j][APOWER] <= min_p)
                    {
                        min_j = j;
                        min_p = dd[j][APOWER];
                    }    
                }
                //printf("min_p[%d]= %.4f\n" , min_j , min_p);
                //printf("minI:%.4fP:%.4fPF:%.4f\n",dd[min_j][CURRENT],dd[min_j][APOWER],dd[min_j][POWERFACTOR]);
            }
            
            if(down_down == 1)
            {
                for(i = 0 ; i < ELECAPP_NUM ; i++)
                {
                    //欧几里得度量
                    d[SOLDERING_down][i] = sqrt( (dd[min_j][CURRENT] - elecapp[SOLDERING_down][i][CURRENT])*(dd[min_j][CURRENT] - elecapp[SOLDERING_down][i][CURRENT]) + (dd[min_j][POWERFACTOR] - elecapp[SOLDERING_down][i][POWERFACTOR])*(dd[min_j][POWERFACTOR] - elecapp[SOLDERING_down][i][POWERFACTOR]) + (dd[min_j][APOWER] - elecapp[SOLDERING_down][i][APOWER])*(dd[min_j][APOWER] - elecapp[SOLDERING_down][i][APOWER]) );
                    //标准化欧几里得度量
                    // d[SOLDERING_down][i] = sqrt( ((dd[min_j][CURRENT] - elecapp[SOLDERING_down][i][CURRENT])/elecapp_SD[SOLDERING_down][CURRENT]) * ((dd[min_j][CURRENT] - elecapp[SOLDERING_down][i][CURRENT])/elecapp_SD[SOLDERING_down][CURRENT]) + 
                    //                                 ((dd[min_j][POWERFACTOR] - elecapp[SOLDERING_down][i][POWERFACTOR])/elecapp_SD[SOLDERING_down][POWERFACTOR]) * ((dd[min_j][POWERFACTOR] - elecapp[SOLDERING_down][i][POWERFACTOR])/elecapp_SD[SOLDERING_down][POWERFACTOR]) + 
                    //                                 ((dd[min_j][APOWER] - elecapp[SOLDERING_down][i][APOWER])/elecapp_SD[SOLDERING_down][APOWER]) * ((dd[min_j][APOWER] - elecapp[SOLDERING_down][i][APOWER])/elecapp_SD[SOLDERING_down][APOWER]) );
                    //printf("d[std][%d]= %.4f\n" , i , d[SOLDERING_down][i]);
                }   
                min_i = 0;
                min_d = d[SOLDERING_down][min_i];
                for(i = 0 ; i < ELECAPP_NUM ; i++)
                {
                    if(d[SOLDERING_down][i] <= min_d)
                    {
                        min_i = i;
                        min_d = d[SOLDERING_down][min_i];
                    }
                }
                if(min_d > PRECISION)
                    min_i = ERRORELEC;
                //printf("min_d[std][%d]= %.4f\n" , min_i , min_d); 
                down_down = 0;
            } 
        }
    }
    else if(soldering_state_bit == SOLDERING_down)
    {
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            //欧几里得度量
            d[SOLDERING_no][i] = sqrt( (data->current - elecapp[SOLDERING_no][i][CURRENT])*(data->current - elecapp[SOLDERING_no][i][CURRENT]) + (data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR])*(data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR]) + (data->power - elecapp[SOLDERING_no][i][APOWER])*(data->power - elecapp[SOLDERING_no][i][APOWER]) );
            d[SOLDERING_down][i] = sqrt( (data->current - elecapp[SOLDERING_down][i][CURRENT])*(data->current - elecapp[SOLDERING_down][i][CURRENT]) + (data->power_factor - elecapp[SOLDERING_down][i][POWERFACTOR])*(data->power_factor - elecapp[SOLDERING_down][i][POWERFACTOR]) + (data->power - elecapp[SOLDERING_down][i][APOWER])*(data->power - elecapp[SOLDERING_down][i][APOWER]) );

            //标准化欧几里得度量
            // d[SOLDERING_no][i] = sqrt( ((data->current - elecapp[SOLDERING_no][i][CURRENT])/elecapp_SD[SOLDERING_no][CURRENT]) * ((data->current - elecapp[SOLDERING_no][i][CURRENT])/elecapp_SD[SOLDERING_no][CURRENT]) + 
            //                             ((data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR])/elecapp_SD[SOLDERING_no][POWERFACTOR]) * ((data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR])/elecapp_SD[SOLDERING_no][POWERFACTOR]) + 
            //                             ((data->power - elecapp[SOLDERING_no][i][APOWER])/elecapp_SD[SOLDERING_no][APOWER]) * ((data->power - elecapp[SOLDERING_no][i][APOWER])/elecapp_SD[SOLDERING_no][APOWER]) );
            // d[SOLDERING_down][i] = sqrt( ((data->current - elecapp[SOLDERING_down][i][CURRENT])/elecapp_SD[SOLDERING_down][CURRENT]) * ((data->current - elecapp[SOLDERING_down][i][CURRENT])/elecapp_SD[SOLDERING_down][CURRENT]) + 
            //                                 ((data->power_factor - elecapp[SOLDERING_down][i][POWERFACTOR])/elecapp_SD[SOLDERING_down][POWERFACTOR]) * ((data->power_factor - elecapp[SOLDERING_down][i][POWERFACTOR])/elecapp_SD[SOLDERING_down][POWERFACTOR]) + 
            //                                 ((data->power - elecapp[SOLDERING_down][i][APOWER])/elecapp_SD[SOLDERING_down][APOWER]) * ((data->power - elecapp[SOLDERING_down][i][APOWER])/elecapp_SD[SOLDERING_down][APOWER]) );
            //printf("d[no][%d]= %.4f\n" , i , d[SOLDERING_no][i]);
            //printf("d[down][%d]= %.4f\n" , i , d[SOLDERING_down][i]);
        }  
        min_s = 0;
        min_i = 0;
        min_d = d[SOLDERING_no][min_i];
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            if(d[SOLDERING_no][i] <= min_d)
            {
                min_i = i;
                min_s = SOLDERING_no;
                min_d = d[SOLDERING_no][min_i];
            }
            if(d[SOLDERING_down][i] <= min_d)
            {
                min_i = i;
                min_s = SOLDERING_down;
                min_d = d[SOLDERING_down][min_i];
            }
        }
        if(min_s == SOLDERING_no) 
        {
            soldering_state_bit = SOLDERING_no;
            soldering_ground = 200;
        } 
        else if(min_s == SOLDERING_down)     
        {
            soldering_state_bit = SOLDERING_down;
        }
        if(min_d > PRECISION)
            min_i = ERRORELEC;
        //printf("min_d[%d][%d]= %.4f\n" , min_s , min_i , min_d); 
    }
    else if(soldering_state_bit == SOLDERING_no)
    {
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            //欧几里得度量
            d[SOLDERING_no][i] = sqrt( (data->current - elecapp[SOLDERING_no][i][CURRENT])*(data->current - elecapp[SOLDERING_no][i][CURRENT]) + (data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR])*(data->power_factor - elecapp[SOLDERING_no][i][POWERFACTOR]) + (data->power - elecapp[SOLDERING_no][i][APOWER])*(data->power - elecapp[SOLDERING_no][i][APOWER]) );
        }
        min_i = 0;
        min_d = d[SOLDERING_no][min_i];
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            if(d[SOLDERING_no][i] <= min_d)
            {
                min_i = i;
                min_d = d[SOLDERING_no][min_i];
            }
        }
        if(min_d > PRECISION)
            min_i = ERRORELEC;
    }
    switch(min_i)
    {
        case CHARGER:
        {
            //printf("now charger is on\n");
            sprintf(elecapp_state , "充电器充电中\n台灯未使用");
            charger_state_bit = ELECAPP_on;
            lamp_state_bit = ELECAPP_off;
            unknown_state_bit = ELECAPP_off;
            Charger_img_set(charger_state_bit);
            Lamp_img_set(lamp_state_bit);
            Unknown_img_set(unknown_state_bit);
            break;
        }
        case LAMP:
        {
            //printf("now soldering is on\n");
            sprintf(elecapp_state , "充电器未使用\n台灯使用中");
            charger_state_bit = ELECAPP_off;
            lamp_state_bit = ELECAPP_on;
            unknown_state_bit = ELECAPP_off;
            Charger_img_set(charger_state_bit);
            Lamp_img_set(lamp_state_bit);
            Unknown_img_set(unknown_state_bit);
            break;
        }
        case LAMP_CHARGER:
        {
            //printf("now soldering is on\n");
            sprintf(elecapp_state , "充电器充电中\n台灯使用中");
            charger_state_bit = ELECAPP_on;
            lamp_state_bit = ELECAPP_on;
            unknown_state_bit = ELECAPP_off;
            Charger_img_set(charger_state_bit);
            Lamp_img_set(lamp_state_bit);
            Unknown_img_set(unknown_state_bit);          
            break;
        }
        case NOAPP:
        {
            //printf("now powersupply is on\n");
            sprintf(elecapp_state , "充电器未使用\n台灯未使用");
            charger_state_bit = ELECAPP_off;
            lamp_state_bit = ELECAPP_off;
            unknown_state_bit = ELECAPP_off;
            Charger_img_set(charger_state_bit);
            Lamp_img_set(lamp_state_bit);
            Unknown_img_set(unknown_state_bit);
            break;
        }
        default:
        {
            //printf("unknown\n");
            sprintf(elecapp_state , "未知电器");
            charger_state_bit = ELECAPP_off;
            lamp_state_bit = ELECAPP_off;
            unknown_state_bit = ELECAPP_on;
            Charger_img_set(charger_state_bit);
            Lamp_img_set(lamp_state_bit);
            Unknown_img_set(unknown_state_bit);
            break;            
        }
    }
}

void euclidean_data_init()
{
    int i , k;
    float AVG[SOLDERING_STATE_NUM][CHARA_NUM] = {0};

    sprintf(soldering_state , "电烙铁关闭");
    sprintf(elecapp_state , "充电器未使用\n台灯未使用");
    
    /************************************************/
    elecapp[SOLDERING_no][CHARGER][CURRENT] = 0.122;
    elecapp[SOLDERING_no][CHARGER][APOWER] = 13.3;
    elecapp[SOLDERING_no][CHARGER][POWERFACTOR] = 0.466;

    elecapp[SOLDERING_no][LAMP][CURRENT] = 0.087;
    elecapp[SOLDERING_no][LAMP][APOWER] = 9.1;
    elecapp[SOLDERING_no][LAMP][POWERFACTOR] = 0.445;

    elecapp[SOLDERING_no][LAMP_CHARGER][CURRENT] = 0.177;
    elecapp[SOLDERING_no][LAMP_CHARGER][APOWER] = 18.2;
    elecapp[SOLDERING_no][LAMP_CHARGER][POWERFACTOR] = 0.452;

    elecapp[SOLDERING_no][NOAPP][CURRENT] = 0.037;
    elecapp[SOLDERING_no][NOAPP][APOWER] = 2.3;
    elecapp[SOLDERING_no][NOAPP][POWERFACTOR] = 0.280;

    /************************************************/
    elecapp[SOLDERING_up][CHARGER][CURRENT] = 0.2993;
    elecapp[SOLDERING_up][CHARGER][APOWER] = 63.0;
    elecapp[SOLDERING_up][CHARGER][POWERFACTOR] = 0.928;

    elecapp[SOLDERING_up][LAMP][CURRENT] = 0.2663;
    elecapp[SOLDERING_up][LAMP][APOWER] = 58.3;
    elecapp[SOLDERING_up][LAMP][POWERFACTOR] = 0.947;

    elecapp[SOLDERING_up][LAMP_CHARGER][CURRENT] = 0.3355;
    elecapp[SOLDERING_up][LAMP_CHARGER][APOWER] = 67.8;
    elecapp[SOLDERING_up][LAMP_CHARGER][POWERFACTOR] = 0.874;

    elecapp[SOLDERING_up][NOAPP][CURRENT] = 0.2318;
    elecapp[SOLDERING_up][NOAPP][APOWER] = 52.2;
    elecapp[SOLDERING_up][NOAPP][POWERFACTOR] = 0.975;

    /************************************************/
    elecapp[SOLDERING_down][CHARGER][CURRENT] = 0.1415;
    elecapp[SOLDERING_down][CHARGER][APOWER] = 17.2;
    elecapp[SOLDERING_down][CHARGER][POWERFACTOR] = 0.587;

    elecapp[SOLDERING_down][LAMP][CURRENT] = 0.1067;
    elecapp[SOLDERING_down][LAMP][APOWER] = 13.2;
    elecapp[SOLDERING_down][LAMP][POWERFACTOR] = 0.534;

    elecapp[SOLDERING_down][LAMP_CHARGER][CURRENT] = 0.185;
    elecapp[SOLDERING_down][LAMP_CHARGER][APOWER] = 22.4;
    elecapp[SOLDERING_down][LAMP_CHARGER][POWERFACTOR] = 0.523;

    elecapp[SOLDERING_down][NOAPP][CURRENT] = 0.0648;
    elecapp[SOLDERING_down][NOAPP][APOWER] = 8.0;
    elecapp[SOLDERING_down][NOAPP][POWERFACTOR] = 0.510;


    /************************************************/
    /*通过以下计算得出以上的数据在对应维度上的均方差*/
    /************************************************/

    for(k = 0 ; k < SOLDERING_STATE_NUM ; k++)
    {
        for(i = 0 ; i < ELECAPP_NUM ; i++)
            AVG[k][CURRENT] = AVG[k][CURRENT] + elecapp[k][i][CURRENT];
        AVG[k][CURRENT] = AVG[k][CURRENT] / ELECAPP_NUM ;
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            elecapp_SD[k][CURRENT] += (elecapp[k][i][CURRENT] - AVG[k][CURRENT]) * (elecapp[k][i][CURRENT] - AVG[k][CURRENT]);
        }
        elecapp_SD[k][CURRENT] = sqrt(elecapp_SD[k][CURRENT] / ELECAPP_NUM);

        for(i = 0 ; i < ELECAPP_NUM ; i++)
            AVG[k][APOWER] = AVG[k][APOWER] + elecapp[k][i][APOWER];
        AVG[k][APOWER] = AVG[k][APOWER] / ELECAPP_NUM ;
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            elecapp_SD[k][APOWER] += (elecapp[k][i][APOWER] - AVG[k][APOWER]) * (elecapp[k][i][APOWER] - AVG[k][APOWER]);
        }
        elecapp_SD[k][APOWER] = sqrt(elecapp_SD[k][APOWER] / ELECAPP_NUM);

        for(i = 0 ; i < ELECAPP_NUM ; i++)
            AVG[k][POWERFACTOR] = AVG[k][POWERFACTOR] + elecapp[k][i][POWERFACTOR];
        AVG[k][POWERFACTOR] = AVG[k][POWERFACTOR] / ELECAPP_NUM ;
        for(i = 0 ; i < ELECAPP_NUM ; i++)
        {
            elecapp_SD[k][POWERFACTOR] += (elecapp[k][i][POWERFACTOR] - AVG[k][POWERFACTOR]) * (elecapp[k][i][POWERFACTOR] - AVG[k][POWERFACTOR]);
        }
        elecapp_SD[k][POWERFACTOR] = sqrt(elecapp_SD[k][POWERFACTOR] / ELECAPP_NUM);                
    }
}

void Charger_img_set(uint8_t state)
{
    if(state == 1)
        lv_img_set_src(lvgl_gui->user_tabview_tab3_img_charger , &charger_on);
    else
        lv_img_set_src(lvgl_gui->user_tabview_tab3_img_charger , &charger_off);
}

void Lamp_img_set(uint8_t state)
{
    if(state == 1)
        lv_img_set_src(lvgl_gui->user_tabview_tab3_img_lamp , &lamp_on);
    else
        lv_img_set_src(lvgl_gui->user_tabview_tab3_img_lamp , &lamp_off);
}

void Solder_img_set(uint8_t state)
{
    switch(state)
    {
        case SOLDERING_no : lv_img_set_src(lvgl_gui->user_tabview_tab3_img_solder , &solder_off); break;
        case SOLDERING_up : lv_img_set_src(lvgl_gui->user_tabview_tab3_img_solder , &solder_up); break;
        case SOLDERING_down : lv_img_set_src(lvgl_gui->user_tabview_tab3_img_solder , &solder_down); break;
        case SOLDERING_steady : lv_img_set_src(lvgl_gui->user_tabview_tab3_img_solder , &solder_steady); break;
        default:break;
    }
}
void Unknown_img_set(uint8_t state)
{
    static uint8_t unknown_bit;
    if(state == 1)
    {
        lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_lamp , 0 , 0);
        lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_charger , 0 , 0);

        if(unknown_bit == 0)
        {
            lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_unknown , 48 , 48);
            unknown_bit = 1;
        }
    }
    else
    {
        if(unknown_bit == 1)
        {
            lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_unknown , 0 , 0);
            unknown_bit = 0;
        }

        lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_lamp , 48 , 48);
        lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_charger , 48 , 48);
    }

}