/*
Freq = CK_PSC / (PSC + 1) / (ARR + 1)
Duty = CCR / (ARR + 1)
Reso = 1 / (ARR + 1)

Steps:
1. Open RCC clock: TIM and GPIO
2. Config clock source and timer unit
3. Config output compare unit: CCR, OC mode, polarity and enable output
4. Config GPIO: init PWM_GPIO as the AF_PP mode
5. Start CNT
*/
#include "stm32f4xx.h"                  // Device header

#define ARR_NUM						1000
#define CCR_NUM						0
#define PSC_NUM						7
#define DCMOTOR_PWM_RCC_CLOCK		RCC_APB2Periph_TIM1
#define DCMOTOR_TIMx				TIM1
#define DCMOTOR_CLOCK_GPIOx			RCC_AHB1Periph_GPIOA
#define DCMOTOR_GPIOx				GPIOA
#define DCMOTOR_GPIO1				GPIO_Pin_8
#define DCMOTOR_GPIO2				GPIO_Pin_11
#define DCMOTOR_GPIO_PINS			(DCMOTOR_GPIO1 | DCMOTOR_GPIO2)
#define DCMOTOR_GPIO_PINSOURCE1		GPIO_PinSource8
#define DCMOTOR_GPIO_PINSOURCE2		GPIO_PinSource11
#define DCMOTOR_AF_TIMx				GPIO_AF_TIM1

extern double RPWM, LPWM;

void DCmotorPWM_Init()
{
	//step 1
	RCC_APB2PeriphClockCmd(DCMOTOR_PWM_RCC_CLOCK, ENABLE);
	RCC_AHB1PeriphClockCmd(DCMOTOR_CLOCK_GPIOx, ENABLE);
	
	//step 2
	TIM_InternalClockConfig(DCMOTOR_TIMx);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=ARR_NUM-1;		//?0-65535???
	TIM_TimeBaseInitStructure.TIM_Prescaler=PSC_NUM-1;					//?0-65535???
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;				//??????????,?0
	TIM_TimeBaseInit(DCMOTOR_TIMx, &TIM_TimeBaseInitStructure);
	
	//step 3
	TIM_OCInitTypeDef TIM1_OCInitStructure;
	TIM_OCStructInit(&TIM1_OCInitStructure);					//init it to avoid uncleared members
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM1_OCInitStructure.TIM_Pulse = CCR_NUM;
//	TIM1_OCInitStructure.TIM_OCIdleState
//	TIM1_OCInitStructure.TIM_OCNIdleState
//	TIM1_OCInitStructure.TIM_OCNPolarity
//	TIM1_OCInitStructure.TIM_OutputNState
//	TIM1_OCInitStructure.TIM_OutputState
	TIM_OC1Init(DCMOTOR_TIMx, &TIM1_OCInitStructure);
	
	TIM_OCInitTypeDef TIM4_OCInitStructure;
	TIM_OCStructInit(&TIM4_OCInitStructure);					//init it to avoid uncleared members
	TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM4_OCInitStructure.TIM_Pulse = CCR_NUM;
	TIM_OC4Init(DCMOTOR_TIMx, &TIM4_OCInitStructure);

	//step 4
	GPIO_InitTypeDef GPIO1_InitStructure;
	GPIO1_InitStructure.GPIO_Mode=GPIO_Mode_AF;				//GPIO AF mode
	GPIO1_InitStructure.GPIO_OType=GPIO_OType_PP;			//GPIO push-pull output
	GPIO1_InitStructure.GPIO_Pin=DCMOTOR_GPIO_PINS;				//GPIO pins to input by KEYS
	GPIO1_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode:no pull down
	GPIO1_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;			//GPIO speed:50MHz
	GPIO_Init(DCMOTOR_GPIOx, &GPIO1_InitStructure);					//GPIO init 
	
	GPIO_PinAFConfig(DCMOTOR_GPIOx, DCMOTOR_GPIO_PINSOURCE1, DCMOTOR_AF_TIMx);
	GPIO_PinAFConfig(DCMOTOR_GPIOx, DCMOTOR_GPIO_PINSOURCE2, DCMOTOR_AF_TIMx);
	//step 5
	TIM_CtrlPWMOutputs(DCMOTOR_TIMx, ENABLE);
	TIM_Cmd(DCMOTOR_TIMx, ENABLE);
	
	
}

void DCmotorPWM_Set()
{
	TIM_SetCompare1(DCMOTOR_TIMx, RPWM*10);
	TIM_SetCompare4(DCMOTOR_TIMx, LPWM*10);
}
