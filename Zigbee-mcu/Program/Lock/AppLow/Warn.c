#define WARN_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "Warn.h"

/* ------------------------------------------------------------------------------------------------
 *                                          系统提示声
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Warn_System_Start
 * @brief       系统启动提示声
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_System_Start(void)
{
  LED_ON();
  BUZZER_ON();
  T4CC0   = GamutF;     
  Delay_Ms(100);
  LED_OFF();
  BUZZER_OFF();
}

/* ------------------------------------------------------------------------------------------------
 *                                          普通提示声
 * ------------------------------------------------------------------------------------------------
 */
/**************************************
 * @fn          Warn_One_Buzzer
 * @brief       普通提示声一声
 * @param       无
 * @return      无
 **************************************
 */
void Warn_One_Buzzer(void)
{
  LED_ON();
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  LED_OFF();
  BUZZER_OFF();
  Delay_Ms(100);
}

/***************************************
 * @fn          Warn_OneBuzzer_Led
 * @brief       普通提示声一声
 * @param       无
 * @return      无
 ***************************************
 */
void Warn_OneBuzzer_Led(uint8_t Led)
{
  if(Led == LedOn)
  {
    LED_ON();
  }
  
  else
  {
    LED_OFF();
  }
  
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  BUZZER_OFF();
  Delay_Ms(100);
  
}

/***************************************
 * @fn          Warn_Two_Buzzer
 * @brief       普通提示声两声
 * @param       无
 * @return      无
 ***************************************
 */
void Warn_Two_Buzzer(void)
{
  Warn_One_Buzzer();
  Warn_One_Buzzer();
}

/***************************************
 * @fn          Warn_Three_Buzzer
 * @brief       普通提示声三声
 * @param       无
 * @return      无
 ***************************************
 */
void Warn_Three_Buzzer(void)
{
  Warn_One_Buzzer();
  Warn_One_Buzzer();
  Warn_One_Buzzer();
}

/* ------------------------------------------------------------------------------------------------
 *                                          开关门提示声
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Warn_Door_Open
 * @brief       开门提示声
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Door_Open(void)
{
  
  T4CC0   = GamutC; 
  BUZZER_ON();       
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
       
  T4CC0   = GamutDp;    
  BUZZER_ON();   
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
      
  T4CC0   = GamutG;    
  BUZZER_ON();   
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
}


/****************************************
 * @fn          Warn_Door_Close
 * @brief       关门提示声
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Door_Close(void)
{       
  T4CC0   = GamutG;   
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutDp;        
  BUZZER_ON();  
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutC; 
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
}


/****************************************
 * @fn          Warn_Door_Lock
 * @brief       门反锁LED提示
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Door_Lock(void)
{
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
}

/****************************************
 * @fn          Warn_Key_Open
 * @brief       钥匙开门提示
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Key_Open(void)
{
  LED_ON();
  Warn_Door_Open();
  Delay_Ms(1000);
}


/****************************************
 * @fn          Warn_Key_Close
 * @brief       钥匙关门提示
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Key_Close(void)
{
  Warn_Door_Close();
}


/* ------------------------------------------------------------------------------------------------
 *                                          授权状态提示声
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Warn_Card_Full
 * @brief       普通卡列表已满
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Card_Full(void)
{
  LED_OFF();
  T4CC0   = GamutB;   
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutC;        
  BUZZER_ON();  
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Warn_Card_Success
 * @brief       普通卡列表授权删权成功
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Card_Success(void)
{
  LED_OFF();
  BUZZER_ON();
  T4CC0   = GamutC;     
  Delay_Ms(100);
  BUZZER_OFF();
  Delay_Ms(100);
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Warn_Card_Fail
 * @brief       普通卡列表授权删权失败
 * @param       无
 * @return      无
 ****************************************
 */
void Warn_Card_Fail(void)
{
  Warn_One_Buzzer();
  Warn_One_Buzzer();
}



/**************************************************************************************************
 * @fn          Warn_Door_Init
 * @brief       门锁初始化提示
 * @param       无
 * @return      无
 **************************************************************************************************
 */
void Warn_Door_Init(void)
{
  T4CC0   = GamutB;   
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutG;        
  BUZZER_ON();  
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutE; 
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutA;   
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutF;        
  BUZZER_ON();  
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutDp; 
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
}