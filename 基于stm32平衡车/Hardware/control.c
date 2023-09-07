
#include "control.h"



float Med_Angle =-0.6;//机械中值，期望角度，让小车直立的期望角度

float Target_Speed = 0; //期望速度(前后，俯仰)。用于控制小车前进后退等操作的速度

float Turn_Speed=0; //期望速度(左右，偏航)，用于控制小车左转右转等操作的速度

float Vertical_Kp = 240; //直立环kp
float Vertical_Kd = 1.0; //直立环kd

float Velocity_Kp = -0.345; //速度环kp
float Velocity_Ki =-0.001725; //速度环ki

int Vertical_out; //直立环输出
int Velocity_out; //速度环输出
int Turn_out;     //转向环输出

float a = 0.7; //低通滤波的系数

float b = 0.65;//转向环的系数

float Turn_Kp=15;//转向环的kp
float Turn_Kd=0.65;//转向环的Kd(就是以前转向环的系数:b)

#define SPEED_Y 70 //俯仰（前后）最大速度
#define SPEED_Z 90 //偏航（左右）最大速度



int Vertical_out,Velocity_out,Turn_out;

int Vertical(float Med, float Angle, float Gyro_y); //直立环函数，期望角度，真实角度，y轴角速度
int Velocity(int Target,int encoder_left,int encoder_right); //速度环函数，期望速度，左电机速度，右电机速度
int Turn (int gyro_z, int RC);

void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5) != 0) //捕获一下pb5的中断标志位Line5,看是否被拉高，如果拉高则说明触发了
	{
		if(PBin(5) == 0)//触发外部中断后会变成低电平
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
			
			//首先采集编码器数据，和MPU6050的角度信息
			Encoder_Left = -Read_Speed(2);//电机是相对安装的，相差180度，为了让编码器的输出极性一样，需要对其中一个取反
			Encoder_Right = Read_Speed(3);
			
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//陀螺仪
			
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);//加速度
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);//角度，俯仰角，横滚角，偏航角
			
			
			//有关遥控的部分
			//前后
			if(Fore == 1)
			{
				Target_Speed--;//前进1标志位拉高，向前走
			}
			
			if(Back == 1)
			{
				Target_Speed++;//后退
			}
			
			if((Fore == 0) && (Back == 0))
			{
				Target_Speed = 0;//如果不前进也不后退，则静止状态，在原地立着
			}
		
			if(Target_Speed > SPEED_Y)//限幅函数
			{
				Target_Speed = SPEED_Y;
			}
			else if(Target_Speed < -SPEED_Y)
			{
				Target_Speed = -SPEED_Y;
			}
			else
			{
				Target_Speed = Target_Speed;
			}
			
			//左右
			if(Left == 1)
			{
				Turn_Speed -= 40; //左转，变化快慢
			}
			
			if(Right == 1)
			{
				Turn_Speed += 40; //右转，变化快慢
			}
			
			if((Left == 0) && (Right == 0))
			{
				Turn_Speed = 0;//如果不左转也不右转，则静止状态，在原地立着
			}
			
			if(Turn_Speed > SPEED_Z)//限幅函数
			{
				Turn_Speed = SPEED_Z;
			}
			else if(Turn_Speed < -SPEED_Z)
			{
				Turn_Speed = -SPEED_Z;
			}
			else
			{
				Turn_Speed = Turn_Speed;
			}
			
			//对转向约束的判断
			if((Left == 0) && (Right == 0))
			{
				Turn_Kd = b;//如果没有左转右转的遥控指令，则开启转向约束，b为转向约束的值
			}
			else if((Left == 1) || (Right == 1))
			{
				Turn_Kd =0;//如果接收到左转或者右转的指令，则关闭转向约束，设置kd的值为0，表示无转向约束
			}
				
			
			//将数据放入闭环控制之中，并且计算出控制输出量
			Velocity_out = Velocity(Target_Speed,Encoder_Left,Encoder_Right);//速度环，期望速度，左电机速度，右电机速度
			Vertical_out = Vertical(Velocity_out + Med_Angle,Pitch,gyroy);//直立环，这里是速度环的输出加上机械中值，防止一开始小车一直跑，俯仰角，陀螺仪的y轴
			Turn_out = Turn(gyroz,Turn_Speed);//转向环，z轴角速度,偏航角速度（左右转动的速度）
			
			//最终输出
			PWM_out = Vertical_out;//直立环的输出就是最终的pwm值，因为速度环的输入到直立环中了，串级调节
			
			//把控制输出量加载到电机上，完成最终的控制
			MOTO1 = PWM_out-Turn_out;//左电机，俩个电机朝向相反
			MOTO2=PWM_out+Turn_out; //右电机
			
			Limit(&MOTO1,&MOTO2);//pwm限幅
			Load(MOTO1,MOTO2);//加载到电机上
			
			Stop(&Med_Angle,&Pitch);//安全检查，如果倾斜角度过大，停止电机
			
			
			
		}
	}
}


//直立环PD公式：Kp*Ek+Kd*Ek_D

int Vertical(float Med, float Angle, float gyro_y) //直立环函数，期望角度(速度环输出的结果)，真实角度，y轴角速度
{
	int PWM_out;
	
	PWM_out = Vertical_Kp * (Angle - Med) + Vertical_Kd * (gyro_y - 0);//期望角速度肯定为0，所以-0，真实角度为角度偏差
	
	return PWM_out;
}


//速度环PI公式：Kp*Ek+Ki*Ek_S

int Velocity(int Target,int encoder_left,int encoder_right) //速度环函数，期望速度，左电机速度，右电机速度
{

	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;//速度偏差积分 //pwm输出//电机速度偏差//电机速度偏差低通滤波后的值
	
	
	Encoder_Err = ((encoder_left+encoder_right)-Target);//舍去误差，左电机速度+右电机速度再减去期望速度
	
	//对速度偏差添加低通滤波，公式：Lowout=(1-a)*Ek+a*low_out_last;//(1-系数)*本次偏差+系数*上次低通滤波器输出
	
	EnC_Err_Lowout = (1-a) * Encoder_Err + a * EnC_Err_Lowout_last;//使波形更加平滑，滤除高频干扰，防止速度的突变
	
	EnC_Err_Lowout_last = EnC_Err_Lowout; //防止速度过大的影响直立环工作
	
	
	Encoder_S += EnC_Err_Lowout;//得到速度偏差积分,得到位移
	
	//积分限幅函数
	if(Encoder_S > 10000)
	{
		Encoder_S = 10000;
	}
	else if(Encoder_S < (-10000))
	{
		Encoder_S = -10000;
	}
	else
	{
		Encoder_S = Encoder_S;
	}
//	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
	if(stop == 1)
	{
		Encoder_S=0,stop=0;//如果倾斜角度过大而使电机停止运行，则清零积分量
	}
	
	PWM_out = Velocity_Kp * EnC_Err_Lowout + Velocity_Ki * Encoder_S;//速度环kp*电机速度偏差+速度环ki*速度偏差积分
	
	return PWM_out;
	
}


//转向环 ：系数*z轴角速度+系数+遥控数据

int Turn(int gyro_z, int RC)
{
	int PWM_out;
	
	PWM_out = Turn_Kp * RC + Turn_Kd * gyro_z;//不是严格的pd控制器，因为kd是以前的转向约束，kp是遥控有关的转向

	return PWM_out;
	
}

