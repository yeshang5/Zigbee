#define PCF8563_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "PCF8563.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Applications
 * ------------------------------------------------------------------------------------------------
 */
/**************************************************************************************************
 * @fn          PCF8563_AlarmInit
 * @brief       RTC时钟芯片报警设定，INT低电平有效,中断来了就自动唤醒系统的睡眠状态
 * @param       无    
 * @return      无
 **************************************************************************************************
 */
void PCF8563_Init(void) 
{
//  //uint8_t i;
//  uint8_t VL;                                                     //只有在重新设置时间的时候才判断有没有掉电，时间准不准，软件清0的话只要重新设个时间就把分寄存器的VL位清除了
  PCF8563_Time Time = {0x16,0x01,0x12,0x02,0x14,0x36,0x00};       //默认时间，注意星期是0-6 0是星期天
//  PCF8563_ReceiveByte(PCF8563_WRITE_ADDR,PCF8563_SEC_ADDR,&VL);   //读取VL的值，VL掉电检测标志位，低电压或掉电时被置1
//
  PCF8563_SetTime(Time);
//  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_CON1_ADDR,      0x00); //默认值
//  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_CON2_ADDR,      0x11); //INT脉冲有效，定时器中断有效,报警中断无效
//  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_TIME_ADDR,      0x81); //定时器有效，定时器时钟频率64Hz 
//  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_TIMECOUNT_ADDR, 0x40); //0x40/64 = 1s, 看门狗是1s，所以其实要小于1s,注意0x00是关闭定时报警
 
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_MIN_ALARM_ADDR, 0x80); //AE = 1，相应的报警条件无效
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_HOUR_ALARM_ADDR,0x80);  
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_DAY_ALARM_ADDR, 0x80); 
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_WEEK_ALARM_ADDR,0x80);    
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_CLKOUT_ADDR,    0x00); //CLKOUT输出被禁止并设成高组态
}




/**************************************************************************************************
 * @fn          PCF8563_SetTime
 * @brief       RTC时钟芯片时间设定
 * @param       Time -> 需要设置的时间      
 * @return      无
 **************************************************************************************************
 */
uint8_t PCF8563_SetTime(PCF8563_Time Time)
{
  uint8_t SetFlag = PCF8563_FAIL;
  PCF8563_Time ReadTime;
  
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_SEC_ADDR,   Time.Sec);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_MIN_ADDR,   Time.Min);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_HOUR_ADDR,  Time.Hour);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_DAY_ADDR,   Time.Day);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_WEEK_ADDR,  Time.Week);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_MONTH_ADDR, Time.Month);
  PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_YEAR_ADDR,  Time.Year);
  
  ReadTime = PCF8563_ReadTime();
  
  if((ReadTime.Min == Time.Min)   && (ReadTime.Hour == Time.Hour)   && (ReadTime.Day == Time.Day) &&
     (ReadTime.Week == Time.Week) && (ReadTime.Month == Time.Month) && (ReadTime.Year == Time.Year))
  {
    SetFlag = PCF8563_SUCCESS;
    return SetFlag;
  }
  
  else
  {
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_SEC_ADDR,   Time.Sec);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_MIN_ADDR,   Time.Min);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_HOUR_ADDR,  Time.Hour);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_DAY_ADDR,   Time.Day);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_WEEK_ADDR,  Time.Week);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_MONTH_ADDR, Time.Month);
    PCF8563_SendByte(PCF8563_WRITE_ADDR,PCF8563_YEAR_ADDR,  Time.Year);
    
    ReadTime = PCF8563_ReadTime();
  
    if((ReadTime.Min == Time.Min)   && (ReadTime.Hour == Time.Hour)   && (ReadTime.Day == Time.Day) &&
       (ReadTime.Week == Time.Week) && (ReadTime.Month == Time.Month) && (ReadTime.Year == Time.Year))
    {
      SetFlag = PCF8563_SUCCESS;
    }
    
    else
    {
      SetFlag = PCF8563_FAIL;
    }
    
  }
  
  return SetFlag;
  
}



/**************************************************************************************************
 * @fn          PCF8563_ReadTime
 * @brief       RTC时钟芯片时间设定
 * @param       无      
 * @return      PCF8563_Time -> 读取的时间
 **************************************************************************************************
 */

