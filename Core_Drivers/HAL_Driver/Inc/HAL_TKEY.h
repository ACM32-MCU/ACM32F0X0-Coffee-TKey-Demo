/***********************************************************************
 * Filename    : HAL_TKEY.h
 * Description : HAL TKEY driver header file
 * Author(s)   : Xiao Han
 * version     : V1.0
 * Modify date : 2020-04-13 
 * @修改记录:	
修改日期		修改人   		修改说明
2021-12-03      Sam            添加自动校准机制 
 ***********************************************************************/
#ifndef __HAL_TKEY_H__
#define __HAL_TKEY_H__

#include "ACM32Fxx_HAL.h"


#define TKEY_CSDISCHARGETIME        1
#define TKEY_SW1H                   2
#define TKEY_SW1L                   2
#define TKEY_CR_SELECT              TKEY_CRSELECT_DISABLE

#define TKEY_SLEEP_SCAN_SPEED       100   //睡眠时TKey扫描速度

#define TKEY_DEBIN                  0  //按下滤波次数
#define TKEY_DEBOUT                 0  //弹起滤波次数

#define TKEY_RELAESE_THRATIO        90    //释放比列系数，用于计算释放阈值，按下阈值DetectInTH的70%
#define TKEY_CALIBRATE_RATIO20      20    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的20%
#define TKEY_CALIBRATE_RATIO30      30    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的30%
#define TKEY_CALIBRATE_RATIO40      40    //校验比列系数，用于计算校验阈值，按下阈值DetectInTH的40%
#define TKEY_CALIBRATE_THMAX        10    //校验阈值最大值
#define TKEY_CALIBRATE_THMIN        7     //校验阈值最小值
#define TKEY_CALIBRATE_TIMES        5     //采样次数，达到该次数后保存采样结果
#define TKEY_CALIBRATE_BUFF_MAX     20    //采样结果保存次数

#define TKEY_DETECTING_TIME         10000  //按键长按时间，单位ms，超过该时间未释放就会触发快速自校准

/****************  Bit definition for TKEY_ISR register  **************************/
#define TKEY_ISR_CHNUM_POS                     (8U)
#define TKEY_ISR_CHNUM_MASK                    (BIT11|BIT10|BIT9|BIT8)
#define TKEY_ISR_CHNUM(x)                      (x << TKEY_ISR_CHNUM_POS)
#define TKEY_ISR_BUSY                          (BIT3)
#define TKEY_ISR_TIMEOUT                       (BIT2)
#define TKEY_ISR_WAKEUP                        (BIT1)
#define TKEY_ISR_EOC                           (BIT0)

/****************  Bit definition for TKEY_IER register  **************************/
#define TKEY_IER_TIMEOUTIE                     (BIT2)
#define TKEY_IER_WAKEUPIE                      (BIT1)
#define TKEY_IER_EOCIE                         (BIT0)

/****************  Bit definition for TKEY_CR register  **************************/
#define TKEY_CR_RANDM_POS                      (12U)
#define TKEY_CR_RANDM_MASK                     (BIT13|BIT12)
#define TKEY_CR_RANDM(x)                       (x << TKEY_CR_RANDM_POS)
#define TKEY_CR_CHARGESEL                      (BIT11)
#define TKEY_CR_VKEYSEL_POS                    (9U)
#define TKEY_CR_VKEYSEL_MASK                   (BIT10|BIT9)
#define TKEY_CR_VKEYSEL(x)                     (x << TKEY_CR_VKEYSEL_POS)
#define TKEY_CR_VREFSEL_POS                    (7U)
#define TKEY_CR_VREFSEL_MASK                   (BIT8|BIT7)
#define TKEY_CR_VREFSEL(x)                     (x << TKEY_CR_VREFSEL_POS)
#define TKEY_CR_SLEEP                          (BIT6)
#define TKEY_CR_SPREAD                         (BIT5)
#define TKEY_CR_CONT                           (BIT4)
#define TKEY_CR_SHIELDEN                       (BIT3)
#define TKEY_CR_CREN                           (BIT2)
#define TKEY_CR_START                          (BIT1)
#define TKEY_CR_SCAN                           (BIT0)

/****************  Bit definition for TKEY_SR register  **************************/
#define TKEY_SR_VCOUT2_ORG                      (BIT3)
#define TKEY_SR_VCOUT1_ORG                      (BIT2)
#define TKEY_SR_VCOUT2                          (BIT1)
#define TKEY_SR_VCOUT1                          (BIT0)

/****************  Bit definition for TKEY_SMPR register  **************************/
#define TKEY_SMPR_SWT_POS                       (4U)
#define TKEY_SMPR_SWT_MASK                      (0xFFF0U)
#define TKEY_SMPR_SWT(x)                        (x << TKEY_SMPR_SWT_POS)
#define TKEY_SMPR_CST_POS                       (0U)
#define TKEY_SMPR_CST_MASK                      (0x000FU)
#define TKEY_SMPR_CST(x)                        (x << TKEY_SMPR_CST_POS)

