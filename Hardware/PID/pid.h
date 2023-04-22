#ifndef		__PID
#define		__PID

#include "ht32.h"

typedef struct
{
    float kp,ki,kd;
    float error;                        //误差
    float last_error;                   //上一次误差

    float error_low,last_error_low;
    float target_value;                 //目标值
    float actual_value;                 //实际值
    float Integral;                     //积分项I
    float Difference;                   //微分项
    float Integral_Max,Integral_Min;    //积分项最大值,最小值
    float output;                       //pid算式计算后的输出值
    float output_Max,output_Min;        //输出限幅
    float output_last;                  //pid上次输出

}position_pid;






#endif

