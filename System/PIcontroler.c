#include "stm32f4xx.h"                  // Device header
#include "DCmotorPWM.h"
#include "OLED.h"

extern double RTargetVel, LTargetVel;
extern double RVelocity, LVelocity;
extern double RPWM, LPWM;
extern double Kp, Ki;
//PI
void IncrementalPI(void)
{
	static double RVelBias, LVelBias, LastRVelBias, LastLVelBias;
	RVelBias=RVelocity-RTargetVel;				//compute current bias
	LVelBias=LVelocity-LTargetVel;				//compute current bias
	RPWM-=Kp*(RVelBias-LastRVelBias)/100+Ki*RVelBias/1000;		//incremental PI controller
	LPWM-=Kp*(LVelBias-LastLVelBias)/100+Ki*LVelBias/1000;		//incremental PI controller
	if(RPWM<0)	{RPWM=0.0;}
	if(LPWM<0)	{LPWM=0.0;}
	if(RPWM-100.0>0)	{RPWM=100.0;}
	if(LPWM-100.0>0)	{LPWM=100.0;}
	LastRVelBias=RVelBias;					//save last velocity bias
	LastLVelBias=LVelBias;					//save last velocity bias
//	OLED_ShowString(4,1,"RT=");
//	OLED_ShowSignedNum(4,4,(int32_t)RTargetVel,3);
//	OLED_ShowString(4,8,"RPWM=");
//	OLED_ShowSignedNum(4,13,(int32_t)RPWM,3);
//		OLED_ShowString(1,1,"LRV=");
//		OLED_ShowSignedNum(1,6,(int32_t)LastRVelBias,5);
//		OLED_ShowString(2,1,"RV=");
//		OLED_ShowSignedNum(2,6,(int32_t)RVelocity,5);
//		OLED_ShowString(3,1,"RVB=");
//		OLED_ShowSignedNum(3,5,(int32_t)RVelBias,3);
	DCmotorPWM_Set();	//change PWM
	return;
}
