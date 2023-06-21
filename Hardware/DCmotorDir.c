#include "stm32f4xx.h"                  // Device header

#define MOTORDIR_CLOCK_GPIOx	RCC_AHB1Periph_GPIOB
#define MOTORDIR_GPIOx			GPIOA
#define AIN1					GPIO_Pin_14
#define AIN2					GPIO_Pin_15
#define BIN1					GPIO_Pin_13
#define BIN2					GPIO_Pin_12
#define MOTORDIR_GPIO_PINS		(AIN1 | AIN2 | BIN1 | BIN2)

#define ADIR_P					Bit_SET
#define ADIR_N					Bit_RESET
#define BDIR_P					Bit_RESET
#define BDIR_N					Bit_SET

#define DIR_P					1
#define DIR_N					0

void DCmotorDir_Init(uint8_t Pos)
{
	RCC_AHB1PeriphClockCmd(MOTORDIR_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef MOTORDIR_GPIO_InitStructure;
	MOTORDIR_GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;				//GPIO input mode
	MOTORDIR_GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;				//GPIO push-pull output
	MOTORDIR_GPIO_InitStructure.GPIO_Pin=MOTORDIR_GPIO_PINS;
	MOTORDIR_GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;					//GPIO pull mode: pull up 
	MOTORDIR_GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;			//GPIO speed:100MHz
	GPIO_Init(MOTORDIR_GPIOx, &MOTORDIR_GPIO_InitStructure);			//GPIO init 
	
	if(Pos==DIR_P)
	{
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN1, ADIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN2, ADIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN1, BDIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN2, BDIR_N);
	}
	else if(Pos==DIR_N)
	{
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN1, ADIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN2, ADIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN1, BDIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN2, BDIR_P);
	}
}
void DCmotorDir_Set(uint8_t Pos)
{
	if(Pos==DIR_P)
	{
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN1, ADIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN2, ADIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN1, BDIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN2, BDIR_N);
	}
	else if(Pos==DIR_N)
	{
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN1, ADIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, AIN2, ADIR_P);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN1, BDIR_N);
		GPIO_WriteBit(MOTORDIR_GPIOx, BIN2, BDIR_P);
	}
}


