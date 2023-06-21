#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"

#define KEY_SHAKE_DELETE_TIME	40

#define KEY1_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define KEY1_GPIOx		GPIOB
#define KEY1_GPIO1		GPIO_Pin_13
#define KEY1_GPIO2		GPIO_Pin_15
#define KEY1_GPIO_PINS	(KEY1_GPIO1 | KEY1_GPIO2)

#define KEY2_CLOCK_GPIOx		RCC_AHB1Periph_GPIOB
#define KEY2_GPIOx		GPIOB
#define KEY2_GPIO1		GPIO_Pin_1
#define KEY2_GPIO2		GPIO_Pin_10
#define KEY2_GPIO_PINS	(KEY2_GPIO1 | KEY2_GPIO2)

void Key_Init(void)
{
	RCC_AHB1PeriphClockCmd(KEY1_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef GPIO1_InitStructure;
	GPIO1_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
//	GPIO1_InitStructure.GPIO_OType=GPIO_OType_OD;			//GPIO push-pull output
	GPIO1_InitStructure.GPIO_Pin=KEY1_GPIO_PINS;				//GPIO pins to input by KEYS
	GPIO1_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode:no pull down
	GPIO1_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(KEY1_GPIOx, &GPIO1_InitStructure);					//GPIO init 
	
	RCC_AHB1PeriphClockCmd(KEY2_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef GPIO2_InitStructure;
	GPIO2_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
// 	GPIO2_InitStructure.GPIO_OType=GPIO_OType_OD;			//GPIO push-pull output
	GPIO2_InitStructure.GPIO_Pin=KEY2_GPIO_PINS;				//GPIO pins to input by KEYS
	GPIO2_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;			//GPIO pull mode:no pull down
	GPIO2_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(KEY1_GPIOx, &GPIO2_InitStructure);					//GPIO init 
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(KEY1_GPIOx, KEY1_GPIO1)==0)
	{
		Delay_ms(KEY_SHAKE_DELETE_TIME);
		while(GPIO_ReadInputDataBit(KEY1_GPIOx, KEY1_GPIO1)==0);
		Delay_ms(KEY_SHAKE_DELETE_TIME);
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(KEY2_GPIOx, KEY2_GPIO2)==1)
	{
		Delay_ms(KEY_SHAKE_DELETE_TIME);
		while(GPIO_ReadInputDataBit(KEY2_GPIOx, KEY2_GPIO1)==1);
		Delay_ms(KEY_SHAKE_DELETE_TIME);
		KeyNum=2;
	}
	return KeyNum;
}
