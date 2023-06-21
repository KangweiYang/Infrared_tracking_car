#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"

#define MOTORCOUNT_CLOCK_GPIOx				RCC_AHB1Periph_GPIOB
#define MOTORCOUNT_GPIOx					GPIOB
#define RIGHT_NVIC_IRQCHANNAL				EXTI0_IRQn
#define LEFT_NVIC_IRQCHANNAL				EXTI1_IRQn
#define RIGHT_EXTILINECONFIG_GPIOx			EXTI_PortSourceGPIOB
#define LEFT_EXTILINECONFIG_GPIOx			EXTI_PortSourceGPIOB
#define RIGHT_EXTILINECONFIG_GPIO_PIN		EXTI_PinSource0
#define LEFT_EXTILINECONFIG_GPIO_PIN		EXTI_PinSource1
#define RIGHT_MOTORCOUNT_GPIO				GPIO_Pin_0
#define RIGHT_EXTI_GPIO						EXTI_Line0
#define LEFT_MOTORCOUNT_GPIO				GPIO_Pin_1
#define LEFT_EXTI_GPIO						EXTI_Line1
#define MOTORCOUNT_GPIO_PINS				(RIGHT_MOTORCOUNT_GPIO | LEFT_MOTORCOUNT_GPIO)
#define EXTI_GPIO_PINS						(RIGHT_EXTI_GPIO | LEFT_EXTI_GPIO)

#define B_MOTORCOUNT_CLOCK_GPIOx			RCC_AHB1Periph_GPIOB
#define B_MOTORCOUNT_GPIOx					GPIOB
#define B_RIGHT_MOTORCOUNT_GPIO				GPIO_Pin_12
#define B_LEFT_MOTORCOUNT_GPIO				GPIO_Pin_13
#define B_MOTORCOUNT_GPIO_PINS				(B_RIGHT_MOTORCOUNT_GPIO | B_LEFT_MOTORCOUNT_GPIO)


extern int64_t RMotorCount, LMotorCount;

void MotorCount_Init(void)
{
	RCC_AHB1PeriphClockCmd(MOTORCOUNT_CLOCK_GPIOx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;						//GPIO input mode
	//GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;					//GPIO push-pull output
	GPIO_InitStructure.GPIO_Pin=MOTORCOUNT_GPIO_PINS;				//GPIO pins to count
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;						//GPIO pull mode:pull up 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;				//GPIO speed:100MHz
	GPIO_Init(MOTORCOUNT_GPIOx, &GPIO_InitStructure);				//GPIO init 
	
	SYSCFG_EXTILineConfig(RIGHT_EXTILINECONFIG_GPIOx,RIGHT_EXTILINECONFIG_GPIO_PIN);		//AFIO select GPIO pin
	SYSCFG_EXTILineConfig(LEFT_EXTILINECONFIG_GPIOx,LEFT_EXTILINECONFIG_GPIO_PIN);		//AFIO select GPIO pin
	
	EXTI_InitTypeDef EXTI_InitStructure;							//EXTI0 init
	EXTI_InitStructure.EXTI_Line=EXTI_GPIO_PINS;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//only can choose one config in a project
	
	NVIC_InitTypeDef RIGHT_NVIC_InitStructure;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannel=RIGHT_NVIC_IRQCHANNAL;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	RIGHT_NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&RIGHT_NVIC_InitStructure);
	
	NVIC_InitTypeDef LEFT_NVIC_InitStructure;
	LEFT_NVIC_InitStructure.NVIC_IRQChannel=LEFT_NVIC_IRQCHANNAL;
	LEFT_NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	LEFT_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	LEFT_NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&LEFT_NVIC_InitStructure);
	
	//GPIO B phase init
	RCC_AHB1PeriphClockCmd(B_MOTORCOUNT_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef B_MOTORCOUNT_GPIO_InitStructure;
	B_MOTORCOUNT_GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
//	B_MOTORCOUNT_GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;			//GPIO push-pull output
	B_MOTORCOUNT_GPIO_InitStructure.GPIO_Pin=B_MOTORCOUNT_GPIO_PINS;
	B_MOTORCOUNT_GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode: pull up 
	B_MOTORCOUNT_GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;			//GPIO speed:100MHz
	GPIO_Init(B_MOTORCOUNT_GPIOx, &B_MOTORCOUNT_GPIO_InitStructure);					//GPIO init 
	
	RMotorCount=0;
	LMotorCount=0;
	
}

void EXTI0_IRQHandler(void)
{
	RMotorCount++;
	EXTI_ClearITPendingBit(RIGHT_EXTI_GPIO);
}

void EXTI1_IRQHandler(void)
{
	LMotorCount++;
	EXTI_ClearITPendingBit(LEFT_EXTI_GPIO);
}

