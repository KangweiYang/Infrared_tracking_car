#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "LEDDriver.h"
#include "KeyDriver.h"
#include "MatrixKeyDriver.h"
#include "OLED.h"
#include "MotorCount.h"
#include "GPTimer.h"
#include "PDSearch.h"
#include "DCmotorPWM.h"
#include "DCmotorDir.h"
#include "PIcontroler.h"

#define DIR_P					1
#define DIR_N					0
#define ON	1
#define OFF	0
#define OUT	0
#define IN	1
#define IT_GPIOx							GPIOA						//need to change in PDSearch.c
#define IFNOR_PIN0							GPIO_Pin_1					//need to change in PDSearch.c
#define IFNOR_PIN1							GPIO_Pin_15					//need to change in PDSearch.c
#define INCREMENT_WHILE_SEARCH_NOLINES		0.01						//need to change in PDSearch.c

#define DCMOTOR_GPIOx				GPIOA
#define DCMOTOR_GPIO1				GPIO_Pin_8
#define DCMOTOR_GPIO2				GPIO_Pin_11

#define MOTORCOUNT_GPIOx					GPIOB
#define RIGHT_MOTORCOUNT_GPIO				GPIO_Pin_0
#define LEFT_MOTORCOUNT_GPIO				GPIO_Pin_1
#define B_MOTORCOUNT_GPIOx					GPIOB
#define B_RIGHT_MOTORCOUNT_GPIO				GPIO_Pin_12
#define B_LEFT_MOTORCOUNT_GPIO				GPIO_Pin_13

#define MAP_SELECT_CLOCK_GPIOx				RCC_AHB1Periph_GPIOB
#define MAP_SELECT_IN_GPIOx					GPIOB
#define MAP_SELECT_IN_GPIO_PIN				GPIO_Pin_14
#define MAP_SELECT_OUT_GPIOx				GPIOB
#define MAP_SELECT_OUT_GPIO_PIN				GPIO_Pin_15

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

//#define L30		37500*2
//#define L30P	40000*2
//#define L60		75000*2
//#define L90		112500*2
//#define L120	150000*2
//#define L150	187500*2

//#define C1		121507
//#define C2		232488
//#define C3		213673
//#define C4		250982
//#define C5		303252
//#define C6		314212
//#define C7		149843
//#define C8		263627
//#define C9		245677
//#define C10		232488
//#define C11		213673
//#define C12		323245
//#define C13		232488
//#define C14		323245
//#define C15		213673
//#define C16		323245
//#define C17		154810
//#define BUFF	3000


int8_t KeyNum=0;
int64_t RMotorCount, LMotorCount;
double RVelocity,LVelocity;
double RTargetVel, LTargetVel;
double Kp=10.0, Ki=640.0;
double RPWM=0, LPWM=0;
double KL2=1.4, KL1=1.2, KM0=1, KR1=1.1;
double KTL2=2, KTL1=1.8, KTM0=1, KTR1=1.3,VT=25.0;
double KSL2=1.28333, KSL1=1.1, KSM0=1, KSR1=1.1,VS=20.0;

double KBL2=1.3, KBL1=1.1, KBM0=1, KBR1=1.2;

double AdvanceAngle=0;
double AngleUnit=15;
double MaxV=0;
double Kturn=0.0001;
int8_t OutFlag=IN;
uint32_t OutContThres=5;
uint32_t OutCont=0;
////const uint32_t ChangePosUnit[6]={8134, 12778, 16269, 24404, 32538, 40673};
//uint32_t ChangePos[17]={C1, C1+C2, C1+C2+C3, C1+C2+C3+C4, C1+C2+C3+C4+C5, C1+C2+C3+C4+C5+C6, C1+C2+C3+C4+C5+C6+C7, C1+C2+C3+C4+C5+C6+C7+C8, C1+C2+C3+C4+C5+C6+C7+C8+C9
//		, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12+C13
//		, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12+C13+C14, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12+C13+C14+C15, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12+C13+C14+C15+C16
//		, C1+C2+C3+C4+C5+C6+C7+C8+C9+C10+C11+C12+C13+C14+C15+C16+C17};

int64_t AVGMotorCount=0;

double GetSignedNum(double SignedNum)
{
	double UnsignedNum=SignedNum;
	if(UnsignedNum<0)	UnsignedNum=-UnsignedNum;
	return SignedNum/UnsignedNum;
}