PCF8563_Time PCF8563_ReadTime(void)
{
  uint8_t time[7] = {0};
  PCF8563_Time Time;
  PCF8563_ReceiveBuff(PCF8563_WRITE_ADDR,PCF8563_SEC_ADDR,time,7);
  
  Time.Sec    = time[0];
  Time.Min    = time[1];
  Time.Hour   = time[2]; 
  Time.Day    = time[3]; 
  Time.Week   = time[4];
  Time.Month  = time[5]; 
  Time.Year   = time[6]; 
  return Time;
}


/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
/**************************************************************************************************
 * @fn          PCF8563_SendByte
 * @brief       RTC时钟芯片发送数据
 * @param       SlaveAddr -> 从机地址
                RegAddr   -> 寄存器地址
                Data      -> 待发送的数据
 * @return      AckFlag   -> 0 有应答 1 无应答 
 **************************************************************************************************
 */
uint8_t PCF8563_SendByte(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t Data)
{
  uint8_t AckFlag;
  
  I2C_Start();                          //启动总线
  AckFlag = I2C_SendByte(SlaveAddr);    //发送从机地址（器件地址）
  if(AckFlag == 1)                      //若没有应答
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(RegAddr);      //发送从机的寄存器地址
  if(AckFlag == 1)                      //若没有应答
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(Data);         //发送数据
  if(AckFlag == 1)                      //若没有应答
  {
    return 0;
  }
  
  I2C_Stop();                           //结束总线
  return 1;
}



/**************************************************************************************************
 * @fn          PCF8563_ReceiveByte
 * @brief       RTC时钟芯片接收数据
 * @param       SlaveAddr -> 从机地址
                RegAddr   -> 寄存器地址
                ch        -> 待接收的数据
 * @return      AckFlag   -> 0 有应答 1 无应答 
 **************************************************************************************************
 */
uint8_t PCF8563_ReceiveByte(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t *ch)
{
  uint8_t AckFlag;
  I2C_Start();       

  AckFlag = I2C_SendByte(SlaveAddr);      //发送从机地址（器件地址）
  if(AckFlag == 1)                        //若没有应答
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(RegAddr);        //发送从机的寄存器地址
  if(AckFlag == 1)                        //若没有应答
  {
    return 0;
  }
  
  I2C_Start();     
  AckFlag = I2C_SendByte(SlaveAddr + 1);  //表明是读数据，注意默认最低位是0，表明写数据，+1变成1
  if(AckFlag == 1)                        
  {
    return 0;
  }
  
  *ch = I2C_ReceiveByte();                //读取数据
  I2C_SendAck(1);                         //最后一个字节 No Acknowledge 所以需要使SDA = 1 
                           
  I2C_Stop();                             //结束总线
  return 1;
}



/**************************************************************************************************
 * @fn          PCF8563_ReceiveBuff
 * @brief       RTC时钟芯片接收数据
 * @param       SlaveAddr -> 从机地址
                RegAddr   -> 寄存器地址
                Buff      -> 待接收的数据
                Len       -> 读取字节数
 * @return      AckFlag   -> 0 有应答 1 无应答 
 **************************************************************************************************
 */
uint8_t PCF8563_ReceiveBuff(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t *Buff,uint8_t Len)
{
  uint8_t i,AckFlag;
  
  I2C_Start();       
  
  AckFlag = I2C_SendByte(SlaveAddr);      //发送从机地址（器件地址）
  if(AckFlag == 1)                        //若没有应答
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(RegAddr);        //发送从机的寄存器地址
  if(AckFlag == 1)                        //若没有应答
  {
    return 0;
  }
  
  I2C_Start();     
  AckFlag = I2C_SendByte(SlaveAddr + 1);  //表明是读数据，注意默认最低位是0，表明写数据，+1变成1
  if(AckFlag == 1)                        //若没有应答
  {
    return 0;
  }
  
  for(i=0; i<Len-1; i++)
  {
    Buff[i] = I2C_ReceiveByte();
    I2C_SendAck(0);  
  }
  
  
  Buff[Len-1] = I2C_ReceiveByte();
  I2C_SendAck(1);
  
  I2C_Stop();                             //结束总线
  return 1;
}









