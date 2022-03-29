
#include "led.h"



//Led IO�˿ڳ�ʼ��
void Led_Init(void)
{
	GPIO_InitTypeDef   GPIO_Handle;
	//Cleanָʾ�� CleanLED  PA8
	CleanLED_Off;
	GPIO_Handle.Pin       = CleanLED_Pin;
	GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_Handle.Pull      = GPIO_NOPULL;
	GPIO_Handle.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(CleanLED_Port, &GPIO_Handle);		
	
	//Filter������ FilterLED  PA9
	FilterLED_Off;
	GPIO_Handle.Pin       = FilterLED_Pin;
	GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_Handle.Pull      = GPIO_NOPULL;
	GPIO_Handle.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(FilterLED_Port, &GPIO_Handle);
    
    //Gold������ GoldLED  PA7  TIM3_CH2
	GPIO_Handle.Pin  =  GoldLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(GoldLED_Port, &GPIO_Handle);   

	//Light������ LightLED  PB0  TIM3_CH3  
	GPIO_Handle.Pin  =  LightLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(LightLED_Port, &GPIO_Handle); 

	//Dark������ DarkLED  PB1  TIM3_CH4
	GPIO_Handle.Pin  =  DarkLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(DarkLED_Port, &GPIO_Handle); 

	//Barista������ BaristaLED  PB4  TIM3_CH1
	GPIO_Handle.Pin  =  BaristaLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(BaristaLED_Port, &GPIO_Handle);
    
    //Run������ RunLED  PB15  TIM1_CH3N  ����
	GPIO_Handle.Pin  =  RunLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_3;         
	HAL_GPIO_Init(RunLED_Port, &GPIO_Handle);   

	//Small������ SmallLED  PB14   TIM1_CH2N  20%����
	GPIO_Handle.Pin  =  SmallLED_Pin;  
	GPIO_Handle.Mode =  GPIO_MODE_AF_PP;          
	GPIO_Handle.Pull =  GPIO_NOPULL;    
	GPIO_Handle.Alternate = GPIO_FUNCTION_3;         
	HAL_GPIO_Init(SmallLED_Port, &GPIO_Handle); 

	//Warm������ WarmLED  PB13    TIM1_CH1N  20%����
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

	TIM3_Handler.Instance = TIM3;   //ģ��ĵ�ַ
	TIM3_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;   //��װ�� ʹ��      
	TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //Ԥ��Ƶ
	TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;      //����ģʽ�����ϼ���
	TIM3_Handler.Init.RepetitionCounter = 0;                 //�ظ�����
	TIM3_Handler.Init.Prescaler = timer_clock/1000000 - 1;    //��ʱ��Ԥ��Ƶ
	TIM3_Handler.Init.Period = TIM_CLOCK_FREQ-1;  // period = 10us      
	HAL_TIMER_MSP_Init(&TIM3_Handler);     //Ԥ��Ƶ��ʼ��
	HAL_TIMER_Base_Init(&TIM3_Handler);   //ʱ�� ��ʼ��
	
	//������ֳ�ʼ��  CCER���Ƚϲ���ʹ�ܼĴ����� CCMR1  CCR1  CR2  
	TIM_OC_InitTypeDef   Tim_OC_Init_Para;     //��ʼ������ 
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;     //���ģʽ����
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;   //OCI ����״̬��Ƶ
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;   //OCN ����״̬��Ƶ    
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_HIGH;   //Nͨ������Ч����  
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = DEFAULT_BRIGHT;   // 50% duty cycle   	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, GoldLED_CHANNEL);    
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, LightLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, DarkLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM3_Handler.Instance, &Tim_OC_Init_Para, BaristaLED_CHANNEL);

	//������ʱ��
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

	TIM1_Handler.Instance = TIM1;   //ģ��ĵ�ַ
	TIM1_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;   //��װ�� ʹ��      
	TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //Ԥ��Ƶ
	TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;      //����ģʽ�����ϼ���
	TIM1_Handler.Init.RepetitionCounter = 2;                 //�ظ�����
	TIM1_Handler.Init.Prescaler = timer_clock/1000000 - 1;    //��ʱ��Ԥ��Ƶ
	TIM1_Handler.Init.Period = TIM_CLOCK_FREQ-1;  // period = 10us      
	HAL_TIMER_MSP_Init(&TIM1_Handler);     //Ԥ��Ƶ��ʼ��
	HAL_TIMER_Base_Init(&TIM1_Handler);   //ʱ����ʼ��
	
	//������ֳ�ʼ��  CCER���Ƚϲ���ʹ�ܼĴ����� CCMR1  CCR1  CR2  
	TIM_OC_InitTypeDef   Tim_OC_Init_Para;     //��ʼ������ 
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;     //���ģʽ����
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;   //OCI ����״̬��Ƶ
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;   //OCN ����״̬��Ƶ    
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_LOW;   //Nͨ������Ч����  
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = DEFAULT_BRIGHT;   // 50% duty cycle   	
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, RunLED_CHANNEL);    
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, SmallLED_CHANNEL);	
	HAL_TIMER_Output_Config(TIM1_Handler.Instance, &Tim_OC_Init_Para, WarmLED_CHANNEL);	
	
	//����ͨ��3�жϺ��ܵ��ж�
	NVIC_SetPriority(TIM1_CC_IRQn,2);    //�����ж����ȼ�
	NVIC_ClearPendingIRQ(TIM1_CC_IRQn);  //����жϱ�־λ
	NVIC_EnableIRQ(TIM1_CC_IRQn);        //ʹ���ж�
	
	//������ʱ��
	TIM1->BDTR |= BIT15;
	TIM1->CR1 |= BIT0;
}

//���ж�ʱ���ĳ�ʼ��
void Timer_Init(void)
{
	uint32_t  timer_clock;
	timer_clock = System_Get_APBClock( );   //ȡ��APBʱ��
	
	if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2  
	{
		 timer_clock =  System_Get_APBClock() << 1;    
	}	
	TIM3_Init(timer_clock);
	TIM1_Init(timer_clock);	
}


//��ʱ��1 �ж�
void TIM1_CC_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(TIM1_CC_IRQn); 
	if(TIMER1_RSW->SR & BIT3)
	{
		TIMER1_RSW->SR &=  ~BIT3;   //����жϱ�־
		static uint8_t RunLED_Direction=0;   //ָʾ�Ʒ���   0:����  1���䰵
		
		if(RunLED_Direction==0)   //�䰵
		{
			RunLED_PWM++;
			if(RunLED_PWM>=TIM_CLOCK_FREQ-1)
            {
                RunLED_Direction = 1;
            }                    
		}
		else   //����
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