int main(void)
{
	int ChangeCont=0, LastChangeCont=0, DebugCont=0, StartFlag=0, BufFlag=0;;
	int8_t LowPDnum;
//	char *RVelocityS,*LVelocityS;
	PDSearch_Init();
	OLED_Init();
	MatrixKey_Init();
//	LED_Init();
	MotorCount_Init();
	GPTimer_Init();
	DCmotorPWM_Init();
//	DCmotorDir_Init(DIR_P);
	
//		OLED_ShowString(1,1,"RCONT");
//		OLED_ShowString(2,1,"AU");
//		OLED_ShowString(3,1,"Kturn");
//		OLED_ShowString(4,1,"RT");
//		OLED_ShowString(4,8,"LT");
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO1_InitStructure;
	GPIO1_InitStructure.GPIO_Mode=GPIO_Mode_OUT;				//GPIO AF mode
	GPIO1_InitStructure.GPIO_OType=GPIO_OType_PP;			//GPIO push-pull output
	GPIO1_InitStructure.GPIO_Pin=GPIO_Pin_10;				//GPIO pins to input by KEYS
	GPIO1_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;			//GPIO pull mode:no pull down
	GPIO1_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;			//GPIO speed:50MHz
	GPIO_Init(GPIOB, &GPIO1_InitStructure);					//GPIO init 
	
	
	while(1)
	{
		KeyNum=MatrixKey_GetNum();
		if(KeyNum==1)
		{
			RTargetVel=0;
			LTargetVel=0;
			KeyNum=MatrixKey_GetNum();
			OLED_Clear();
			OLED_ShowString(1,1,"Debug Para");

			while(KeyNum!=1)
			{
				KeyNum=MatrixKey_GetNum();
				if(KeyNum==1)
				{
					OLED_Clear();
					break;
//				}
//				else if(KeyNum==2)
//				{
//					ChangePos[0]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[0] Set");
//					OLED_ShowNum(3,1,ChangePos[0],16);
//				}
//				else if(KeyNum==3)
//				{
//					ChangePos[1]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[1] Set");
//					OLED_ShowNum(3,1,ChangePos[1],16);
//				}
//				else if(KeyNum==4)
//				{
//					ChangePos[2]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[2] Set");
//					OLED_ShowNum(3,1,ChangePos[2],16);
//				}
//				else if(KeyNum==5)
//				{
//					ChangePos[3]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[3] Set");
//					OLED_ShowNum(3,1,ChangePos[3],16);
//				}
//				else if(KeyNum==6)
//				{
//					ChangePos[4]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[4] Set");
//					OLED_ShowNum(3,1,ChangePos[4],16);
//				}
//				else if(KeyNum==7)
//				{
//					ChangePos[5]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[5] Set");
//					OLED_ShowNum(3,1,ChangePos[5],16);
//				}
//				else if(KeyNum==8)
//				{
//					ChangePos[6]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[6] Set");
//					OLED_ShowNum(3,1,ChangePos[6],16);
//				}
//				else if(KeyNum==9)
//				{
//					ChangePos[7]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[7] Set");
//					OLED_ShowNum(3,1,ChangePos[7],16);
//				}
//				else if(KeyNum==10)
//				{
//					ChangePos[8]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[8] Set");
//					OLED_ShowNum(3,1,ChangePos[8],16);
//				}
//				else if(KeyNum==11)
//				{
//					ChangePos[9]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[9] Set");
//					OLED_ShowNum(3,1,ChangePos[9],16);
//				}
//				else if(KeyNum==12)
//				{
//					ChangePos[10]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[10] Set");
//					OLED_ShowNum(3,1,ChangePos[10],16);
//				}
//				else if(KeyNum==13)
//				{
//					ChangePos[11]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[11] Set");
//					OLED_ShowNum(3,1,ChangePos[11],16);
//				}
//				else if(KeyNum==14)
//				{
//					ChangePos[12]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[12] Set");
//					OLED_ShowNum(3,1,ChangePos[12],16);
//				}
//				else if(KeyNum==15)
//				{
//					ChangePos[13]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[13] Set");
//					OLED_ShowNum(3,1,ChangePos[13],16);
//				}
//				else if(KeyNum==16)
//				{
//					ChangePos[14+DebugCont%3]=RMotorCount+LMotorCount;
//					OLED_ShowString(2,1,"Position[  ] Set");
//					OLED_ShowNum(2,10,14+DebugCont%3,2);
//					OLED_ShowNum(3,1,ChangePos[14+DebugCont%3],16);
//					DebugCont++;
//				}
//			}
			OLED_ShowString(1,1,"            ");
		}
		else if(KeyNum==2)					//restart
		{
			LastChangeCont=0;
			ChangeCont=0;
			RMotorCount=0;
			LMotorCount=0;
			RTargetVel=0;
			LTargetVel=0;
			StartFlag=0;
		}
		else if(KeyNum==3)
		{
			KTL2+=0.01;
			OLED_ShowString(2,1,"KTL2+0.01 Set!");
			OLED_ShowNum(3,1,KTL2*100,16);
		}
		else if(KeyNum==4)
		{
			KTL2-=0.01;
			OLED_ShowString(2,1,"KTL2-0.01 Set!");
			OLED_ShowNum(3,1,KTL2*100,16);
		}
		else if(KeyNum==5)
		{
			KTL1+=0.01;
			OLED_ShowString(2,1,"KTL1+0.01 Set!");
			OLED_ShowNum(3,1,KTL1*100,16);
		}
		else if(KeyNum==6)
		{
			KTL1-=0.01;
			OLED_ShowString(2,1,"KTL1-0.01 Set!");
			OLED_ShowNum(3,1,KTL1*100,16);
		}
		else if(KeyNum==7)
		{
			KTR1+=0.01;
			OLED_ShowString(2,1,"KTR1+0.01 Set!");
			OLED_ShowNum(3,1,KTR1*100,16);
		}
		else if(KeyNum==8)
		{
			KTR1-=0.01;
			OLED_ShowString(2,1,"KTR1-0.01 Set!");
			OLED_ShowNum(3,1,KTR1*100,16);
		}
		else if(KeyNum==9)
		{
			KSL2+=0.01;
			OLED_ShowString(2,1,"KSL2+0.01 Set!");
			OLED_ShowNum(3,1,KSL2*100,16);
		}
		else if(KeyNum==10)
		{
			KSL2-=0.01;
			OLED_ShowString(2,1,"KSL2-0.01 Set!");
			OLED_ShowNum(3,1,KSL2*100,16);
		}
		else if(KeyNum==11)
		{
			KSL1+=0.01;
			OLED_ShowString(2,1,"KSL1+0.01 Set!");
			OLED_ShowNum(3,1,KSL1*100,16);
		}
		else if(KeyNum==12)
		{
			KSL1-=0.01;
			OLED_ShowString(2,1,"KSL1-0.01 Set!");
			OLED_ShowNum(3,1,KSL1*100,16);
		}
		else if(KeyNum==13)
		{
			KSR1+=0.01;
			OLED_ShowString(2,1,"KSR1+0.01 Set!");
			OLED_ShowNum(3,1,KSR1*100,16);
		}
		else if(KeyNum==14)
		{
			KSR1-=0.01;
			OLED_ShowString(2,1,"KSR1-0.01 Set!");
			OLED_ShowNum(3,1,KSR1*100,16);
		}
		else if(KeyNum==15)
		{
			VS+=1.0;
			OLED_ShowString(2,1,"VS+1.0 Set!");
			OLED_ShowNum(3,1,VS,16);
		}
		else if(KeyNum==16)
		{
			OLED_Clear();
			if(StartFlag==0)
			{
				StartFlag=1;
				OLED_ShowString(3,1,"Start!");
				if(StartFlag==1)
				{
					if(ChangeCont%2==0)
					{
						MaxV=VS;
						KL2=KSL2;
						KL1=KSL1;
						KM0=1;
						KR1=KSR1;
					}
					else
					{
						MaxV=VT;
						KL2=KTL2;
						KL1=KTL1;
						KM0=1;
						KR1=KTR1;
					}
				}
			}
			else
			{
				StartFlag=0;
				OLED_ShowString(3,1,"Stop!");
				MaxV=0;
				RTargetVel=0;
				LTargetVel=0;
			}
		}
//		OLED_ShowSignedNum(1,5,(int32_t)MaxV,3);
//		OLED_ShowSignedNum(2,8,(int32_t)OutContThres,8);
//		OLED_ShowSignedNum(2,3,(int32_t)AngleUnit,3);
//		OLED_ShowSignedNum(3,6,(int32_t)(Kturn*10000),3);
//		OLED_ShowSignedNum(3,11,(int32_t)OutCont,5);
////		OLED_ShowSignedNum(2,10,(int32_t)Ki,3);
////		OLED_ShowSignedNum(3,4,(int32_t)RVelocity,3);
////		OLED_ShowSignedNum(3,12,(int32_t)LVelocity,3);
//		OLED_ShowSignedNum(4,3,(int32_t)RTargetVel,3);
//		OLED_ShowSignedNum(4,10,(int32_t)LTargetVel,3);
		
//		OLED_ShowSignedNum(2,1,(int32_t)RMotorCount,15);
//		OLED_ShowSignedNum(3,1,(int32_t)LMotorCount,15);
		if(AVGMotorCount>1800000)	
		LowPDnum=LowPDnum=Get_AdvanceAngle(1, 0, SECOND_CIRCLE);
		else
		LowPDnum=LowPDnum=Get_AdvanceAngle(1, 0, FIRST_CIRCLE);
//		OLED_ShowSignedNum(1,9,AdvanceAngle,2);
		//- turn right
//		if(OutFlag==OUT)
//		{
//			if(LowPDnum>0)
//			{
//				if(AdvanceAngle==0)	OutFlag=IN;
//				else
//				{
//					RTargetVel=MaxV*(90-AngleUnit*GetSignedNum(AdvanceAngle))/90;
//					LTargetVel=MaxV*(90+AngleUnit*GetSignedNum(AdvanceAngle))/90;
//				}
//			}
//		}
		
//		if(AdvanceAngle==-2)
//		{
//			RTargetVel=MaxV;
//			LTargetVel=MaxV*2.3;
//		}
//		else if(AdvanceAngle==-1)
//		{
//			RTargetVel=MaxV;
//			LTargetVel=MaxV*1.5;
//		}
//		else if(AdvanceAngle==0)
//		{
//			RTargetVel=MaxV;
//			LTargetVel=MaxV;
//		}
//		else if(AdvanceAngle==1)
//		{
//			RTargetVel=MaxV*1.11;
//			LTargetVel=MaxV;
//		}
//		else if(AdvanceAngle==2)
//		{
//			RTargetVel=MaxV*1.4 ;
//			LTargetVel=MaxV;
//		}
//		OLED_ShowNum(1,1,(int32_t)RMotorCount+LMotorCount,16);
////		OLED_ShowNum(2,1,(int32_t)ChangeCont,2);
//		if(ChangePos[ChangeCont]<RMotorCount+LMotorCount)
//		{
////			BufFlag=!BufFlag;
////			if(BufFlag==1)
////			{
////				RMotorCount-=BUFF;
////				GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
////				ChangeCont--;
////				MaxV=25;
////				KL2=KBL2;
////				KL1=KBL1;
////				KM0=1;
////				KR1=KBR1;
////				
////			}
////			else
////			{
//				GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
//				ChangeCont++;
//				if(StartFlag==1)
//				{
//					if(ChangeCont%2==0)
//					{
//						MaxV=VS;
//						KL2=KSL2;
//						KL1=KSL1;
//						KM0=1;
//						KR1=KSR1;
//					}
//					else
//					{
//						MaxV=VT;
//						KL2=KTL2;
//						KL1=KTL1;
//						KM0=1;
//						KR1=KTR1;
//					}
//				}
////			}
//		}
//		else
//		{
//			GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
//		}

		if(AdvanceAngle==-2)
		{
			RTargetVel=MaxV*(2-KL2);
			LTargetVel=MaxV*KL2;
		}
		else if(AdvanceAngle==-1)
		{
			RTargetVel=MaxV*(2-KL1);
			LTargetVel=MaxV*KL1;
		}
		else if(AdvanceAngle==0)
		{
			RTargetVel=MaxV;
			LTargetVel=MaxV;
		}
		else if(AdvanceAngle==1)
		{
			RTargetVel=MaxV*KR1;
			LTargetVel=MaxV*(2-KR1);
		}
		if(AVGMotorCount>3700000 && LowPDnum==3)
			StartFlag=0;
		IncrementalPI();
//		if(AdvanceAngle==-2)
//		{
//			RPWM=MaxV;
//			LPWM=MaxV*1.79;
//		}
//		else if(AdvanceAngle==-1)
//		{
//			RPWM=MaxV;
//			LPWM=MaxV*1.3;
//		}
//		else if(AdvanceAngle==0)
//		{
//			RPWM=MaxV;
//			LPWM=MaxV;
//		}
//		else if(AdvanceAngle==1)
//		{
//			RPWM=MaxV*1.21;
//			LPWM=MaxV;
//		}
////		else if(AdvanceAngle==2)
////		{
////			RPWM=MaxV*1.4;
////			LPWM=MaxV;
//////		}
//		else
//		{
//			RPWM=0;
//			LPWM=0;
//		}
//		RTargetVel=MaxV*(90+AdvanceAngle)/90;
//		LTargetVel=MaxV*(90-AdvanceAngle)/90;
		
//		if(GPIO_ReadInputDataBit(IT_GPIOx,IFNOR_PIN1)==SET)		Get_AdvanceAngle(1, 0.01, FIRST_CIRCLE);
//		OLED_ShowSignedNum(1,3,(int)AdvanceAngle,5);
//		while(KeyNum--)
//		{
//			LED1_ON();
//			LED2_ON();
//			Delay_ms(100);
//			LED1_OFF();
//			LED2_OFF();
//			Delay_ms(20);
		}
	}
}
}
