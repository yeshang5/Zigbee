#define MAIN_C_
#include "main.h"

/* ------------------------------------------------------------------------------------------------
 *                                          主函数
 * ------------------------------------------------------------------------------------------------
 */


//1.需要注意的是一个时刻只能让一个中断工作，尤其是RF发送和接收时最容易跑飞程序
//2.延时不应该用阻塞性延时
//3.同一个时刻要防止两个一起发送！且发送时一定要关闭所有的中断来防止接收。
//4.不能同时刷卡和钥匙开门，会复位程序！


void main(void)
{
    /*1.局部变量初始化*/
    uint8_t RunMode         = ReadCardAction;                                   //当前运行模式
    uint8_t Status          = MFRC522_ERR;                                      //读卡操作标志
    uint8_t DoorId[4]       = {0x00};                                           //门锁ID
    uint8_t BlockData[16]   = {0x00};                                           //M1卡扇区数据 
    uint16_t AdcValue       = 0x00;                                             //P0_0口AD值
    uint8_t Adc[2]          = {0x00};                                           //发送给射频模块的AD值高低八位
    uint16_t k1,k2;

    /*2.全局变量初始化*/
    IsDoorClose           = CLOSE_INIT;                                         //门状态未知，平常是不检测的，这时候也不会跳进中断
    IsKeyOpen             = OPEN_NO;
    
    
    /*3.总中断关闭、系统时钟初始化*/
    EA = 0;                                                                     //关掉总中断
    Sysclk_Init();                                                              //系统时钟频率设置为32Mhz
    
    /*4.CC2530端口初始化*/
    CC2530_Init();
    
    /*5.Driver初始化*/
    Spi_Init();                                                                 //SPI初始化
    Timer1_Init();                                                              //定时器1初始化
    Timer3_Init();                                                              //定时器3初始化
    Timer4_Init();                                                              //蜂鸣器PWM初始化
    Delay_Init();                                                               //定时器中断计时变量初始化
    Key_Int_Init();                                                             //机械门锁中断初始化
    Rf_Init();                                                                  //射频模块初始化,接收发送禁止
    Sleep_Init();                                                               //系统定时睡眠初始化
    //Wdt_Init();                                                                 //开门狗初始化,调试的时候会受到影响
    
    /*6.Device初始化*/     
    MFRC522_Init();                                                             //MFRC522读卡模块初始化
    
    /*7.App初始化*/
   
    
    /*8.开启总中断,关闭当下中断*/
    EA = 1;                                                                     //开启总中断
    CC2530_Int(AllDisable);                                                     //关闭当下所有中断,除了睡眠中断

    
    /*9.只初始化1次，最终版本需要屏蔽*/
    PCF8563_Init();                                                             //RTC时钟芯片初始化,这里会有一次时间设定
    //Card_Init();                                                              //特权卡发放,调试程序用，最终会写一个读卡器程序
    //Data_CommonCard_Init();
    Data_DoorID_Init();                                                         //门锁ID存储
    
    
    /*10.程序启动*/
    Warn_System_Start();                                                         //系统启动声音
    Door_Close(LedOff);
    
    while(1)
    {     
      switch(RunMode)
      {
        
        /*0.电池电压检测*/
        case BatteryDetection:  CC2530_Int(KeyEnable);                          //P0.1中断使能
                                LED_ON();
                                Delay_Ms(10);
                                AdcValue = Adc_GetVoltage();
                                Adc[0]   = AdcValue & 0xFF;                     //这里千万不能做射频工作，其他射频模板工作时这里会复位
                                Adc[1]   = (AdcValue >> 8) & 0xFF;
                                Rf_Send(Adc,2);
                                //Delay_Ms(1000);
                                LED_OFF();  
                                
                                //Wdt_FeetDog();                                  //主程序喂狗
                                
                                
                                if((AdcValue >= 3500) && (AdcValue <= 3900))
                                { 
                                  LED_ON();
                                  Delay_Ms(1000);
                                  LED_OFF();                                    //电池低电压闪烁报警 
                                  Status = Card_ReadBlock(CARD_INFORMATION,BlockData);  //如果此时有读卡，则继续优先读卡任务
                                  if(Status == MFRC522_OK)
                                  {
                                    RunMode = ReadCardAction;                   //执行读卡任务
                                    break;
                                  }
                                  
                                  if(IsKeyOpen == OPEN_YES)
                                  {
                                    RunMode = KeyAction;                        //执行机械开门任务 
                                    break;         
                                  }
                                  
                                  RunMode = BatteryDetection;                   //如果已经报警，电池检测任务优先，无线任务不工作
                                  CC2530_Sleep();                               //睡眠1S  
                                  break;
                                }
                                      
                                else if(AdcValue < 3500)
                                {
                                  LED_ON();
                                  CC2530_Sleep_Config(LastSleep);               //灯亮，刷卡开门无效果，钥匙开门重启
                                  //KeyDoor_Int_Disable();
                                  Sleep_SetMode(SLEEP_PM3);                     //钥匙开门还是能够唤醒的          
                                }
                                
                                CC2530_Int(AllDisable);                         //关闭当下所有中断,除了睡眠中断
                                RunMode = ReadCardAction;                       //检测完电池就继续执行读卡
                                break;
        
        
        /*1.刷卡授权和开门*/             
        case ReadCardAction:    Status = Card_ReadBlock(CARD_INFORMATION,BlockData); 
                                if(Status == MFRC522_OK)
                                {
                                  /*1.1.如果门未反锁*/
                                  if(LOCK == LOCK_NO)
                                  {
                                    /*1.2.黑名单操作无效(未实现)*/
                                    
                                    /*1.3.特权卡操作*/
                                    Data_DoorID_Read(DoorId);                   //门锁ID读取
                                    if((BlockData[2] == DoorId[0]) && (BlockData[3] == DoorId[1]))    //如果是此公司此撞楼的特权卡
                                    {
                                      /*1.3.1.授权卡和删权卡*/
                                      if(((BlockData[0] == Authorization)    && (BlockData[1] == Authorization)) ||
                                         ((BlockData[0] == UnAuthorizataion) && (BlockData[1] == UnAuthorizataion)))     
                                      {
                                         Door_Open(LedOn);
                                         LED_OFF();
                                         Delay_Ms(1000);
                                         Warn_OneBuzzer_Led(LedOn);
                                         Card_Authorization(BlockData[0]);      //卡号授权或删权处理
                                         Door_Close(LedOn);  
                                      }
                                      
                                      /*1.3.2.总卡*/
                                      else if((BlockData[0] == TotalCard) && (BlockData[1] == TotalCard))   
                                      {
                                        Door_Open_Close();
                                      }
                                      
                                      /*1.3.3.楼层卡*/
                    
                                    }
                                    
                                    /*1.4.普通卡操作*/
                                    else
                                    {
                                      if(Data_CommonCard_Confirm(BlockData+12)) 
                                      {
                                        Door_Open_Close();   
                                      }
                                      
                                      else
                                      {
                                        Warn_Card_Fail();                       //无权限卡警告
                                      }
                                      
                                    }
                                  }
                                  
                                  /*1.5.门反锁*/
                                  else
                                  {
                                    Warn_Door_Lock();                            //门反锁时不要发出声音
                                  }
     
                                  /*1.6.刷卡检测电池*/
                                  RunMode = BatteryDetection;                  
                                  break;
                                }
                                
                                RunMode = KeyAction;                            //如果未开门继续执行机械开门任务 
                                break;          
        
                                
        /*2.机械开门*/
        case KeyAction:         CC2530_Int(KeyEnable);                          //P0.1中断使能
                                   
                                if(IsKeyOpen == OPEN_YES)
                                {
                                  Warn_Key_Open();                              //相当于一直在消抖了，滤掉了脉冲  
                                  
                                  for (k1=0;k1<40000;k1++)                 
                                  { 
                                     for (k2=0;k2<200;k2++)
                                     {
                                       ;;
                                     }
                                     if(KEY == TURNOFF)                         //变成高电平的状态才允许再次中断
                                     {
                                       break;
                                     }
                                  }
                                  IsKeyOpen = OPEN_NO;
                                  Warn_Key_Close();
                                  
                                  RunMode = BatteryDetection;                   //钥匙开门检测电池        
                                  break;
                                }
                                  
                                CC2530_Int(AllDisable);                         //关闭当下所有中断,除了睡眠中断
                                RunMode = SysIntoSleep;
                                break;
       
                              
        /*5.系统进入睡眠*/
        case SysIntoSleep:      //Wdt_FeetDog();                                  //主程序喂狗
                                RunMode = ReadCardAction;               
                                CC2530_Sleep();                                 //睡眠1S          
                                break;                             
                                
                                
                                    
        default:                RunMode = ReadCardAction;
                                break;
      }    
    }
}  