/****************  Bit definition for TKEY_SOFR register  **************************/
#define TKEY_SOFR_SW1H_POS                      (8U)
#define TKEY_SOFR_SW1H_MASK                     (0xFF00U)
#define TKEY_SOFR_SW1H(x)                       (x << TKEY_SOFR_SW1H_POS)
#define TKEY_SOFR_SW1L_POS                      (0U)
#define TKEY_SOFR_SW1L_MASK                     (0x00FFU)
#define TKEY_SOFR_SW1L(x)                       (x << TKEY_SOFR_SW1L_POS)

typedef enum
{
    // Calibration states
    TKEY_STATEID_CALIB              = 0,  /**<  0 - Object is in Calibration */
    // Release states
    TKEY_STATEID_RELEASE            = 1,  /**<  1 - Object is released */
    // Detect states
    TKEY_STATEID_DETECT             = 2, /**<   2 - Object is in Detect */
    //Startuo states
    TKEY_STATEID_STARTUP            = 3, /**<   3 - Object is in Startup */
    // Detecting states
    TKEY_STATEID_DETECTING          = 4, /**<   4 - Object is in Detecting */
  
} TKEY_StateId_enum_T;

typedef enum
{
    TKEY_CRSELECT_DISABLE           = 0,  /**<  0 - Object is in Calibration */
    TKEY_CRSELECT_ENABLE            = 1,  /**<  1 - Object is in Debounce Calibration */
} TKEY_CRSELECT_enum_T;

typedef enum
{
    TKEY_CALIBRAT_IDLE           = 0,   /**<  0 - Not need calibrate */
    TKEY_CALIBRAT_SELF           = 1,   /**<  1 - need self calibrate */
    TKEY_CALIBRAT_QUICK          = 2,   /**<  2 - need quick calibrate */
} TKEY_CALIBRAT_enum_T;

/**
  * @brief  TKEY Configuration Structure definition
  */
typedef struct
{       
    uint8_t            ShieldEn;
    uint16_t           ScanWaitTime;
}TKEY_InitTypeDef;

/**
  * @brief  TKEY Configuration Structure definition
  */
typedef struct
{
    uint16_t                DetectInTH;          /*!< The threshold when the state from release to detect*/ 
    uint16_t                WakeUpTh;            /*!< The threshold when the state from sleep to wakeup*/
}TKEY_BaseParaDef;

typedef struct
{
    uint16_t                CalibratTH;          /*!< The threshold when need to calibration*/
    uint16_t                DetectOutTH;         /*!< The threshold when the state from detect to release*/
    uint16_t                BaseLine;            /*!< The reference data  */
    uint16_t                RawData;             /*!< The current data  */
    int16_t                 Delta;               /*!< The delta value  */
    uint8_t                 DebIn;               /*!< The variate value for debounce*/
    uint8_t                 DebOut;              /*!< The variate value for debounce*/
    TKEY_StateId_enum_T     StateId;             /*!< The state machine ID*/
}TKEY_DataDef;

typedef struct
{
    uint8_t     pos;
    uint16_t    Caldata[TKEY_CALIBRATE_BUFF_MAX];
    uint8_t     ScanTimer;  
    uint32_t    DetectingTimer;       
    uint8_t     CalFlag; 
}TKEY_CalibartData;

typedef struct
{
    TKEY_BaseParaDef    *Tkey_RefPara;
    TKEY_DataDef        *Tkey_Data;        
    TKEY_CalibartData   *Tkey_CalData;
    uint16_t            ChannelId;
}TKEY_ChannelDataDef;

/**
  * @brief  ADC handle Structure definition
  */
typedef struct
{
    const TKEY_ChannelDataDef         *ChannelData;       /*!< The channel data point*/
    TKEY_InitTypeDef                  Init;               /*!< TKEY init parameters */
    uint16_t                          ChannelDetected;    /*!< TKEY detected*/
    uint16_t                          ChannelValue;       /*!< TKEY Value*/
    uint16_t                          ChannelDetectedNum; /*!< TKEY detected channel number*/
    uint8_t                           TotalChannelNum;    /*!< TKEY toatal channel number*/
} TKEY_HandleTypeDef;

#define TKEY_CHANNEL_0                  (0U)       
#define TKEY_CHANNEL_1                  (1U)       
#define TKEY_CHANNEL_2                  (2U)       
#define TKEY_CHANNEL_3                  (3U)       
#define TKEY_CHANNEL_4                  (4U)       
#define TKEY_CHANNEL_5                  (5U)       
#define TKEY_CHANNEL_6                  (6U)       
#define TKEY_CHANNEL_7                  (7U)       
#define TKEY_CHANNEL_8                  (8U)       
#define TKEY_CHANNEL_9                  (9U)       
#define TKEY_CHANNEL_10                 (10U)       
#define TKEY_CHANNEL_11                 (11U)       
#define TKEY_CHANNEL_12                 (12U)       
#define TKEY_CHANNEL_13                 (13U)       
#define TKEY_CHANNEL_14                 (14U)       
#define TKEY_CHANNEL_15                 (15U)  
#define TKEY_CHANNEL_NULL               (0xFFFF)     

