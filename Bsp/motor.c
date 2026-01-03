#include "motor.h"

float PositionR_KP = 0.09,PositionR_KI = 0.0004,PositionR_KD = 0.295;                          // PID系数
float PositionL_KP = 0.09,PositionL_KI = 0.0004,PositionL_KD = 0.295;                                 // PID系数
float IncrementalR_KP = 1.2,IncrementalR_KI = 0.2,IncrementalR_KD = 0.2;                        // PID系数
float IncrementalL_KP = 1.2,IncrementalL_KI = 0.2,IncrementalL_KD = 0.2;                        // PID系数

int Reality_PositionR, Reality_PositionL;                                                       // 位置环左右电机的实际值（脉冲数）
int Reality_VelocityR, Reality_VelocityL;                                                       // 速度环左右电机的实际值（脉冲数）
int Target_PositionR,Target_PositionL;                                                          // 位置环左右电机的目标（理论）值（脉冲数）
int Target_VelocityR, Target_VelocityL;                                                         // 速度环左右电机的目标（理论）值（脉冲数）
/*绝对值函数*/
int Myabs(int value)
{
    int tempValue = 0;
    if (value < 0)  tempValue = -value;
    else            tempValue = value;
    return tempValue;
}
/*读取编码器值*/
int Read_Encoder(int TIMX)
{
    int Encoder_TIM = 0;
    switch (TIMX)
    {
        case 2:     Encoder_TIM = (short)TIM2 -> CNT; TIM2 -> CNT = 0; break;           // 必须将编码器的值强制类型转换成short类型（其寄存器的数据类型就是short类型的）
        case 3:     Encoder_TIM = (short)TIM3 -> CNT; TIM3 -> CNT = 0; break;           // 必须将编码器的值强制类型转换成short类型（其寄存器的数据类型就是short类型的）
        default :   Encoder_TIM = 0;
    }
    return Encoder_TIM;
}
/*计算电机转对应圈数所需要的脉冲数*/
long Num_Encoder_Cnt(float num)
{
    return (num * ReductionRatio * LineNumber * 4);                                     // 4倍频
}
/*计算转速对应编码器的脉冲数*/
long Rpm_Encoder_Cnt(float rpm)
{
    return (rpm * ReductionRatio *LineNumber * 4) / (60 * 1000 / CountTime);
}
/*电机限幅函数*/
int Xianfu(int data, int max)
{
    if (data < -max)     data = -max;                                                   // 反转限幅
    if (data >  max)     data = max;                                                    // 正转限幅
    return data;
}
/*右电机使能函数*/
void SetPwm_R(int motor)
{
    if (motor > 0)      {AIN1(1);AIN2(0);}                                              // 正转
    else                {AIN1(0);AIN2(1);}                                              // 反转
    if (motor)          {motor = Myabs(motor) + DeadValue_R;PWMA = Xianfu(motor, Amplitude);}      // 先加死区再限幅
    else                PWMA = 0;                                                       // 无PWM输出时，电机停转
}
/*左电机使能函数*/
void SetPwm_L(int motor)
{
    if (motor > 0)      {BIN1(0);BIN2(1);}                                              // 正转
    else                {BIN1(1);BIN2(0);}                                              // 反转
    if (motor)          {motor = Myabs(motor) + DeadValue_L;PWMB = Xianfu(motor, Amplitude);}      // 先加死区再限幅
    else                PWMB = 0;                                                       // 无PWM输出时，电机停转
}
/*电机刹车*/
void Motor_Stop(void)
{
    MotorStop;
    PWMA = 0;
    PWMB = 0;
    TIM1 -> CCR1 = 0;
    TIM1 -> CCR2 = 0;
    Reality_PositionR = 0;
    Reality_PositionL = 0;
}
/*右轮位置式PID控制器*/
int Position_PID_R(int reality,int target)
{
    static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
    Bias=target-reality;                                                                // 计算偏差
    Integral_bias+=Bias;                                                                // 偏差累积
    
    if(Integral_bias> 10000) Integral_bias = 10000;                                     // 积分限幅
    if(Integral_bias<-10000) Integral_bias =-10000;
    
    Pwm = (PositionR_KP*Bias)                                                           // 比例环节
         +(PositionR_KI*Integral_bias)                                                  // 积分环节
         +(PositionR_KD*(Bias-Last_Bias));                                              // 微分环节
    
    Last_Bias=Bias;                                                                     // 保存上次偏差
    return Pwm;                                                                         // 输出结果
}
/*左轮位置式PID控制器*/
int Position_PID_L(int reality,int target)
{
    static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
    Bias=target-reality;                                                                // 计算偏差
    Integral_bias+=Bias;                                                                // 偏差累积
    
    if(Integral_bias> 10000) Integral_bias = 10000;                                     // 积分限幅
    if(Integral_bias<-10000) Integral_bias =-10000;
    
    Pwm = (PositionL_KP*Bias)                                                           // 比例环节
         +(PositionL_KI*Integral_bias)                                                  // 积分环节
         +(PositionL_KD*(Bias-Last_Bias));                                              // 微分环节
    
    Last_Bias=Bias;                                                                     // 保存上次偏差
    return Pwm;                                                                         // 输出结果
}
/*右轮增量式PID控制器*/
int Incremental_PID_R(int reality, int target)
{
    static float Bias, Pwm, Last_bias = 0, Prev_bias = 0;
    
    Bias = target - reality;                                                            // 计算偏差
    
    Pwm += (IncrementalR_KP * (Bias - Last_bias))                                       // 比例环节
        + (IncrementalR_KI * Bias)                                                      // 积分环节
        + (IncrementalR_KD * (Bias - 2 * Last_bias + Prev_bias));                       // 微分环节
    
    Prev_bias = Last_bias;                                                              // 保存上上次偏差
    Last_bias = Bias;                                                                   // 保存上一次偏差
    
    return Pwm;                                                                         // 输出结果
}
/*左轮增量式PID控制器*/
int Incremental_PID_L(int reality, int target)
{
    static float Bias, Pwm, Last_bias = 0, Prev_bias = 0;
    
    Bias = target - reality;                                                            // 计算偏差
    
    Pwm += (IncrementalL_KP * (Bias - Last_bias))                                       // 比例环节
        + (IncrementalL_KI * Bias)                                                      // 积分环节
        + (IncrementalL_KD * (Bias - 2 * Last_bias + Prev_bias));                       // 微分环节
    
    Prev_bias = Last_bias;                                                              // 保存上上次偏差
    Last_bias = Bias;                                                                   // 保存上一次偏差
    
    return Pwm;                                                                         // 输出结果
}
