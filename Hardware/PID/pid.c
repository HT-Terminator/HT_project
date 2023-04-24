#include "pid.h"
#include "ht32.h"
#include "encoder.h"
#include "pwm.h"

volatile position_pid Speedl;         //�ٶ�
volatile position_pid Speedr;         //�ٶ�

position_pid Dir;     			//ת��

float left_motor_out,right_motor_out;

//λ��ʽPID
float Position_pid(position_pid *pid)
{
    pid->error=pid->target_value - pid->actual_value;//���
    pid->Integral+=pid->error;//������
    pid->Difference=pid->error-pid->last_error;//΢����
    if(pid->Integral>pid->Integral_Max)//�����޷�
    {
        pid->Integral=pid->Integral_Max;
    }
    if(pid->Integral<pid->Integral_Min)
    {
        pid->Integral=pid->Integral_Min;
    }
    pid->output= pid->kp*pid->error + pid->ki*pid->Integral + pid->kd*pid->Difference;//λ��ʽpid����
//    if(pid->output > pid->output_Max)//pid����޷�
//    {
//        pid->output=pid->output_Max;
//    }
//    if(pid->output < pid->output_Min)
//    {
//        pid->output=pid->output_Min;
//    }
    pid->last_error=pid->error;
		return pid->output;
}

void Pidinit(void)
{
    //�ٶ�
    Speedl.kp=5.5;//0.25 0.3 0.375 0.8
    Speedl.ki=1;
    Speedl.kd=1;
    Speedl.target_value=50+left_right_error;//57 58
    Speedl.output=0;
    Speedl.output_last=0;
    Speedl.output_Max=4500;//300
    Speedl.output_Min=-4500;
	
	  Speedr.kp=4;
    Speedr.ki=1;
    Speedr.kd=1;
    Speedr.target_value=50;
    Speedr.output=0;
    Speedr.output_last=0;
    Speedr.output_Max=4500;
    Speedr.output_Min=-4500;

}

//�ٶȻ�
//target_speed:Ŀ���ٶ�
//pid:�ٶ�pid��ز���
void Speed_pid_ctrl(float target_speed,position_pid *pid)
{
    Position_pid(pid);
}

//ת���⻷
//target_yaw:Ŀ�꺽���
//pid:pid��ز���
void Direction_pid_ctrl(float target_yaw,position_pid *pid)
{
    Position_pid(pid);
}

void MotorOutput(float left_pid_control_output, float right_pid_control_output)
{
	
    left_motor_out = left_pid_control_output;
    right_motor_out = right_pid_control_output;
	
//		if(left_motor_out > 4500)
//			left_motor_out = 4500;
//		if(left_motor_out < 100)
//			left_motor_out = 100;
//		
//		if(right_motor_out > 4500)
//			right_motor_out = 4500;
//		if(right_motor_out < 100)
//			right_motor_out = 100;
	
		SetMotor_L(left_motor_out);
		SetMotor_R(right_motor_out);

}

/*����������ʹ���������*/
/*
{
			Speedl.output_last = Speedl.output;
			Speedr.output_last = Speedr.output;
			Speedl.actual_value = Speed_data.actual_value_l;
			Speedr.actual_value = Speed_data.actual_value_r;
			
			Direction_pid_ctrl(Dir.target_value,&Dir);//ת��
			
			Speed_pid_ctrl(Speedl.target_value, &Speedl);
			Speed_pid_ctrl(Speedr.target_value, &Speedr);
			

			PWML += Speedl.output;
			PWMR += Speedr.output;
			
			MotorOutput(PWML,PWMR);


}
*/







