#define DELAY_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "Delay.h"


/* ------------------------------------------------------------------------------------------------
 *                                          Local Variable
 * ------------------------------------------------------------------------------------------------
 */
uint8_t   T1_100MS_Count;
uint8_t   T1_1S_Count;
uint8_t   T1_3S_Count;
uint8_t   T3_10MS_Count;






/**************************************************************************************************
 * @fn          Delay_Init
 * @brief       中断变量初始化
 * @param       无
 * @return      无
 **************************************************************************************************
 */
void Delay_Init(void)
{
  //T1
  T1_100MS_Count  = 0;
  T1_1S_Count     = 0;
  T1_3S_Count     = 0;
  
  T1_100MS_Flag   = TIMEEND;
  T1_1S_Flag      = TIMEEND;
  T1_3S_Flag      = TIMEEND;
  
  
  //T3
  T3_10MS_Count   = 0; 
  T3_10MS_Flag    = TIMEEND;          //计数标志
}


/**************************************************************************************************
 * @fn          Delay_Ms
 * @brief       延时
 * @param       n -> 延时时间 Ms
 * @return      无
 **************************************************************************************************
 */
void Delay_Ms(uint16_t n)
{
  
  
  if(n == 10)
  {
    T3_10MS_Flag = TIMESTART;             //开始计时
    while(!(T3_10MS_Flag == TIMEOUT));    //等待超时
    T3_10MS_Flag = TIMEEND;
  }
  
  
  else if(n == 100)
  {
    T1_100MS_Flag = TIMESTART;            //开始计时
    while(!(T1_100MS_Flag == TIMEOUT));   //等待超时
    T1_100MS_Flag = TIMEEND;              //停止计时
  }
  
  else if(n == 1000)
  {
    T1_1S_Flag = TIMESTART;               //开始计时
    while(!(T1_1S_Flag == TIMEOUT));      //等待超时
    T1_1S_Flag = TIMEEND;                 //停止计时
  }
}


/* ------------------------------------------------------------------------------------------------
 *                                          定时中断
 * ------------------------------------------------------------------------------------------------
 */

/************************************************
 *function :  T1_ISR
 *describe :  定时器1中断，每隔50ms触发一次，需要注意的是同时只有一个延时在工作
 *input    :  无
 *return   :  无
 ***********************************************/
#pragma vector = T1_VECTOR
__interrupt void T1_ISR(void)
{
  
  
  
  /*1.开始计数*/
  if(T1_100MS_Flag == TIMESTART)
  {
    T1_100MS_Count ++;
  }
  
  else if(T1_1S_Flag == TIMESTART)
  {
    //Wdt_FeetDog();                 //中断里喂狗！可能需要注意
    T1_1S_Count ++;
  }
  
  
  if(T1_3S_Flag == TIMESTART)      //这里不要用else if 因为3s是在while里的，如果有前面的延时触发，3s是会一直延长的
  {
    //Wdt_FeetDog();                 //中断里喂狗！可能需要注意 
    T1_3S_Count ++;
  }
 
  /*2.计数结束*/
  if(T1_100MS_Count == T1_100MS)
  {
    //Wdt_FeetDog();                //中断里喂狗！可能需要注意
    T1_100MS_Flag = TIMEOUT;
    T1_100MS_Count = 0;
  }
  
  
  else if(T1_1S_Count == T1_1S)
  {
    T1_1S_Flag = TIMEOUT;
    T1_1S_Count = 0;
  }
  
  if(T1_3S_Count == T1_3S)
  {
    T1_3S_Flag = TIMEOUT;
    T1_3S_Count = 0;
  }
  
  
  /*3.计数清除*/
  if((T1_3S_Flag == TIMEEND) && (T1_3S_Count != 0))
  {
    T1_3S_Count = 0;
  }
  
  T1IF = 0;   //清除中断标志
}


/************************************************
 *function :  T3_ISR
 *describe :  定时器3中断，用于小延时
 *input    :  无
 *return   :  无
 ***********************************************/
#pragma vector = T3_VECTOR
__interrupt void T3_ISR(void)
{
  T3IF = 0;   //清除中断标志
  
  /*1.开始计数*/
  if(T3_10MS_Flag == TIMESTART)
  {
    T3_10MS_Count ++;
  }
  
  /*2.计数结束*/
  if(T3_10MS_Count == T3_10MS)    //定时10MS
  {
    T3_10MS_Flag  = TIMEOUT;
    T3_10MS_Count = 0;
  }  
  
}