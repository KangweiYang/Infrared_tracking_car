#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"

//10 9 8 7 6 5 4 3 2 1 0 L1 L2
#define PINS_NUM			5
#define M0PIN				2
#define M0PIN0				2.0
#define R10_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R10_GPIO			GPIOB
#define R10					GPIO_Pin_0
#define R9_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R9_GPIO				GPIOB
#define R9					GPIO_Pin_1
#define R8_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R8_GPIO				GPIOB
#define R8					GPIO_Pin_2
#define R7_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R7_GPIO				GPIOB
#define R7					GPIO_Pin_3
#define R6_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R6_GPIO				GPIOB
#define R6					GPIO_Pin_4
#define R5_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R5_GPIO				GPIOB
#define R5					GPIO_Pin_5
#define R4_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R4_GPIO				GPIOB
#define R4					GPIO_Pin_6
#define R3_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R3_GPIO				GPIOB
#define R3					GPIO_Pin_7
#define R2_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R2_GPIO				GPIOB
#define R2					GPIO_Pin_3
#define R1_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define R1_GPIO				GPIOB
#define R1					GPIO_Pin_6
#define M0_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define M0_GPIO				GPIOB
#define M0					GPIO_Pin_7
#define L1_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define L1_GPIO				GPIOB
#define L1					GPIO_Pin_8
#define L2_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define L2_GPIO				GPIOB
#define L2					GPIO_Pin_9


#define ALL_CLOCK_GPIOx0		RCC_AHB1Periph_GPIOB
#define ALL_GPIOx0			GPIOB
#define ALL_PDGPIOs0			(R10 | R9 | R8 | R7 | R6 | R5 | R4 | R3 | R2 | R1 | M0 | L1 | L2)
#define ALL_CLOCK_GPIOx1		RCC_AHB1Periph_GPIOB
#define ALL_GPIOx1			GPIOB
#define ALL_PDGPIOs1			(R10 | R9 | R8 | R7 | R6 | R5 | R4 | R3 | R2 | R1 | M0 | L1 | L2)


#define IT_CLOCK_GPIOx						RCC_AHB1Periph_GPIOA
#define IT_GPIOx							GPIOA						//need to change in main.c
#define IFNOR_PIN0							GPIO_Pin_1					//need to change in main.c
#define IFNOR_PIN1							GPIO_Pin_15					//need to change in main.c
#define IFNOR_ALL_GPIO_PINS					(IFNOR_PIN0 | IFNOR_PIN1)
#define IFNOR_EXTILINECONFIG_GPIOx0			EXTI_PortSourceGPIOA
#define IFNOR_EXTILINECONFIG_GPIOx1			EXTI_PortSourceGPIOA
#define IFNOR_EXTILINECONFIG_GPIO_PIN0		EXTI_PinSource1
#define IFNOR_EXTILINECONFIG_GPIO_PIN1		EXTI_PinSource15
#define IFNOR_EXTI_GPIO0					EXTI_Line1
#define IFNOR_EXTI_GPIO1					EXTI_Line15
#define IFNOR_EXTI_GPIO_PINS				(IFNOR_EXTI_GPIO0 | IFNOR_EXTI_GPIO1)
#define IFNOR_NVIC_IRQCHANNAL0				EXTI15_10_IRQn
#define IFNOR_NVIC_IRQCHANNAL1				EXTI15_10_IRQn

#define OUT	0
#define IN	1

extern double AdvanceAngle;
extern double RTargetVel, LTargetVel;
extern int8_t OutFlag;
extern int32_t OutContThres;

extern uint32_t OutCont;


void PDSearch_Init(void)
{
	RCC_AHB1PeriphClockCmd(ALL_CLOCK_GPIOx0, ENABLE);
	GPIO_InitTypeDef GPIO0_InitStructure;
	GPIO0_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
//	GPIO0_InitStructure.GPIO_OType=GPIO_OType_OD;			//GPIO push-pull output
	GPIO0_InitStructure.GPIO_Pin=ALL_PDGPIOs0;				//GPIO pins to input by KEYS
	GPIO0_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode:no pull down
	GPIO0_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(ALL_GPIOx0, &GPIO0_InitStructure);					//GPIO init 
	
	RCC_AHB1PeriphClockCmd(ALL_CLOCK_GPIOx1, ENABLE);
	GPIO_InitTypeDef GPIO1_InitStructure;
	GPIO1_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
//	GPIO1_InitStructure.GPIO_OType=GPIO_OType_OD;			//GPIO push-pull output
	GPIO1_InitStructure.GPIO_Pin=ALL_PDGPIOs1;				//GPIO pins to input by KEYS
	GPIO1_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode:no pull down
	GPIO1_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(ALL_GPIOx1, &GPIO1_InitStructure);					//GPIO init 
	
	
	RCC_AHB1PeriphClockCmd(IT_CLOCK_GPIOx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;						//GPIO input mode
	//GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;					//GPIO push-pull output
	GPIO_InitStructure.GPIO_Pin=IFNOR_ALL_GPIO_PINS;				//GPIO pins to count
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;						//GPIO pull mode:pull up 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;				//GPIO speed:100MHz
	GPIO_Init(IT_GPIOx, &GPIO_InitStructure);				//GPIO init 
	
	SYSCFG_EXTILineConfig(IFNOR_EXTILINECONFIG_GPIOx0,IFNOR_EXTILINECONFIG_GPIO_PIN0);		//AFIO select GPIO pin
	SYSCFG_EXTILineConfig(IFNOR_EXTILINECONFIG_GPIOx1,IFNOR_EXTILINECONFIG_GPIO_PIN1);		//AFIO select GPIO pin
	
	EXTI_InitTypeDef EXTI_InitStructure;							//EXTI0 init
	EXTI_InitStructure.EXTI_Line=IFNOR_EXTI_GPIO_PINS;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//only can choose one config in a project
	
	NVIC_InitTypeDef IFNOR_NVIC_InitStructure0;
	IFNOR_NVIC_InitStructure0.NVIC_IRQChannel=IFNOR_NVIC_IRQCHANNAL0;
	IFNOR_NVIC_InitStructure0.NVIC_IRQChannelCmd=ENABLE;
	IFNOR_NVIC_InitStructure0.NVIC_IRQChannelPreemptionPriority=0;
	IFNOR_NVIC_InitStructure0.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&IFNOR_NVIC_InitStructure0);
	
	NVIC_InitTypeDef IFNOR_NVIC_InitStructure1;
	IFNOR_NVIC_InitStructure1.NVIC_IRQChannel=IFNOR_NVIC_IRQCHANNAL1;
	IFNOR_NVIC_InitStructure1.NVIC_IRQChannelCmd=ENABLE;
	IFNOR_NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority=0;
	IFNOR_NVIC_InitStructure1.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&IFNOR_NVIC_InitStructure1);
}

