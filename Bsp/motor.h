#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

#define AIN1(x) do{x? HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET): HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);} while(0)
#define AIN2(x) do{x? HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET): HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);} while(0)
#define BIN1(x) do{x? HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET): HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);} while(0)
#define BIN2(x) do{x? HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET): HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);} while(0)

#define ReductionRatio  28
#define LineNumber      500
#define CountTime       10
#define DeadValue_R     300
#define DeadValue_L     300
#define Amplitude       3500

#define PWMA            TIM1 -> CCR1
#define PWMB            TIM1 -> CCR2

#define MotorStart  HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET)
#define MotorStop   HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_RESET)

extern int Target_PositionR, Target_PositionL;
extern int Reality_PositionR, Reality_PositionL;
extern int Target_VelocityR, Target_VelocityL;
extern int Reality_VelocityR, Reality_VelocityL;


int Myabs (int value);
void Motor_Stop(void);
void SetPwm_R(int motor);
void SetPwm_L(int motor);
int Read_Encoder(int TIMX);

int Xianfu(int data, int max);
long Num_Encoder_Cnt(float num);
long Rpm_Encoder_Cnt(float rpm);

int Position_PID_R(int reality,int target);
int Position_PID_L(int reality,int target);
int Incremental_PID_R(int reality, int target);
int Incremental_PID_L(int reality, int target);

#endif
