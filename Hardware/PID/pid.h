#ifndef		__PID
#define		__PID

#include "ht32.h"

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






#endif

