#ifndef		__PID
#define		__PID

#include "ht32.h"

#define  left_right_error		1		//左右轮的静态误差

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

extern volatile position_pid Speedl;         //速度
extern volatile position_pid Speedr;         //速度

extern position_pid Dir;     			//转向环

float Position_pid(position_pid *pid);
void Pidinit(void);
void Speed_pid_ctrl(float target_speed,position_pid *pid);
void Direction_pid_ctrl(float target_yaw,position_pid *pid);
void MotorOutput(float left_pid_control_output, float right_pid_control_output);


#endif