int8_t AbsValue(int8_t unsolve)
{
	if(unsolve<0)	return -unsolve;
	else	return unsolve;
}

int8_t Get_AdvanceAngle(int16_t AngleUnit, double IncrementWhileSearchNoLines, WhichCircle Circle)
{
	double PD[PINS_NUM];
	AdvanceAngle=0;
	int8_t LowPDnum=0, ContinuousLowPD=0;
	int8_t LowPDpos[PINS_NUM], NearM0PDpos=PINS_NUM-1, FarM0PDpos=M0PIN;
	int j=0;
	
	PD[4]=!(double)GPIO_ReadInputDataBit(R2_GPIO,R2);
	PD[3]=!(double)GPIO_ReadInputDataBit(R1_GPIO,R1);
	PD[2]=!(double)GPIO_ReadInputDataBit(M0_GPIO,M0);
	PD[1]=!(double)GPIO_ReadInputDataBit(L1_GPIO,L1);
	PD[0]=!(double)GPIO_ReadInputDataBit(L2_GPIO,L2);
	
	for(int i=0;i<PINS_NUM;i++)
	{
		if(PD[i]==RESET)
		{
			LowPDnum++;
			ContinuousLowPD++;
			LowPDpos[j]=i;												//LowPDpos->低电平的位置
			j++;
			if(AbsValue(i-M0PIN)<AbsValue(NearM0PDpos-i))	NearM0PDpos=i;	//filtration to get the nearest position
			else	FarM0PDpos=i;
			if(ContinuousLowPD>0)	ContinuousLowPD--;					//如果低电平不连续->ContinuousLowPD!=LowPDnum
		}
	}
	if(LowPDnum==PINS_NUM)
	{
		//Motor stop!!!!!!!!!!!!!!!!!
		RTargetVel=0;
		LTargetVel=0;
		AdvanceAngle=0;
		for(int k=0;k<PINS_NUM;k++)
			LowPDpos[k]=0;	
		return LowPDnum;
	}
	//Motor start!!!!!!!!!!!!!!!
	else if(LowPDnum==0)
	{
		if(AdvanceAngle>0)	AdvanceAngle+=IncrementWhileSearchNoLines;
		if(AdvanceAngle<0)	AdvanceAngle-=IncrementWhileSearchNoLines;
		return LowPDnum;
	}
	if(LowPDnum==1)
	{
		AdvanceAngle=(double)(LowPDpos[0]-M0PIN)*AngleUnit;
		return LowPDnum;
	}
	else if(LowPDnum>1 && LowPDnum==ContinuousLowPD)
	{
		double LowPDnumAvg=0;
		for(int i=0;i<LowPDnum;i++)
		{
			LowPDnumAvg+=(double)LowPDpos[i];
		}
		LowPDnumAvg/=LowPDnum;
		AdvanceAngle=(double)(LowPDnumAvg-M0PIN0)*AngleUnit;
		return LowPDnum;
	}
	else if(LowPDnum>1 && LowPDnum!=ContinuousLowPD)
	{
		if(Circle==FIRST_CIRCLE)
		{
			AdvanceAngle=(double)(NearM0PDpos-M0PIN)*AngleUnit;
		}
		else if(Circle==SECOND_CIRCLE)
		{
			AdvanceAngle=(double)(FarM0PDpos-M0PIN)*AngleUnit;
		}
	}
	return LowPDnum;
}

void EXTI15_10_IRQHandler(void)
{
	Get_AdvanceAngle(1, 0.01, FIRST_CIRCLE);
	EXTI_ClearITPendingBit(IFNOR_EXTI_GPIO1);
//	EXTI_ClearITPendingBit(IFNOR_EXTI_GPIO0);
}
