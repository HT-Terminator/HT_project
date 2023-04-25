#ifndef		__PID
#define		__PID

#include "ht32.h"

#define  left_right_error		1		//�����ֵľ�̬���

typedef struct
{
    float kp,ki,kd;
    float error;                        //���
    float last_error;                   //��һ�����

    float error_low,last_error_low;
    float target_value;                 //Ŀ��ֵ
    float actual_value;                 //ʵ��ֵ
    float Integral;                     //������I
    float Difference;                   //΢����
    float Integral_Max,Integral_Min;    //���������ֵ,��Сֵ
    float output;                       //pid��ʽ���������ֵ
    float output_Max,output_Min;        //����޷�
    float output_last;                  //pid�ϴ����

}position_pid;

extern volatile position_pid Speedl;         //�ٶ�
extern volatile position_pid Speedr;         //�ٶ�

extern position_pid Dir;     			//ת��

float Position_pid(position_pid *pid);
void Pidinit(void);
void Speed_pid_ctrl(float target_speed,position_pid *pid);
void Direction_pid_ctrl(float target_yaw,position_pid *pid);
void MotorOutput(float left_pid_control_output, float right_pid_control_output);


#endif

