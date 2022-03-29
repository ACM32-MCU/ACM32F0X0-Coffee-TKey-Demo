/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_fifo.c
@功能描述:	FIFO模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_fifo.h"
#include "String.h"


/*****************************************************************
@功能	FIFO创建，最大64KB。本模块所有函数不可重入，除非加禁止中断
@参数	pfifo-FIFO结构指针
@参数	buff-FIFO的数据缓存
@参数	len-FIFO的长度
@返回	无
******************************************************************/
void FIFO_Create(FIFO_TypeDef *pfifo, uint8_t *buff, uint16_t len)
{
    pfifo->tt_len = len;
    pfifo->size = 0;
    pfifo->in = pfifo->out = 0;
    pfifo->buff = buff;
}


/*****************************************************************
@功能	用一段数据初始化FIFO。必须要先创建FIFO，才能初始化。
@参数	pfifo-FIFO结构指针
@参数	pdata[in]-数据地址
@参数	len-数据长度
@返回	AC_OK,AC_FAIL
******************************************************************/
AC_StatusTypeDef FIFO_InitData(FIFO_TypeDef *pfifo, const char *pdata, unsigned int len)
{
    if(pfifo == NULL)
    {
        return AC_FAIL;
    }    
    if(pdata == NULL)
    {
        FIFO_Clear(pfifo);
        return AC_FAIL;
    }
    
    if(len>pfifo->tt_len)
    {
        return AC_FAIL;
    } 
    
    pfifo->in = len;
    pfifo->out = 0;
    memcpy((void *)pfifo->buff, pdata, len);
    pfifo->size = len;
    return AC_OK;
}

/*****************************************************************
@功能	清空FIFO 
@参数	pfifo-FIFO结构指针
@返回	无
******************************************************************/
void FIFO_Clear(FIFO_TypeDef *pfifo)
{   
    pfifo->size = 0;
    pfifo->in = pfifo->out = 0;
}

/*****************************************************************
@功能	FIFO入栈。
@参数	pfifo-FIFO结构指针
@参数	data-字节数据
@返回	AC_OK,AC_FAIL
******************************************************************/
AC_StatusTypeDef FIFO_In(FIFO_TypeDef *pfifo, uint8_t data)
{   
    if(pfifo->size >= pfifo->tt_len)
    {
        return AC_FAIL;
    }
    
    *(pfifo->buff + pfifo->in) = data; 
	pfifo->in++;
    
    if(pfifo->in >= pfifo->tt_len)
    {
        pfifo->in = 0;
    }
    pfifo->size++;
    return AC_OK;
}


/*****************************************************************
@功能	FIFO多字节入栈。函数不可重入，除非加禁止中断
@参数	pfifo-FIFO结构指针
@参数	pdata-数据地址
@参数	len-数据长度
@返回	实际入栈的数据长度
******************************************************************/
uint16_t FIFO_InMulti(FIFO_TypeDef *pfifo, uint8_t *pdata, uint16_t len)
{
    uint16_t i=0;
    while(len--)
    {
        if(pfifo->size >= pfifo->tt_len)
        {
            //printf("<%d,%d>",pfifo->size,pfifo->tt_len-1);
            return i;
        }
        
        *(pfifo->buff + pfifo->in) = *pdata++; 
        pfifo->in++;
        if(pfifo->in >= pfifo->tt_len)
        {
            pfifo->in = 0;
        }
        pfifo->size++;
        i++;
    }
    return i;
    
}


/*****************************************************************
@功能	FIFO出栈。函数不可重入，除非加禁止中断
@参数	pfifo-FIFO结构指针
@参数	pdata-字节数据地址
@返回	AC_OK,AC_FAIL
******************************************************************/
AC_StatusTypeDef FIFO_Out(FIFO_TypeDef *pfifo, uint8_t *pdata)
{
    if(!(pfifo->size))
        return AC_FAIL;
    
    *pdata = *(pfifo->buff + pfifo->out);
	pfifo->out++;    
    if(pfifo->out >= pfifo->tt_len)
    {
        pfifo->out = 0;
    }
    
    pfifo->size--;
    return AC_OK;
}

/*****************************************************************
@功能	FIFO多字节出栈。函数不可重入，除非加禁止中断
@参数	pfifo-FIFO结构指针
@参数	pdata-数据地址
@参数	len-数据长度
@返回	实际出栈的数据长度
******************************************************************/
uint16_t FIFO_OutMulti(FIFO_TypeDef *pfifo, uint8_t *pdata, uint16_t len)
{
    uint16_t i=0;
    while(len--)
    {
        if(!(pfifo->size))
            return i;
        
        *pdata++ = *(pfifo->buff + pfifo->out);  
        pfifo->out++;  
        if(pfifo->out >= pfifo->tt_len)
        {
            pfifo->out = 0;
        }
        pfifo->size--;
        i++;
    }
    return i;   
}

/*****************************************************************
@功能	查询FIFO数据长度
@参数	pfifo-FIFO结构指针
@返回	FIFO长度
******************************************************************/
uint16_t FIFO_Query(FIFO_TypeDef *pfifo)
{
    return pfifo->size;
}
