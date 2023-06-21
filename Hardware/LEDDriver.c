#include "stm32f4xx.h"                  // Device header

#define LED_CLOCK_GPIOx		RCC_AHB1Periph_GPIOC
#define LED_GPIOx		GPIOC
#define LED_GPIO1		GPIO_Pin_13
#define LED_GPIO2		GPIO_Pin_3
#define LED_GPIO_PINS	(LED_GPIO1 | LED_GPIO2)

void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(LED_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;				//GPIO output mode
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;			//GPIO push-pull output
	GPIO_InitStructure.GPIO_Pin=LED_GPIO_PINS;				//GPIO pins to control LEDS
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;			//GPIO pull mode:no pull up or pull down
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(LED_GPIOx, &GPIO_InitStructure);					//GPIO init 
	
	GPIO_SetBits(LED_GPIOx, LED_GPIO_PINS);						//LED_GPIOS set high to close LED
}

void LED1_ON(void)
{
	GPIO_ResetBits(LED_GPIOx, LED_GPIO1);
}

void LED1_OFF(void)
{
	GPIO_SetBits(LED_GPIOx, LED_GPIO1);
}

void LED2_ON(void){
	GPIO_ResetBits(LED_GPIOx, LED_GPIO2);
}

void LED2_OFF(void)
{
	GPIO_SetBits(LED_GPIOx, LED_GPIO2);
}
