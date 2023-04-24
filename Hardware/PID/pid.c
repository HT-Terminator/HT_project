#include "pid.h"
#include "ht32.h"
#include "encoder.h"
#include "pwm.h"

volatile position_pid Speedl;         //速度
volatile position_pid Speedr;         //速度

position_pid Dir;     			//转向环

float left_motor_out,right_motor_out;

//位置式PID
float Position_pid(position_pid *pid)
{
    pid->error=pid->target_value - pid->actual_value;//误差
    pid->Integral+=pid->error;//积分项
    pid->Difference=pid->error-pid->last_error;//微分项
    if(pid->Integral>pid->Integral_Max)//积分限幅
    {
        pid->Integral=pid->Integral_Max;
    }
    if(pid->Integral<pid->Integral_Min)
    {
        pid->Integral=pid->Integral_Min;
    }
    pid->output= pid->kp*pid->error + pid->ki*pid->Integral + pid->kd*pid->Difference;//位置式pid计算
//    if(pid->output > pid->output_Max)//pid输出限幅
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
    //速度
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
	
		//转向环
    Dir.kp=5;
    Dir.ki=0;
    Dir.kd=6;
	  Dir.target_value=0;
    Dir.output=0;
    Dir.output_last=0;
    Dir.output_Max=15;
    Dir.output_Min=-15;

}

//速度环
//target_speed:目标速度
//pid:速度pid相关参数
void Speed_pid_ctrl(float target_speed,position_pid *pid)
{
    Position_pid(pid);
}

//转向环外环
//target_yaw:目标航向角
//pid:pid相关参数
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

/*控制周期中使用如下语句*/
/*
{
			Speedl.output_last = Speedl.output;
			Speedr.output_last = Speedr.output;
			Speedl.actual_value = Speed_data.actual_value_l;
			Speedr.actual_value = Speed_data.actual_value_r;
			
			Direction_pid_ctrl(Dir.target_value,&Dir);//转向环
			
			Speed_pid_ctrl(Speedl.target_value, &Speedl);
			Speed_pid_ctrl(Speedr.target_value, &Speedr);
			

			PWML += Speedl.output;
			PWMR += Speedr.output;
			
			MotorOutput(PWML,PWMR);


}
*/