/* ------------------------------------------------------------------------------------------------
 *                                          中断函数
 * ------------------------------------------------------------------------------------------------
 */

/************************************************
 *function :  P0_ISR
 *describe :  P0_1用于检测钥匙开门，P0_4用于检测开门瞬间或关门瞬间
 *input    :  无
 *return   :  无
 ***********************************************/
#pragma vector = P0INT_VECTOR
__interrupt void P0_ISR(void)
{
 EA   =  0;

 /*1.钥匙开门*/
 if(P0IFG & 0x02)                 //P0.1发生中断
 { 
   if((KEY == TURNON) && (IsKeyOpen == OPEN_NO))
   {
     CC2530_UnSleep_Config();     //解除睡眠时端口设置
     Sleep_SetMode(SLEEP_OFF);    //进入正常工作模式
     IsKeyOpen = OPEN_YES;        //钥匙开门
   }
 }
 
 /*2.开门之后判断关门*/
 else if(P0IFG & 0x10)            //P0.4发生中断
 {
   if((DOOR == PUSHIN) && (IsDoorClose == CLOSE_NO))
   {
     IsDoorClose = CLOSE_YES;     //开门之后关门了
   }
 }
 
 P0IFG = 0;                       //清除中断标志
 P0IF  = 0;                        //无中断未决   
 EA    = 1;
}



/************************************************
 * @fn          ST_ISR
 * @brief       睡眠定时器中断
 * @param       无
 * @return      无
 ************************************************
 */
#pragma vector = ST_VECTOR
__interrupt void ST_ISR(void)
{
  STIF = 0x00;                  //清除中断标志
  CC2530_UnSleep_Config();      //解除睡眠时端口设置
  Sleep_SetMode(SLEEP_OFF);     //进入正常工作模式
}


/************************************************
 *function :  Rf_ISR()
 *describe :  RF射频接收中断函数
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