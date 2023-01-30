#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

//电机初始化（主要是对PWM初始化）
void DC_Motor_Init(void);
//调节电机（PWM）占空比
void DC_Motor_SetSpeed(unsigned char speed);

#endif