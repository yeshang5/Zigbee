#define MAIN_C_

#include "main.h"


/* ------------------------------------------------------------------------------------------------
 *                                          全局变量
 * ------------------------------------------------------------------------------------------------
 */




/* ------------------------------------------------------------------------------------------------
 *                                          主函数
 * ------------------------------------------------------------------------------------------------
 */
void main(void)
{
    /*1.局部变量*/
    uint8_t RunMode = UartTask;
  
      
  
    /*3.总中断关闭、系统时钟初始化*/
    EA = 0;                                                                     //关掉总中断
    Sysclk_Init();                                                              //系统时钟频率设置为32Mhz
    
    /*4.CC2530端口初始化*/
    Led_Init();                                                                 //P0.4、P1.0、P1.1 LED初始化
    I2C_Init();                                                                 //P1.2、P1.3 I2C端口设置
    Iport_Init();                                                               //
    
    
    /*5.Driver初始化*/
    Timer1_Init();                                                              //定时器1初始化
    Timer3_Init();                                                              //定时器3初始化
    Rf_Init();                                                                  //射频模块初始化,接收发送禁止   
    Uart0_Init();                                                               //串口
    
    /*6.Device初始化*/ 
    
    /*7.App初始化*/
    TcpClient_Init();                                                           //TCP接收数据参数初始化
    
    
    /*8.开启总中断*/
    EA = 1;                                                                     //开启总中断   
    CC2530_Int(AllDisable);                                                     //关闭当下所有中断
    CC2530_Int(UartRxEnable);                                                   //打开串口中断
    
    /*9.只初始化1次，最终版本需要屏蔽*/
    Data_BaseID_Init();
    
    /*10.程序启动*/
    //Uart0_SendByte(0x11);
    
    while(1)
    {
      
      switch(RunMode)
      {
        case UartTask:  TcpClient_IsFullFrame();        
                        RunMode ++;
                        break;
        
        case RfTask:    
          
          
                        RunMode ++;
                        break;
        
        default:        RunMode = UartTask;  
                        break;        
      
      }
    }
   
}




/* ------------------------------------------------------------------------------------------------
 *                                          中断函数
 * ------------------------------------------------------------------------------------------------
 */
/************************************************
 *function :  UART0_ISR
 *describe :  串口中断接收函数 
 *input    :  None
 *return   :  None
 ***********************************************/


#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
  uint8_t Uart0_Rx;
  URX0IF    = 0;  
  Uart0_Rx  = U0DBUF;    
  TcpClient_Receive(Uart0_Rx);
}


/************************************************
 *function :  Rf_ISR()
 *describe :  
 *input    :  None
 *return   :  None
 ***********************************************/
#pragma vector=RF_VECTOR  
__interrupt void rf_isr(void)  
{
  uint8_t Rx[1024];
  EA   =  0;
  
  //Wdt_Feet();
  
  if(RFIRQF0&(1<<6))                    //接收到一个完整的数据帧           
  {
    Rf_Receive(Rx);                     //接收数据处理函数
    S1CON     = 0;                      //清除RF中断标志
    RFIRQF0  &= ~(1<<6);                //清除RF接收完成中断标志
    
  }
  
  EA   =  1;
}