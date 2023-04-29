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
    Speedl.kp=52;//6.5 8.5 30 40 50
    Speedl.ki=2;
    Speedl.kd=0;	//0
    Speedl.target_value=12;//57 58
    Speedl.output=0;
    Speedl.output_last=0;
    Speedl.output_Max=1500;//300
    Speedl.output_Min=-1500;
	
	  Speedr.kp=52;
    Speedr.ki=2;
    Speedr.kd=0;		//0
    Speedr.target_value=12;
    Speedr.output=0;
    Speedr.output_last=0;
    Speedr.output_Max=1500;
    Speedr.output_Min=-1500;
	
		//ת��
    Dir.kp=20;
    Dir.ki=0;
    Dir.kd=1;
	  Dir.target_value=0;
    Dir.output=0;
    Dir.output_last=0;
    Dir.output_Max=2000;
    Dir.output_Min=-2000;

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







