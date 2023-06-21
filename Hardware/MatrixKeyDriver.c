#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"
#include "OLED.h"

#define MatrixKey_SHAKE_DELETE_TIME	40

#define MATRIXKEYOUT_CLOCK_GPIOx		RCC_AHB1Periph_GPIOA
#define MATRIXKEYOUT_GPIOx		GPIOA
#define MATRIXKEYOUT_GPIO0		GPIO_Pin_0
#define MATRIXKEYOUT_GPIO1		GPIO_Pin_1
#define MATRIXKEYOUT_GPIO2		GPIO_Pin_2
#define MATRIXKEYOUT_GPIO3		GPIO_Pin_3
#define MATRIXKEYOUT_GPIO_PINS	(MATRIXKEYOUT_GPIO0 | MATRIXKEYOUT_GPIO1 | MATRIXKEYOUT_GPIO2 | MATRIXKEYOUT_GPIO3)

#define MATRIXKEYIN_CLOCK_GPIOx		RCC_AHB1Periph_GPIOA
#define MATRIXKEYIN_GPIOx		GPIOA
#define MATRIXKEYIN_GPIO0		GPIO_Pin_4
#define MATRIXKEYIN_GPIO1		GPIO_Pin_5
#define MATRIXKEYIN_GPIO2		GPIO_Pin_6
#define MATRIXKEYIN_GPIO3		GPIO_Pin_7
#define MATRIXKEYIN_GPIO_PINS	(MATRIXKEYIN_GPIO0 | MATRIXKEYIN_GPIO1 | MATRIXKEYIN_GPIO2 | MATRIXKEYIN_GPIO3)

void MatrixKey_Init(void)
{
//	GPIO0s set output, PP, no pull.	0001
	RCC_AHB1PeriphClockCmd(MATRIXKEYOUT_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef GPIOOUT_InitStructure;
	GPIOOUT_InitStructure.GPIO_Mode=GPIO_Mode_OUT;				//GPIO input mode
	GPIOOUT_InitStructure.GPIO_OType=GPIO_OType_PP;			//GPIO push-pull output
	GPIOOUT_InitStructure.GPIO_Pin=MATRIXKEYOUT_GPIO_PINS;				//GPIO pins to input by MatrixKeyS
	GPIOOUT_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;			//GPIO pull mode:no pull down
	GPIOOUT_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(MATRIXKEYOUT_GPIOx, &GPIOOUT_InitStructure);					//GPIO init 
	
//	GPIO1s set input, __, pull down.	xxxx
	RCC_AHB1PeriphClockCmd(MATRIXKEYIN_CLOCK_GPIOx, ENABLE);
	GPIO_InitTypeDef GPIOIN_InitStructure;
	GPIOIN_InitStructure.GPIO_Mode=GPIO_Mode_IN;				//GPIO input mode
// 	GPIOIN_InitStructure.GPIO_OType=GPIO_OType_OD;			//GPIO push-pull output
	GPIOIN_InitStructure.GPIO_Pin=MATRIXKEYIN_GPIO_PINS;				//GPIO pins to input by MatrixKeyS
	GPIOIN_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;			//GPIO pull mode:no pull down
	GPIOIN_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;			//GPIO speed:50MHz
	GPIO_Init(MATRIXKEYIN_GPIOx, &GPIOIN_InitStructure);					//GPIO init 
}

uint8_t MatrixKey_InputUnit(uint16_t GPIO_Pin)
{
	uint8_t Column=0;
	if(GPIO_ReadInputDataBit(MATRIXKEYIN_GPIOx, GPIO_Pin)==1)
	{
		Delay_ms(MatrixKey_SHAKE_DELETE_TIME);
		while(GPIO_ReadInputDataBit(MATRIXKEYIN_GPIOx, GPIO_Pin)==1);
		Delay_ms(MatrixKey_SHAKE_DELETE_TIME);
		GPIO_Pin=GPIO_Pin>>4;
		while(GPIO_Pin)
		{
			GPIO_Pin=GPIO_Pin>>1;
			Column++;
		}
	}
	return Column;
}

uint8_t MatrixKey_OutputUnit(uint16_t OutputCont)
{
	uint8_t Row=0;
	GPIO_WriteBit(MATRIXKEYOUT_GPIOx, MATRIXKEYOUT_GPIO0, Bit_RESET);
	GPIO_WriteBit(MATRIXKEYOUT_GPIOx, MATRIXKEYOUT_GPIO1, Bit_RESET);
	GPIO_WriteBit(MATRIXKEYOUT_GPIOx, MATRIXKEYOUT_GPIO2, Bit_RESET);
	GPIO_WriteBit(MATRIXKEYOUT_GPIOx, MATRIXKEYOUT_GPIO3, Bit_RESET);
	GPIO_WriteBit(MATRIXKEYOUT_GPIOx, OutputCont, Bit_SET);
	OutputCont=OutputCont<<1;
	while(OutputCont>>1!=0)
	{
		OutputCont=OutputCont>>1;
		Row++;
	}
//	OLED_ShowNum(2,1,MatrixKeyUnitNum,1);
	
	return Row;
}

uint8_t MatrixKey_GetNum(void)
{
	uint8_t MatrixKeyNum = 0, Row=0, Column=0;
	
	for(int j=0;j<4;j++)
	{
		Row=MatrixKey_OutputUnit(MATRIXKEYOUT_GPIO0<<j);
		for(int i=0;i<4;i++)
		{
			Delay_ms(5);
			Column=MatrixKey_InputUnit(MATRIXKEYIN_GPIO0<<i);
//			OLED_ShowNum(4-j,4-i,!!Column,1);
			if(Column!=0)	MatrixKeyNum=(Row-1)*4+Column;
//			OLED_ShowNum(1,6,Row*4-4+Column,2);
//			if(Temp1!=0)
//				OLED_ShowNum(3,4,Temp1,2);
//			OLED_ShowNum(3,7,MatrixKeyNum,2);
		}
	}
	
	return MatrixKeyNum;
}
