#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"

/*
第一步，使能RCC时钟
第二步，设置内部时钟模式
第三步，设置时基单元的分频、自动重装器：计数器溢出频率：CK_CNT_OV=CK_CNT/(ARR+1)
																=CK_PSC/(PSC+1)/(ARR+1)
第四步，配置中断输出控制
第五步，配置NVIC
第六步，配置运行控制
第七步，使能计数器
*/

#define SAMPLING_TIME_US			1000
#define SAMPLING_TIME_US0			1000.0
#define MOTOR_PULSES_PER_CIRCLE		15000.0

#define VELOCITY_RCC_CLOCK			RCC_APB1Periph_TIM2
#define VELOCITY_TIMx				TIM2
#define VELOCITY_NVIC_IRQCHANNAL	TIM2_IRQn
#define VELOCITY_IT_FUNCTION		TIM2_IRQHandler

extern double RVelocity,LVelocity;
extern int32_t RMotorCount, LMotorCount;

void GPTimer_Init(void)
{
	//step 1
	RCC_APB1PeriphClockCmd(VELOCITY_RCC_CLOCK, ENABLE);
	
	//step 2
	TIM_InternalClockConfig(VELOCITY_TIMx);
	
	//step 3
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=SAMPLING_TIME_US-1;		//在0-65535范围内
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;					//在0-65535范围内
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;				//通用定时器没有该功能，置0
	TIM_TimeBaseInit(VELOCITY_TIMx, &TIM_TimeBaseInitStructure);
	
	//step 4
	TIM_ITConfig(VELOCITY_TIMx, TIM_IT_Update, ENABLE);				//使能更新中断
	
	//step 5
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//注意避免不同配置，这个只能配置一种
	
	NVIC_InitTypeDef RIGHT_NVIC_InitStructure;						//NVIC结构体配置
	RIGHT_NVIC_InitStructure.NVIC_IRQChannel=VELOCITY_NVIC_IRQCHANNAL;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&RIGHT_NVIC_InitStructure);
	
	//step 6
	TIM_Cmd(VELOCITY_TIMx, ENABLE);
	
}

void TIM2_IRQHandler(void)
{
	static int32_t LastRMotorCount, LastLMotorCount;
	RVelocity=(RMotorCount-LastRMotorCount)*1000000.0*60.0/MOTOR_PULSES_PER_CIRCLE/SAMPLING_TIME_US0/5.0;		//unit: rpm
	LVelocity=(LMotorCount-LastLMotorCount)*1000000.0*60.0/MOTOR_PULSES_PER_CIRCLE/SAMPLING_TIME_US0/5.0;		//unit: rpm
//	LED1_ON();
//	Delay_ms(100);
//	LED1_OFF();
//	Delay_ms(100);
	LastRMotorCount=RMotorCount;
	LastLMotorCount=LMotorCount;
	TIM_ClearITPendingBit(VELOCITY_TIMx, TIM_IT_Update);
}
