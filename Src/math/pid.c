/******************* (C) COPYRIGHT 2018-20~~ HappyMoon **************************
 *              github:https://github.com/MoonUniverse/HappyMoon
 *              issue:https://github.com/MoonUniverse/HappyMoon/issues           
 * @文件     pid.c
 * @说明     pid控制器
 * @作者     YuyingJin
 * @邮箱     jinyuyingwork@gmail.com
 * @日期     2018 ~
*********************************************************************************/
#include "pid.h"

/**********************************************************************************************************
*函 数 名: PID_GetP
*功能说明: 比例控制器
*形    参: PID结构体 误差输入
*返 回 值: 输出比例控制
**********************************************************************************************************/
float PID_GetP(PID_t* pid, float error)
{
    return (float)error * pid->kP;
}

/**********************************************************************************************************
*函 数 名: PID_GetI
*功能说明: 积分控制器
*形    参: PID结构体 误差输入 运行时间间隔
*返 回 值: 输出积分控制
**********************************************************************************************************/
float PID_GetI(PID_t* pid, float error, float dt)
{
    if((pid->kI != 0) && (dt != 0))
    {
        pid->integrator += ((float)error * pid->kI) * dt;
        pid->integrator = ConstrainFloat(pid->integrator, -pid->imax, +pid->imax);
        return pid->integrator;
    }
    return 0;
}

/**********************************************************************************************************
*函 数 名: PID_ResetI
*功能说明: 微分项清零
*形    参: PID结构体
*返 回 值: 无
**********************************************************************************************************/
void PID_ResetI(PID_t* pid)
{
    pid->integrator = 0;
}

/**********************************************************************************************************
*函 数 名: PID_GetD
*功能说明: 带低通滤波的微分控制器，避免微分项噪声过大
*形    参: PID结构体 误差输入 运行时间间隔
*返 回 值: 输出微分控制
**********************************************************************************************************/
float PID_GetD(PID_t* pid, float error, float dt)
{
    if ((pid->kD != 0) && (dt != 0))
    {
        float derivative;

        derivative = (error - pid->lastError) / dt;

        derivative = pid->lastDerivative + (dt / ( pid->dFilter + dt)) * (derivative - pid->lastDerivative);

        pid->lastError	= error;
        pid->lastDerivative = derivative;

        return pid->kD * derivative;
    }
    return 0;
}

/**********************************************************************************************************
*函 数 名: PID_GetPI
*功能说明: 比例+积分控制器
*形    参: PID结构体 误差输入 运行时间间隔
*返 回 值: 输出PI控制
**********************************************************************************************************/
float PID_GetPI(PID_t* pid, float error, float dt)
{
    return PID_GetP(pid, error) + PID_GetI(pid, error, dt);
}

/**********************************************************************************************************
*函 数 名: PID_GetPID
*功能说明: 比例+积分+微分控制器
*形    参: PID结构体 误差输入 运行时间间隔
*返 回 值: 输出PID控制
**********************************************************************************************************/
float PID_GetPID(PID_t* pid, float error, float dt)
{
    return PID_GetP(pid, error) + PID_GetI(pid, error, dt) + PID_GetD(pid, error, dt);
}

/**********************************************************************************************************
*函 数 名: PID_SetParam
*功能说明: PID参数设置
*形    参: PID结构体 比例参数 积分参数 微分参数 积分上限 微分项低通截止频率
*返 回 值: 无
**********************************************************************************************************/
void PID_SetParam(PID_t* pid, float p, float i, float d, float imaxval, float dCutFreq)
{
    pid->kP = p;
    pid->kI = i;
    pid->kD = d;
    pid->imax = imaxval;
    if(dCutFreq != 0)
        pid->dFilter = 1 / (2 * PI * dCutFreq);
    else
        pid->dFilter = 0;
}

