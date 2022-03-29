
#ifndef  __LED_H
#define  __LED_H

#include "ACM32Fxx_HAL.h"

#define  TIM_CLOCK_FREQ            (1000U)   //定义的定时器频率  
#define  DEFAULT_BRIGHT            (200U)    //20%的亮度
#define  TIMER3_GLDB   TIM3
#define  TIMER1_RSW    TIM1

//Clean指示灯 CleanLED  PA8
#define  CleanLED_Port  GPIOA
#define  CleanLED_Pin   GPIO_PIN_8
#define  CleanLED_On    HAL_GPIO_WritePin(CleanLED_Port,CleanLED_Pin, GPIO_PIN_SET)
#define  CleanLED_Off   HAL_GPIO_WritePin(CleanLED_Port,CleanLED_Pin, GPIO_PIN_CLEAR)

//Filter按键灯 FilterLED  PA9
#define  FilterLED_Port  GPIOA
#define  FilterLED_Pin   GPIO_PIN_9
#define  FilterLED_On    HAL_GPIO_WritePin(FilterLED_Port,FilterLED_Pin, GPIO_PIN_SET)
#define  FilterLED_Off   HAL_GPIO_WritePin(FilterLED_Port,FilterLED_Pin, GPIO_PIN_CLEAR)

//Gold按键灯 GoldLED  PA7  TIM3_CH2
#define  GoldLED_Port       GPIOA
#define  GoldLED_Pin        GPIO_PIN_7
#define  GoldLED_CHANNEL    TIM_CHANNEL_2
#define  GoldLED_PWM        TIMER3_GLDB->CCR2
#define  GoldLED_On         TIMER3_GLDB->CCER |= BIT4
#define  GoldLED_Off        TIMER3_GLDB->CCER &= ~BIT4

//Light按键灯 LightLED  PB0  TIM3_CH3  
#define  LightLED_Port      GPIOB
#define  LightLED_Pin       GPIO_PIN_0
#define  LightLED_CHANNEL   TIM_CHANNEL_3
#define  LightLED_PWM       TIMER3_GLDB->CCR3
#define  LightLED_On        TIMER3_GLDB->CCER |= BIT8
#define  LightLED_Off       TIMER3_GLDB->CCER &= ~BIT8


//Dark按键灯 DarkLED  PB1  TIM3_CH4
#define  DarkLED_Port       GPIOB
#define  DarkLED_Pin        GPIO_PIN_1
#define  DarkLED_CHANNEL    TIM_CHANNEL_4
#define  DarkLED_PWM        TIMER3_GLDB->CCR4
#define  DarkLED_On         TIMER3_GLDB->CCER |= BIT12
#define  DarkLED_Off        TIMER3_GLDB->CCER &= ~BIT12


//Barista按键灯 BaristaLED  PB4  TIM3_CH1
#define  BaristaLED_Port        GPIOB
#define  BaristaLED_Pin         GPIO_PIN_4
#define  BaristaLED_CHANNEL     TIM_CHANNEL_1
#define  BaristaLED_PWM         TIMER3_GLDB->CCR1
#define  BaristaLED_On          TIMER3_GLDB->CCER |= BIT0
#define  BaristaLED_Off         TIMER3_GLDB->CCER &= ~BIT0

//Run按键灯 RunLED  PB15  TIM1_CH3N  呼吸
#define  RunLED_Port        GPIOB
#define  RunLED_Pin         GPIO_PIN_15
#define  RunLED_CHANNEL     TIM_CHANNEL_3
#define  RunLED_PWM         TIMER1_RSW->CCR3
#define  RunLED_On          TIMER1_RSW->CCER |= (BIT10|BIT8)
#define  RunLED_Off         TIMER1_RSW->CCER &= ~(BIT10|BIT8)
#define  RunLED_IntOn       TIM1->DIER |= BIT3    //开启中断
#define  RunLED_IntOff      TIM1->DIER &= ~BIT3   //关闭中断

//Small按键灯 SmallLED  PB14   TIM1_CH2N  20%亮度
#define  SmallLED_Port      GPIOB
#define  SmallLED_Pin       GPIO_PIN_14
#define  SmallLED_CHANNEL   TIM_CHANNEL_2
#define  SmallLED_PWM       TIMER1_RSW->CCR2
#define  SmallLED_On        TIMER1_RSW->CCER |= (BIT6|BIT4)
#define  SmallLED_Off       TIMER1_RSW->CCER &= ~(BIT6|BIT4)

//Warm按键灯 WarmLED  PB13    TIM1_CH1N  20%亮度
#define  WarmLED_Port       GPIOB
#define  WarmLED_Pin        GPIO_PIN_13
#define  WarmLED_CHANNEL    TIM_CHANNEL_1
#define  WarmLED_PWM        TIMER1_RSW->CCR1
#define  WarmLED_On         TIMER1_RSW->CCER |= (BIT2|BIT0)
#define  WarmLED_Off        TIMER1_RSW->CCER &= ~(BIT2|BIT0)




void Led_Init(void);
void Timer_Init(void);


#endif  