#define TKEY_CR_CHARGESEL_OPA            (0U)       
#define TKEY_CR_CHARGESEL_LDO            (1U)   

#define TKEY_CR_VKEYSEL_1V2              (0U)       
#define TKEY_CR_VKEYSEL_2V0              (1U)  
#define TKEY_CR_VKEYSEL_2V5              (2U)  
#define TKEY_CR_VKEYSEL_3V0              (3U)  

#define TKEY_CR_VREFSEL_0V6              (0U)       
#define TKEY_CR_VREFSEL_1V0              (1U)  
#define TKEY_CR_VREFSEL_1V5              (2U)  
#define TKEY_CR_VREFSEL_2V0              (3U)  

#define TKEY_CR_SPREAD_DISABLE           (0U)       
#define TKEY_CR_SPREAD_ENABLE            (1U)   

#define TKEY_CR_CONT_DISABLE             (0U)       
#define TKEY_CR_CONT_ENABLE              (1U)   

#define TKEY_CR_SHIELDEN_DISABLE         (0U)       
#define TKEY_CR_SHIELDEN_ENABLE          (1U)   

#define TKEY_CR_CREN_CX                  (0U)       
#define TKEY_CR_CREN_CR                  (1U)   
      
#define TKEY_CR_SCAN_DISABLE             (0U)   
#define TKEY_CR_SCAN_ENABLE              (1U)   

#define TKEY_SOFR_RANDM_1                (0U)   
#define TKEY_SOFR_RANDM_12               (1U)   
#define TKEY_SOFR_RANDM_123              (2U)   
#define TKEY_SOFR_RANDM_1234             (3U)   


/******************************** ADC Instances *******************************/
#define IS_TKEY_ALL_INSTANCE(INSTANCE)     ((INSTANCE) == TKEY)

#define IS_TKEY_ALL_CHARGESEL(CHARGESEL)   (((CHARGESEL) == TKEY_CR_CHARGESEL_OPA) || \
                                           ((CHARGESEL) == TKEY_CR_CHARGESEL_LDO))
										   
#define IS_TKEY_ALL_VKEYSEL(VKEYSEL)       (((VKEYSEL) == TKEY_CR_VKEYSEL_1V2) || \
                                           ((VKEYSEL) == TKEY_CR_VKEYSEL_2V0) || \
                                           ((VKEYSEL) == TKEY_CR_VKEYSEL_2V5) || \
                                           ((VKEYSEL) == TKEY_CR_VKEYSEL_3V0))

#define IS_TKEY_ALL_VREFSEL(VREFSEL)       (((VREFSEL) == TKEY_CR_VREFSEL_0V6) || \
                                           ((VREFSEL) == TKEY_CR_VREFSEL_1V0) || \
                                           ((VREFSEL) == TKEY_CR_VREFSEL_1V5) || \
                                           ((VREFSEL) == TKEY_CR_VREFSEL_2V0))

#define IS_TKEY_ALL_SPREAD(SPREAD)         (((SPREAD) == TKEY_CR_SPREAD_DISABLE) || \
                                           ((SPREAD) == TKEY_CR_SPREAD_ENABLE))
										   
#define IS_TKEY_ALL_SCANCONT(SCANCONT)     (((SCANCONT) == TKEY_CR_CONT_DISABLE) || \
                                           ((SCANCONT) == TKEY_CR_CONT_ENABLE))

#define IS_TKEY_ALL_SHIELDEN(SHIELDEN)     (((SHIELDEN) == TKEY_CR_SHIELDEN_DISABLE) || \
                                           ((SHIELDEN) == TKEY_CR_SHIELDEN_ENABLE))

#define IS_TKEY_ALL_JITTERRANDM(JITTERRANDM)     (((JITTERRANDM) == TKEY_SOFR_RANDM_1) || \
                                           ((JITTERRANDM) == TKEY_SOFR_RANDM_12) || \
                                           ((JITTERRANDM) == TKEY_SOFR_RANDM_123) || \
                                           ((JITTERRANDM) == TKEY_SOFR_RANDM_1234))
										   
#define IS_TKEY_ALL_SCANWAITTIME(SCANWAITTIME)     ((SCANWAITTIME) <= 0x0FFF)
										   
#define IS_TKEY_ALL_CSDISCHARGETIME(CSDISCHARGETIME)    ((CSDISCHARGETIME) <= 0x0F)
										   
#define IS_TKEY_ALL_SW1(_SW1)              ((_SW1) <= 0xFF)

/***************** TKEY Function definition *************************/
HAL_StatusTypeDef HAL_TKEY_Init(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_Start(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_Stop(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_ReadNr(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_ReadChannelData(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_ReadAllNx(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_Suspend(TKEY_HandleTypeDef* htkey);
HAL_StatusTypeDef HAL_TKEY_Resume(TKEY_HandleTypeDef* htkey);
void HAL_TKEY_DetectProcess(TKEY_HandleTypeDef* htkey);
void HAL_TKEY_Quick_Calibrate_RefData(TKEY_HandleTypeDef* htkey, uint8_t CalTimes);
void HAL_TKEY_ScanProcess(TKEY_HandleTypeDef* htkey);

#endif





