/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/
#include "main.h"

uint8_t  ChannelState[7] = {0};

//触摸按键分布
#define GoldKey      14
#define LightKey     12
#define DarkKey       7
#define BaristaKey    4
#define RunKey        2
#define SmallKey      1
#define WarmKey       0

//按键处理函数
void TKey_Process(uint8_t KeyValue)
{
	switch(KeyValue)
	{
		case GoldKey:   
			if(ChannelState[0]++ > 2)
            {
				ChannelState[0] = 0;
            }
			switch(ChannelState[0])
			{
				case 0:
					TIM3->CCER &= (~BIT4); //关闭通道
					GoldLED_Off;
					break;
				case 1:    //启动定时器
					GoldLED_PWM = DEFAULT_BRIGHT;
					GoldLED_On; //使能输出通道
					break;	
				case 2:
					GoldLED_PWM = TIM_CLOCK_FREQ-1;
					GoldLED_On; //使能输出通道					
					break;				
			}
			break;
		case LightKey:
			if(ChannelState[1]++ > 2)
            {
				ChannelState[1] = 0;
            }
			switch(ChannelState[1])
			{
				case 0:
					LightLED_Off;
					break;
				case 1:    //启动定时器
					LightLED_PWM = DEFAULT_BRIGHT;
					LightLED_On; 
					break;	
				case 2:
					LightLED_PWM = TIM_CLOCK_FREQ-1;
					LightLED_On; 					
					break;				
			}			
			break;
		case DarkKey:
			if(ChannelState[2]++ >2)
            {
				ChannelState[2] = 0;
            }
			switch(ChannelState[2])
			{
				case 0:
					DarkLED_Off;
					break;
				case 1:    //启动定时器
					DarkLED_PWM = DEFAULT_BRIGHT;
					DarkLED_On; 
					break;	
				case 2:
					DarkLED_PWM = TIM_CLOCK_FREQ-1;
					DarkLED_On; 					
					break;				
			}			
			break;		
		case BaristaKey:
			if(ChannelState[3]++ > 2)
            {
				ChannelState[3] = 0;
            }
			switch(ChannelState[3])
			{
				case 0:
					BaristaLED_Off;
					break;
				case 1:    //启动定时器
					BaristaLED_PWM = DEFAULT_BRIGHT;
					BaristaLED_On; 
					break;	
				case 2:
					BaristaLED_PWM = TIM_CLOCK_FREQ-1;
					BaristaLED_On; 					
					break;				
			}			
			break;		
		case RunKey:
			if(ChannelState[4]++ > 2)
            {
				ChannelState[4] = 0;
            }
			switch(ChannelState[4])
			{
				case 0:
					RunLED_Off;
					RunLED_IntOff;  //中断关闭
					break;
				case 1:    //启动定时器
					RunLED_PWM = DEFAULT_BRIGHT;
					RunLED_On; 
					RunLED_IntOff;  //中断关闭
					break;	
				case 2:
					RunLED_PWM = 1;
					RunLED_On; 
					RunLED_IntOn;  //中断打开
					break;				
			}			
			break;	
		case SmallKey:
			if(ChannelState[5]++ > 2)
            {
				ChannelState[5] = 0;
            }
			switch(ChannelState[5])
			{
				case 0:
					SmallLED_Off;
					break;
				case 1:    //启动定时器
					SmallLED_PWM = DEFAULT_BRIGHT;
					SmallLED_On; 
					break;	
				case 2:
					SmallLED_PWM = TIM_CLOCK_FREQ-1;
					SmallLED_On; 					
					break;				
			}			
			break;
		case WarmKey:
			if(ChannelState[6]++ > 2)
            {
				ChannelState[6] = 0;
            }
			switch(ChannelState[6])
			{
				case 0:
					WarmLED_Off;
					break;
				case 1:    //启动定时器
					WarmLED_PWM = DEFAULT_BRIGHT;
					WarmLED_On; 
					break;	
				case 2:
					WarmLED_PWM = TIM_CLOCK_FREQ-1;
					WarmLED_On; 					
					break;				
			}			
			break;
	}
}


UART_HandleTypeDef   Uart_Handle;
void Uart_Init(uint32_t fu32_Baudrate)
{ 
    Uart_Handle.Instance = UART3;
    Uart_Handle.Init.BaudRate   = fu32_Baudrate;
    Uart_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart_Handle.Init.StopBits   = UART_STOPBITS_1;
    Uart_Handle.Init.Parity     = UART_PARITY_NONE;
    Uart_Handle.Init.Mode       = UART_MODE_TX_RX_DEBUG;
    Uart_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    HAL_UART_Init(&Uart_Handle);
	
    /* UART_DEBUG_ENABLE control printfS */   
    printfS("MCU is running, HCLK=%dHz, PCLK=%dHz\n", System_Get_SystemClock(), System_Get_APBClock());
}


/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Date : 2021  
**********************************************************************************/
int main(void)
{
    uint8_t  ucKey = 0xFF;
	//系统初始化
    System_Init();
  
	System_Delay(5000000);
  
	Uart_Init(115200);

	Led_Init( );
	
	Timer_Init( );
	
	
	
	TouchKey_Init( );
	
	
	while(1)
	{
		//触摸按键扫描
		ucKey = TK_TimerSacn_GetKeyVal();
        if(ucKey != 0xFF)
        {
        #ifndef TKEY_DEBUG
            printfS("TK_%d\n", ucKey);
        #endif
            TKey_Process(ucKey);
        }
        
		TKEY_Calibrate_Process();
			
	}
}

