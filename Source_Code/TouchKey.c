
#include "TouchKey.h"
#include "ac_fifo.h"


extern volatile uint32_t gu32_SystemCount;

uint8_t gu8_TKEYScanTime;
uint16_t gu16_RawDataRangRatio;
uint8_t TKEY_FifoBuffer[TOTAL_CHANNELS];
FIFO_TypeDef TKEY_Fifo;    
static TKEY_HandleTypeDef TKEY_Handle;
TKEY_DataDef TKey_Channel_Data[TOTAL_CHANNELS]={0};
TKEY_CalibartData TKey_Calibart_Data[TOTAL_CHANNELS]={0};

/*****************************************************************
*   ����ÿ��ͨ����������
*   {DetectInTH[���·�ֵ], WakeUpTh[���ѷ�ֵ], }
******************************************************************/
static TKEY_BaseParaDef TKey_Channel_BasePara[TOTAL_CHANNELS] = {   
#ifdef  TKEY_CHANNEL_0_ENABLE   //TKEY_CHANNEL_0 UART_RX  
    {TKEY_0_SENSITIVITY, TKEY_0_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_1_ENABLE   //TKEY_CHANNEL_1   
    {TKEY_1_SENSITIVITY, TKEY_1_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_2_ENABLE   //TKEY_CHANNEL_2   
    {TKEY_2_SENSITIVITY, TKEY_2_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_3_ENABLE   //TKEY_CHANNEL_3 SWDIO   
    {TKEY_3_SENSITIVITY, TKEY_3_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_4_ENABLE   //TKEY_CHANNEL_4   
    {TKEY_4_SENSITIVITY, TKEY_4_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_5_ENABLE   //TKEY_CHANNEL_5   
    {TKEY_5_SENSITIVITY, TKEY_5_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_6_ENABLE   //TKEY_CHANNEL_6 SWDCLK   
    {TKEY_6_SENSITIVITY, TKEY_6_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_7_ENABLE   //TKEY_CHANNEL_7   
    {TKEY_7_SENSITIVITY, TKEY_7_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_8_ENABLE   //TKEY_CHANNEL_8   
    {TKEY_8_SENSITIVITY, TKEY_8_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_9_ENABLE   //TKEY_CHANNEL_9   
    {TKEY_9_SENSITIVITY, TKEY_9_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_10_ENABLE  //TKEY_CHANNEL_10   
    {TKEY_10_SENSITIVITY, TKEY_10_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_11_ENABLE  //TKEY_CHANNEL_11   
    {TKEY_11_SENSITIVITY, TKEY_11_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_12_ENABLE  //TKEY_CHANNEL_12   
    {TKEY_12_SENSITIVITY, TKEY_12_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_13_ENABLE  //TKEY_CHANNEL_13   
    {TKEY_13_SENSITIVITY, TKEY_13_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_14_ENABLE  //TKEY_CHANNEL_14   
    {TKEY_14_SENSITIVITY, TKEY_14_SLEEP_SENSITIVITY}, 
#endif
#ifdef  TKEY_CHANNEL_15_ENABLE  //TKEY_CHANNEL_15   
    {TKEY_15_SENSITIVITY, TKEY_15_SLEEP_SENSITIVITY},
#endif
};


/*****************************************************************
*   TKeyͨ������
*   ChannelIdΪTKEY_CHANNEL_1-TKEY_CHANNEL_15
******************************************************************/
const TKEY_ChannelDataDef TKey_Config[] = {	
#ifdef  TKEY_CHANNEL_0_ENABLE  
    {&TKey_Channel_BasePara[TK_0], &TKey_Channel_Data[TK_0], &TKey_Calibart_Data[TK_0], TKEY_CHANNEL_0 },
#endif
#ifdef  TKEY_CHANNEL_1_ENABLE     
    {&TKey_Channel_BasePara[TK_1], &TKey_Channel_Data[TK_1], &TKey_Calibart_Data[TK_1], TKEY_CHANNEL_1 },
#endif
#ifdef  TKEY_CHANNEL_2_ENABLE      
    {&TKey_Channel_BasePara[TK_2], &TKey_Channel_Data[TK_2], &TKey_Calibart_Data[TK_2], TKEY_CHANNEL_2 },
#endif
#ifdef  TKEY_CHANNEL_3_ENABLE     
    {&TKey_Channel_BasePara[TK_3], &TKey_Channel_Data[TK_3], &TKey_Calibart_Data[TK_3], TKEY_CHANNEL_3 },
#endif
#ifdef  TKEY_CHANNEL_4_ENABLE     
    {&TKey_Channel_BasePara[TK_4], &TKey_Channel_Data[TK_4], &TKey_Calibart_Data[TK_4], TKEY_CHANNEL_4 },
#endif
#ifdef  TKEY_CHANNEL_5_ENABLE      
    {&TKey_Channel_BasePara[TK_5], &TKey_Channel_Data[TK_5], &TKey_Calibart_Data[TK_5], TKEY_CHANNEL_5 },
#endif
#ifdef  TKEY_CHANNEL_6_ENABLE     
    {&TKey_Channel_BasePara[TK_6], &TKey_Channel_Data[TK_6], &TKey_Calibart_Data[TK_6], TKEY_CHANNEL_6 },
#endif
#ifdef  TKEY_CHANNEL_7_ENABLE      
    {&TKey_Channel_BasePara[TK_7], &TKey_Channel_Data[TK_7], &TKey_Calibart_Data[TK_7], TKEY_CHANNEL_7 },
#endif
#ifdef  TKEY_CHANNEL_8_ENABLE      
    {&TKey_Channel_BasePara[TK_8], &TKey_Channel_Data[TK_8], &TKey_Calibart_Data[TK_8], TKEY_CHANNEL_8 },
#endif
#ifdef  TKEY_CHANNEL_9_ENABLE      
    {&TKey_Channel_BasePara[TK_9], &TKey_Channel_Data[TK_9], &TKey_Calibart_Data[TK_9], TKEY_CHANNEL_9 },
#endif
#ifdef  TKEY_CHANNEL_10_ENABLE      
    {&TKey_Channel_BasePara[TK_10], &TKey_Channel_Data[TK_10], &TKey_Calibart_Data[TK_10], TKEY_CHANNEL_10},
#endif
#ifdef  TKEY_CHANNEL_11_ENABLE      
    {&TKey_Channel_BasePara[TK_11], &TKey_Channel_Data[TK_11], &TKey_Calibart_Data[TK_11], TKEY_CHANNEL_11},
#endif
#ifdef  TKEY_CHANNEL_12_ENABLE      
    {&TKey_Channel_BasePara[TK_12], &TKey_Channel_Data[TK_12], &TKey_Calibart_Data[TK_12], TKEY_CHANNEL_12},
#endif
#ifdef  TKEY_CHANNEL_13_ENABLE      
    {&TKey_Channel_BasePara[TK_13], &TKey_Channel_Data[TK_13], &TKey_Calibart_Data[TK_13], TKEY_CHANNEL_13},
#endif
#ifdef  TKEY_CHANNEL_14_ENABLE      
    {&TKey_Channel_BasePara[TK_14], &TKey_Channel_Data[TK_14], &TKey_Calibart_Data[TK_14], TKEY_CHANNEL_14},
#endif
#ifdef  TKEY_CHANNEL_15_ENABLE      
    {&TKey_Channel_BasePara[TK_15], &TKey_Channel_Data[TK_15], &TKey_Calibart_Data[TK_15], TKEY_CHANNEL_15},
#endif
   
    {0, 0, 0, TKEY_CHANNEL_NULL},
 
};
    

/*****************************************************************
@����	TKEY��ʱ�����á�
@����	�� 
@����	��
******************************************************************/
static void TKEY_SCAN_TIMX_Cfg(void)  
{
    uint32_t timer_clock;
    TIM_HandleTypeDef TIMx_Handler;  
    
    timer_clock = System_Get_APBClock(); 
    
    if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2  
    {
       timer_clock =  System_Get_APBClock() << 1;    
    }
    
	TIMx_Handler.Instance = TKEY_SCAN_TIMX;
	TIMx_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	TIMx_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIMx_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIMx_Handler.Init.RepetitionCounter = 0;  
	TIMx_Handler.Init.Prescaler = (timer_clock/1000) - 1;  // count++ every 1ms 
	TIMx_Handler.Init.Period = TKEY_SCAN_TIM_PERIOD - 1;  //1ms * TKEY_SCAN_TIME   
	
    HAL_TIMER_MSP_Init(&TIMx_Handler);  
	HAL_TIMER_Base_Init(&TIMx_Handler);  
	HAL_TIM_ENABLE_IT(&TIMx_Handler, TIMER_INT_EN_UPD);
    NVIC_SetPriority(TKEY_SCAN_TIMX_IRQn, TKEY_SCAN_TIMX_IRQn_PRIORITY);      
}

/*****************************************************************
@����	TKEY��ʱ���жϴ�������
@����	�� 
@����	��
******************************************************************/
void TKEY_SCAN_TIMX_IRQHandler(void)
{
	if (TKEY_SCAN_TIMX->SR & TIMER_SR_UIF)
	{
		if(TKEY->ISR & TKEY_ISR_EOC)//����ͨ��ɨ�������Լ��Ҫ31ms
        {
            TKEY->ISR = TKEY_ISR_EOC;
            HAL_TKEY_ScanProcess(&TKEY_Handle);  //��ʱ1ms
            if(TKEY_Handle.ChannelDetected)
            {
                if(TKEY_Handle.ChannelDetectedNum  == 1)
                {
                    FIFO_In(&TKEY_Fifo, TKEY_Handle.ChannelValue);
                }
                TKEY_Handle.ChannelDetectedNum = 0;
                TKEY_Handle.ChannelDetected = 0;
                TKEY_Handle.ChannelValue = 0;     
            }
        }
        else if(!READ_BIT(TKEY->ISR, TKEY_ISR_BUSY)) //Some times will stop.restart. 
        {
            SET_BIT(TKEY->CR, TKEY_CR_START);
        }     
        
        else if(READ_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT))
        {
            SET_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT); //Clear the timeout flag
        }
	}
	
	TKEY_SCAN_TIMX->SR = 0;   //write 0 to clear hardware flag  
}

/*****************************************************************
@����	TKEY��ʱɨ���ʼ����
@����	��
@����	��
******************************************************************/
void TKEY_Timer_ScanInit(void)
{
    memset(TKEY_FifoBuffer, 0, sizeof(TKEY_FifoBuffer));
    FIFO_Create(&TKEY_Fifo, TKEY_FifoBuffer, sizeof(TKEY_FifoBuffer));
    FIFO_Clear(&TKEY_Fifo);
    TKEY_SCAN_TIMX_Cfg();                 
}

/*****************************************************************
@����	���TKEY��ʱɨ��FIFO��
@����	��
@����	��
******************************************************************/
void TKEY_Scan_FIFO_Clear(void)
{
    memset(TKEY_FifoBuffer, 0, sizeof(TKEY_FifoBuffer));
    FIFO_Clear(&TKEY_Fifo);              
}

/*****************************************************************
@����	����TKEY��ʱɨ�衣
@����	��
@����	��
******************************************************************/
void TKEY_Timer_Scan_Start(void)
{     
    HAL_TIMER_Base_Start(TIM17);        //TIM17 ����
}

/*****************************************************************
@����	�ر�TKEY��ʱɨ�衣
@����	��
@����	��
******************************************************************/
extern HAL_StatusTypeDef HAL_TIMER_Base_Stop(TIM_TypeDef *TIMx);
void TKEY_Timer_Scan_Stop(void)
{
    HAL_TIMER_Base_Stop(TIM17);        	 
}

/*****************************************************************
@����	��ȡTKEY��ʱɨ���ֵ��
@����	pucKey[OUT] ��ֵ
@����	��
******************************************************************/
void TKEY_Timer_Scan_Result(uint8_t *pucKey)
{
    *pucKey = 0xFF;
    if(FIFO_Query(&TKEY_Fifo))
    {
        FIFO_Out(&TKEY_Fifo, pucKey);
    }
}

/*****************************************************************
@����	TKey��ʼ����
@����	��
@����	��
******************************************************************/
void TKEY_Init(void)
{    
    gu8_TKEYScanTime = TKEY_SCAN_WAIT_TIMES;
    gu16_RawDataRangRatio = TKEY_RAWDATA_RANGE_RATIO;
    TKEY_Handle.Init.ShieldEn = TKEY_CR_SHIELDEN_DISABLE;
    TKEY_Handle.Init.ScanWaitTime = gu8_TKEYScanTime;
    TKEY_Handle.TotalChannelNum = TOTAL_CHANNELS;
    TKEY_Handle.ChannelData = TKey_Config;
    HAL_TKEY_Init(&TKEY_Handle); 
}

/*****************************************************************
@����	TKeyУ�������ʼ����
@����	��
@����	��
******************************************************************/
void TKEY_Calibrate_ParaInit(void)
{
    uint8_t ucI;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->DetectingTimer = 0;
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;  
    }
  
}
/*****************************************************************
@����	TKey��������У׼��
@����	��
@����	0-�ɹ�����0-ʧ�ܣ�1-ɨ��ȴ�ʱ���쳣��2-RawData������Χ�쳣
******************************************************************/
uint8_t TKEY_Quick_Calibrate(void)
{
    uint8_t ucI,ucJ = 0;
    static uint16_t RawData_Ref[TOTAL_CHANNELS] = {0};
    uint32_t RawData_Sum[TOTAL_CHANNELS] = {0};
    uint8_t count[TOTAL_CHANNELS] = {0};
    __IO uint32_t *gu32RegTemp;
    uint8_t Temp_Delta = 0;
    uint8_t Temp_timeout_cnt = 0;
    uint8_t Temp_outrange_cnt = 0;

QUICK_CALIBRATE:
    for(ucJ = 0; ucJ < TKEY_QUICK_QUICK_CALIBRATE_TIMES; ucJ++)
    {
        TKEY->ISR = 0x07;
        if(!(TKEY->CR & TKEY_CR_CONT))
        {
            /*Start the Tkey scan*/    
            SET_BIT(TKEY->CR, TKEY_CR_START); 
        }
        while(!READ_BIT(TKEY->ISR, TKEY_ISR_EOC))
        {
            if(!READ_BIT(TKEY->ISR, TKEY_ISR_BUSY)) //Some times will stop.restart. 
                SET_BIT(TKEY->CR, TKEY_CR_START); 
            
            if(READ_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT))
            {
                SET_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT); //Clear the timeout flag
                Temp_timeout_cnt++;
                if(Temp_timeout_cnt == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    Temp_timeout_cnt = 0;                
                    //�Զ�����ɨ��ȴ�ʱ��:gu8_TKEYScanTime��ÿ�����ӵ�ǰֵ��(1/TKEY_SCAN_WAIT_TIMES_RATIO)
                    gu8_TKEYScanTime += (gu8_TKEYScanTime / TKEY_SCAN_WAIT_TIMES_RATIO);
                    if(gu8_TKEYScanTime > TKEY_SCAN_WAIT_MAX_TIMES)
                    {
                        // printfS("gu8_TKEYScanTime more than TKEY_SCAN_WAIT_MAX_TIMES\n");
                        return 1;
                    } 
                    // printfS("gu8_TKEYScanTime = %d\n", gu8_TKEYScanTime);
                    /*Config the Tkey TKEY_SMPR register*/
                    MODIFY_REG(TKEY->SMPR, TKEY_SMPR_SWT_MASK, TKEY_SMPR_SWT(gu8_TKEYScanTime));  
                }
                goto QUICK_CALIBRATE;
            }
        }
        TKEY->ISR = TKEY_ISR_EOC;
        gu32RegTemp = &TKEY->CH0;

        for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
        {
            /* Read the data and calculate the delta.*/
            TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData = *(gu32RegTemp + TKEY_Handle.ChannelData[ucI].ChannelId);
            Temp_Delta = TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData / gu16_RawDataRangRatio;
            // printfS("\n[%d]--RawData[%d]=%d,RawData_Ref[%d]=%d\n",ucJ,ucI,TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData,ucI,RawData_Ref[ucI]); 
            if((RawData_Ref[ucI])&&((RawData_Ref[ucI] > (INT32)(TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData + Temp_Delta)) \
            || (RawData_Ref[ucI] < ((INT32)(TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData - Temp_Delta)))))
            {
                for(uint8_t i=0;TKEY_Handle.ChannelData[i].ChannelId != 0xFFFF;i++)
                {
                    count[i] = 0;
                    RawData_Ref[i] = 0;
                    RawData_Sum[i] = 0;
                    // printfS("count[%d]=%d,RawData_Ref[%d]=%d,RawData_Sum[%d]=%d\n",i,count[i],i,RawData_Ref[i],i,RawData_Sum[i]); 
                }
                Temp_outrange_cnt++; 
                if(Temp_outrange_cnt == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    Temp_outrange_cnt = 0;                
                    //�Զ���СRawData���²�����Χϵ��:gu16_RawDataRangRatio,ÿ�μ�С��ǰֵ��(1/TKEY_RAWDATA_RANGE_DEC_RATIO)
                    gu16_RawDataRangRatio -= (gu16_RawDataRangRatio / TKEY_RAWDATA_RANGE_DEC_RATIO); 
                    if(gu16_RawDataRangRatio < TKEY_RAWDATA_RANGE_MIN_RATIO)
                    {
                        // printfS("gu16_RawDataRangRatio less than TKEY_RAWDATA_RANGE_MIN_RATIO\n");
                        return 2;
                    } 
                    // printfS("[%d] Error, gu16_RawDataRangRatio = %d\n", ucI, gu16_RawDataRangRatio);
                }
                goto  QUICK_CALIBRATE;            
            }
            else
            {
                RawData_Sum[ucI] +=  TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData;             
                count[ucI]++;
                // printfS("RawData_Sum[%d]=%d,count[%d]=%d\n",ucI,RawData_Sum[ucI],ucI,count[ucI]);  
                if(count[ucI] == TKEY_QUICK_QUICK_CALIBRATE_TIMES)
                {
                    TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine =  RawData_Sum[ucI] / TKEY_QUICK_QUICK_CALIBRATE_TIMES;
                    // printfS("BaseLine[%d]=%d\n",ucI,TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine);
                }
            }
            RawData_Ref[ucI] = (INT32)TKEY_Handle.ChannelData[ucI].Tkey_Data->RawData;
        }
    } 
    //У׼������ɨ��ȴ�ʱ����������
    gu8_TKEYScanTime += (gu8_TKEYScanTime / TKEY_SCAN_WAIT_TIMES_RATIO);
    // printfS("gu8_TKEYScanTime = %d\n", gu8_TKEYScanTime); 
    /*Config the Tkey TKEY_SMPR register*/
    MODIFY_REG(TKEY->SMPR, TKEY_SMPR_SWT_MASK, TKEY_SMPR_SWT(gu8_TKEYScanTime)); 
    return 0; 
}

/*****************************************************************
@����	TKey������У׼����
@����	��
@����	��
******************************************************************/
void TKEY_Calibrate_Process(void)
{
    uint8_t ucI;
    uint16_t tempMax = 0, tempMin = 0;
    const TKEY_ChannelDataDef *ChannelData;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        ChannelData = &TKEY_Handle.ChannelData[ucI];
        if(ChannelData->Tkey_CalData->CalFlag == TKEY_CALIBRAT_SELF)//����У׼
        {
            Find_Max_Min(ChannelData->Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX, &tempMax, &tempMin);
            // printfS("ChannelData[%d]--max(%d)-min(%d)=%d,DetectInTH=%d,CalibratTH=%d\n", ucI, tempMax, tempMin,tempMax-tempMin,ChannelData->Tkey_RefPara->DetectInTH,ChannelData->Tkey_Data->CalibratTH);
            if((tempMax) && ((tempMax - tempMin) < ChannelData->Tkey_Data->CalibratTH))
            {
                Upgraded_Bubble_Sort(ChannelData->Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX);
                ChannelData->Tkey_Data->BaseLine = (ChannelData->Tkey_CalData->Caldata[9] + ChannelData->Tkey_CalData->Caldata[10]) / 2;
                // printfS("BaseLine[%d]=%d\n",ucI,ChannelData->Tkey_Data->BaseLine);
            }
            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;
        }
        else if(ChannelData->Tkey_CalData->CalFlag == TKEY_CALIBRAT_QUICK)//����У׼
        {
            ChannelData->Tkey_CalData->CalFlag = TKEY_CALIBRAT_IDLE;
            // printfS("TKEY_Quick_Calibrate\n");
            TKEY_Quick_Calibrate();
        } 
    }
}

/*****************************************************************
@����	TKey������У׼����
@����	��
@����	��
******************************************************************/
void TKEY_RTC_Calibrate_Process(void)
{
    uint8_t ucI;
    __IO uint32_t *gu32RegTemp;
    uint16_t tempMax = 0, tempMin = 0;

    TKEY->ISR = 0x07;
    while(!READ_BIT(TKEY->ISR, TKEY_ISR_EOC))
    {
        if(!READ_BIT(TKEY->ISR, TKEY_ISR_BUSY)) //Some times will stop.restart. 
            SET_BIT(TKEY->CR, TKEY_CR_START); 
        
        if(READ_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT))
        {
            SET_BIT(TKEY->ISR, TKEY_ISR_TIMEOUT); //Clear the timeout flag
            return;
        }
    }
    TKEY->ISR = TKEY_ISR_EOC;
    gu32RegTemp = &TKEY->CH0;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos] = *(gu32RegTemp + TKEY_Handle.ChannelData[ucI].ChannelId);
        if((TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer)++ >= TKEY_CALIBRATE_TIMES)
        {
            TKEY_Handle.ChannelData[ucI].Tkey_CalData->ScanTimer = 0;
            // printfS("[%d] need Calibrate]\n", ucI);
            Find_Max_Min(TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX, &tempMax, &tempMin);
            // printfS("ChannelData[%d]--max(%d)-min(%d)=%d,DetectInTH=%d,CalibratTH=%d\n", ucI, tempMax, tempMin,tempMax-tempMin,ChannelData->Tkey_RefPara->DetectInTH,ChannelData->Tkey_Data->CalibratTH);
            if((tempMax) && ((tempMax - tempMin) < TKEY_Handle.ChannelData[ucI].Tkey_Data->CalibratTH))
            {
                Upgraded_Bubble_Sort(TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata, TKEY_CALIBRATE_BUFF_MAX);
                TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine = (TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[9] + TKEY_Handle.ChannelData[ucI].Tkey_CalData->Caldata[10]) / 2;
                // printfS("BaseLine[%d]=%d\n",ucI,TKEY_Handle.ChannelData[ucI].Tkey_Data->BaseLine);
            }
        }
        if((TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos)++ >= TKEY_CALIBRATE_BUFF_MAX)
        {
            TKEY_Handle.ChannelData[ucI].Tkey_CalData->pos = 0;
        }
    }
     
}

/*****************************************************************
@����	TK�����������
@����	�� 
@����   ��  
@����	��
******************************************************************/
void TK_DebugDataOut(void)
{
    extern UART_HandleTypeDef   Uart_Handle;
    uint8_t ucI, i, tk_ch_h, tk_ch_l;
    uint8_t length = 0;
    unsigned short data_checksum = 0;
    
    unsigned short raw_data[16];
    unsigned short base_line[16];
    
    uint8_t gucSendBuff[128], ucSendLength;

    uint8_t opened_tk_amount = TOTAL_CHANNELS;

    for(i = 0; i < opened_tk_amount; i++)
    {
        raw_data[i] = TKEY_Handle.ChannelData[i].Tkey_Data->RawData;

        base_line[i] = TKEY_Handle.ChannelData[i].Tkey_Data->BaseLine;
    }

    tk_ch_h = 0;
    tk_ch_l = 0;
    for(ucI = 0; TKEY_Handle.ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
        if(TKEY_Handle.ChannelData[ucI].ChannelId < 8)
        {
            tk_ch_l |= (1 << TKEY_Handle.ChannelData[ucI].ChannelId);
        }
        else
        {
            tk_ch_h |= (1 << (TKEY_Handle.ChannelData[ucI].ChannelId-8));
        }
    }

    ucSendLength = 0;
    memset(gucSendBuff, 0x00, sizeof(gucSendBuff));
    gucSendBuff[ucSendLength++] = 0x55;
    gucSendBuff[ucSendLength++] = 0xAA;
    gucSendBuff[ucSendLength++] = 0x55;
    gucSendBuff[ucSendLength++] = 0xAA;
    
    length = 1 + 2 + 4 + 1 + 16 + 4 * opened_tk_amount + 2;
    
    gucSendBuff[ucSendLength++] = length;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;
    gucSendBuff[ucSendLength++] = 0;

    data_checksum = length;

    gucSendBuff[ucSendLength++] = tk_ch_h;
    gucSendBuff[ucSendLength++] = tk_ch_l;

    //TK Channel H
    data_checksum += tk_ch_h;
    
    //TK Channel L
    data_checksum += tk_ch_l;

    gucSendBuff[ucSendLength++] = 16;
    //TK Channel MAX
    data_checksum += 16;

    for(i = 0; i < 16; i++)
    {
        gucSendBuff[ucSendLength++] = i;
        data_checksum += i;
    }
    
    for(i = 0; i < opened_tk_amount; i++)
    {
        gucSendBuff[ucSendLength++] = (uint8_t)(base_line[i] >> 8);
        gucSendBuff[ucSendLength++] = (uint8_t)base_line[i];
        data_checksum += (uint8_t)(base_line[i] >> 8);
        data_checksum += (uint8_t)base_line[i];
    }
    
    for(i = 0; i < opened_tk_amount; i++)
    {
        gucSendBuff[ucSendLength++] = (uint8_t)(raw_data[i] >> 8);
        gucSendBuff[ucSendLength++] = (uint8_t)raw_data[i];
        data_checksum += (uint8_t)(raw_data[i] >> 8);
        data_checksum += (uint8_t)raw_data[i];
    }

    data_checksum ^= 0x55AA;

    gucSendBuff[ucSendLength++] = (uint8_t)(data_checksum >> 8);
    gucSendBuff[ucSendLength++] = (uint8_t)data_checksum;
    
    HAL_UART_Transmit(&Uart_Handle, gucSendBuff, ucSendLength, 0);  
}

/*****************************************************************
@����	TKey���������������ⰴ�°���ʱ�Ĳ�ֵ(��Nxֵ)
@����	��
@����	��
******************************************************************/
void TKEY_Calibrate_RefDelta_Debug(void)
{
    while(1)
    {
        HAL_TKEY_ReadChannelData(&TKEY_Handle); 
        TK_DebugDataOut();  //Send the date to the PC tools
    }    
}

/*****************************************************************
@����	TKey˯�ߴ���
@����	��
@����	��
******************************************************************/
void TKEY_Suspend(void)
{
    HAL_TKEY_Suspend(&TKEY_Handle); //Set the sleep parameters. 
}

/*****************************************************************
@����	TKey���Ѵ���
@����	��
@����	��
******************************************************************/
void TKEY_Resume(void)
{
    HAL_TKEY_Resume(&TKEY_Handle);  //Resume the wakeup parameters.
}

/*****************************************************************
@����	TKey��������
@����	��
@����	��
******************************************************************/
void TKEY_Wakeup(void)
{
    
}

/*****************************************************************
@����	����˯��
@����	��
@����	��
******************************************************************/
void TouchKey_suspend(void)
{
    TKEY_Timer_Scan_Stop();
    TKEY_Suspend();
}

/*****************************************************************
@����	�������ѻָ�
@����	��
@����	��
******************************************************************/
void TouchKey_resume(void)
{
    TKEY_Resume();
    TKEY_Timer_ScanInit();
    TKEY_Timer_Scan_Start();
    printfS("gu16_RawDataRangRatio = %d\n", gu16_RawDataRangRatio);
    printfS("gu8_TKEYScanTime = %d\n", gu8_TKEYScanTime); 
}

/*****************************************************************
@ԭ��	void TouchKey_Init(void)
@����	����ģ�������ʼ��
@����	��
@����	��
******************************************************************/
void TouchKey_Init(void)
{
    uint8_t ret;
    TKEY_Init();
    TKEY_Calibrate_ParaInit();
    ret = TKEY_Quick_Calibrate(); //У׼�޴���ʱ�Ķ�ȡ���ݼĴ���ֵ 
    if(ret)
    {
        printfS("TKEY_Quick_Calibrate Fail return %d\n", ret);
    }
    TKEY_Timer_ScanInit();
    TKEY_Timer_Scan_Start(); 
    printfS("TKEY is Reading\n");
}

/*****************************************************************
@����	TKeyɨ�貢��ȡ��ֵ��
@����	��
@����	TK_NULL,��ֵ
******************************************************************/
uint8_t TK_Sacn_GetKeyVal(void)
{
    uint8_t  ucKey = 0xFF; 
    HAL_TKEY_DetectProcess(&TKEY_Handle);        
            
    if(TKEY_Handle.ChannelDetected)
    {
        if(TKEY_Handle.ChannelDetectedNum  == 1)
        {
            ucKey = TKEY_Handle.ChannelValue;
        }
        TKEY_Handle.ChannelDetectedNum = 0;
        TKEY_Handle.ChannelDetected = 0;
        TKEY_Handle.ChannelValue = 0;
    } 
    return ucKey;
}


/*****************************************************************
@����	TKey��ʱɨ�貢��ȡ��ֵ��
@����	��
@����	TK_NULL,��ֵ
******************************************************************/
uint8_t TK_TimerSacn_GetKeyVal(void)
{
	uint8_t  ucKey = 0xFF;
	TKEY_Timer_Scan_Result(&ucKey);
#ifdef TKEY_DEBUG    
    TK_DebugDataOut();
#endif
    return ucKey;
}


/*****************************************************************
@����	ð�������㷨: �Ƚ����ڵ������������ǰ�߱Ⱥ��ߴ�����н�����
        ÿһ�����������ѡ��һ��δ�������������ŵ�������档
@����	arr: ָ��Ҫת��������
@����   n[IN]:Ҫת�������ݳ���
@����	��	
******************************************************************/
void Bubble_Sort(uint16_t *arr, uint16_t n) 
{
    uint16_t i, j, temp;
    for (i = 0; i<n - 1; i++)
	{
        for (j = 0; j < n - i - 1; j++)
        {
            //���ǰ������Ⱥ���󣬽��н���
            if (arr[j] > arr[j + 1]) 
            {
                temp = arr[j]; 
                arr[j] = arr[j + 1]; 
                arr[j + 1] = temp;
            }
        }
	}
}

/*****************************************************************
@����	������ð������: �㷨ͨ���ӵ͵���ѡ���������ŵ����棬�ٴӸߵ���ѡ����С�����ŵ�ǰ�棬
        ��˷�����ֱ����߽���ұ߽��غϡ�����������������õ���ʱ����������ȴ�ͳð�����������Ժá�
@����	arr: ָ��Ҫת��������
@����   n[IN]:Ҫת�������ݳ���
@����	��	
******************************************************************/
void Upgraded_Bubble_Sort(uint16_t *arr, uint16_t n) 
{
    uint16_t i, j, temp;
    //�����������ұ߽�
    uint16_t left = 0, right = n - 1;
    //�����ұ߽�δ�غ�ʱ����������
    while (left < right) 
    {
        //�����ұ���ѡ���������ŵ������ұ�
        for (i = left; i < right; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i]; 
                arr[i] = arr[i + 1]; 
                arr[i + 1] = temp;
            }
        }
        right--;
        //���ҵ������ѡ����С�����ŵ��������
        for (j = right; j > left; j--)
        {
            if (arr[j + 1] < arr[j])
            {
                temp = arr[j]; 
                arr[j] = arr[j + 1]; 
                arr[j + 1] = temp;
            }
        }
        left++;
    }
}


/*****************************************************************
@����	�������ֵ����Сֵ
@����	arr: ָ��Ҫ���ҵ�����
@����   max: ָ�����ֵ
@����   min: ָ����Сֵ
@����	��	
******************************************************************/
void Find_Max_Min(uint16_t *arr, uint16_t n, uint16_t *max, uint16_t *min)
{
    uint16_t i, tempMax, tempMin;
    tempMax = arr[0];            // �ٶ���һ����Ϊ���ֵ
    tempMin = arr[0];            // ͬʱ�ٶ�Ҳ��һ����Ϊ���ֵ

    /* �ӵڶ�������ʼ�Ƚϲ�����ֵ */
    for (i = 1; i < n; i++)
    {
        if (arr[i] > tempMax)    // ���ֵ
    	{
    	    tempMax = arr[i];
    	}

    	if (arr[i] < tempMin)   // ��Сֵ������ʹ��else if������������Ƚ�
    	{
    	    tempMin = arr[i];
    	}
    }
    *max = tempMax;
    *min = tempMin;
}


