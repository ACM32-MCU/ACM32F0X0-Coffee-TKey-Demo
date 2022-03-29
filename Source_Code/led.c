
#include "led.h"



//Led IO端口初始化
void Led_Init(void)
{
	GPIO_InitTypeDef   GPIO_Handle;
	//Clean指示灯 CleanLED  PA8
	CleanLED_Off;
	GPIO_Handle.Pin       = CleanLED_Pin;
	GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_Handle.Pull      = GPIO_NOPULL;
	GPIO_Handle.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(CleanLED_Port, &GPIO_Handle);		
	
	//Filter按键灯 FilterLED  PA9
	FilterLED_Off;
	GPIO_Handle.Pin       = FilterLED_Pin;
	GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_Handle.Pull      = GPIO_NOPULL;
	GPIO_Handle.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(FilterLED_Port, &GPIO_Handle);
    
    //Gold按键灯 GoldLED  PA7  TIM3_CH2
	GPIO_Handle.Pin  =  GoldLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(GoldLED_Port, &GPIO_Handle);   

	//Light按键灯 LightLED  PB0  TIM3_CH3  
	GPIO_Handle.Pin  =  LightLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(LightLED_Port, &GPIO_Handle); 

	//Dark按键灯 DarkLED  PB1  TIM3_CH4
	GPIO_Handle.Pin  =  DarkLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(DarkLED_Port, &GPIO_Handle); 

	//Barista按键灯 BaristaLED  PB4  TIM3_CH1
	GPIO_Handle.Pin  =  BaristaLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(BaristaLED_Port, &GPIO_Handle);
    
    //Run按键灯 RunLED  PB15  TIM1_CH3N  呼吸
	GPIO_Handle.Pin  =  RunLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_3;         
	HAL_GPIO_Init(RunLED_Port, &GPIO_Handle);   

	//Small按键灯 SmallLED  PB14   TIM1_CH2N  20%亮度
	GPIO_Handle.Pin  =  SmallLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_3;         
	HAL_GPIO_Init(SmallLED_Port, &GPIO_Handle); 

	//Warm按键灯 WarmLED  PB13    TIM1_CH1N  20%亮度
	GPIO_Handle.Pin  =  WarmLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_3;         
	HAL_GPIO_Init(WarmLED_Port, &GPIO_Handle); 
}

 
/*********************************************************************************
* Function    : TIM3_Init      
* Description : timer 1 initiation, includes clock, io, configuration 
* Input       : none    
* Output      : none 
* Author      : xwl                       
**********************************************************************************/  
void TIM3_Init(uint32_t  timer_clock)  
{
    TIM_HandleTypeDef   TIM3_Handler; 

	TIM3_Handler.Instance = TIM3;   //模块的地址
	TIM3_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;   //重装载 使能      
	TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //预分频
	TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;      //计数模式，向上计数
	TIM3_Handler.Init.RepetitionCounter = 0;                 //重复计数
	TIM3_Handler.Init.Prescaler = timer_clock/1000000 - 1;    //定时器预分频
	TIM3_Handler.Init.Period = TIM_CLOCK_FREQ-1;  // period = 10us      
	HAL_TIMER_MSP_Init(&TIM3_Handler);     //预分频初始化
	HAL_TIMER_Base_Init(&TIM3_Handler);   //时基 初始化
	
	//输出部分初始化  CCER（比较捕获使能寄存器） CCMR1  CCR1  CR2  
	TIM_OC_InitTypeDef   Tim_OC_Init_Para;     //初始化变量 
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;     //输出模式设置
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;   //OCI 空闲状态电频
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;   //OCN 空闲状态电频    
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_HIGH;   //N通道的有效极性  
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = DEFAULT_BRIGHT;   // 50% duty cycle   	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, GoldLED_CHANNEL);    
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, LightLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, DarkLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, BaristaLED_CHANNEL);

	//启动定时器
	TIM3->CR1 |= BIT0;
}


/*********************************************************************************
* Function    : TIM1_Init      
* Description : timer 1 initiation, includes clock, io, configuration 
* Input       : none    
* Output      : none 
* Author      : xwl                       
**********************************************************************************/  
void TIM1_Init(uint32_t  timer_clock)  
{
    TIM_HandleTypeDef   TIM1_Handler; 

	TIM1_Handler.Instance = TIM1;   //模块的地址
	TIM1_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;   //重装载 使能      
	TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //预分频
	TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;      //计数模式，向上计数
	TIM1_Handler.Init.RepetitionCounter = 2;                 //重复计数
	TIM1_Handler.Init.Prescaler = timer_clock/1000000 - 1;    //定时器预分频
	TIM1_Handler.Init.Period = TIM_CLOCK_FREQ-1;  // period = 10us      
	HAL_TIMER_MSP_Init(&TIM1_Handler);     //预分频初始化
	HAL_TIMER_Base_Init(&TIM1_Handler);   //时基初始化
	
	//输出部分初始化  CCER（比较捕获使能寄存器） CCMR1  CCR1  CR2  
	TIM_OC_InitTypeDef   Tim_OC_Init_Para;     //初始化变量 
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;     //输出模式设置
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;   //OCI 空闲状态电频
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;   //OCN 空闲状态电频    
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_LOW;   //N通道的有效极性  
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = DEFAULT_BRIGHT;   // 50% duty cycle   	
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, RunLED_CHANNEL);    
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, SmallLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, WarmLED_CHANNEL);	
	
	//开启通道3中断和总的中断
	NVIC_SetPriority(TIM1_CC_IRQn,2);    //设置中断优先级
	NVIC_ClearPendingIRQ(TIM1_CC_IRQn);  //清除中断标志位
	NVIC_EnableIRQ(TIM1_CC_IRQn);        //使能中断
	
	//启动定时器
	TIM1->BDTR |= BIT15;
	TIM1->CR1 |= BIT0;
}

//所有定时器的初始化
void Timer_Init(void)
{
	uint32_t  timer_clock;
	timer_clock = System_Get_APBClock( );   //取得APB时钟
	
	if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2  
	{
		 timer_clock =  System_Get_APBClock() << 1;    
	}	
	TIM3_Init(timer_clock);
	TIM1_Init(timer_clock);	
}


//定时器1 中断
void TIM1_CC_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(TIM1_CC_IRQn); 
	if(TIMER1_RSW->SR & BIT3)
	{
		TIMER1_RSW->SR &=  ~BIT3;   //清除中断标志
		static uint8_t RunLED_Direction=0;   //指示灯方向   0:变亮  1：变暗
		
		if(RunLED_Direction==0)   //变暗
		{
			RunLED_PWM++;
			if(RunLED_PWM>=TIM_CLOCK_FREQ-1)
            {
                RunLED_Direction = 1;
            }                    
		}
		else   //变亮
		{
			if(RunLED_PWM>1)
            {
				RunLED_PWM--;
            }
			else if(RunLED_PWM==1)
			{
				RunLED_Direction = 0; 
			}	
		}
	}
}


