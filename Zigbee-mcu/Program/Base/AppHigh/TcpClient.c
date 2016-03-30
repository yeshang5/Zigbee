#define TCPCLIENT_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "TcpClient.h"

/*全局变量*/
typedef struct{
  uint8_t RxStatus;         //从上位机服务器发送的数据帧包接收情况
  uint8_t RxProgress;       //当前一帧数据的接收进程
  
  uint8_t CMD;              //命令字
  uint8_t LENG;             //帧长度
  uint8_t SEQ;              //帧序列
  uint8_t END_DEV;          //目的设备类型
  uint8_t DEV_ID[8];        //目的设备ID
  uint8_t DATA[Rx_BUFF];    //数据
  uint8_t LRC;              //校验
  
  uint8_t RESP;             //命令状态
  
  uint8_t RxCount;          //计数值
  
}Typedef_Frame;

Typedef_Frame Frame; 


/* ------------------------------------------------------------------------------------------------
 *                                          数据帧接收处理
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 * @fn          TcpClient_Init
 * @brief       数据帧参数初始化
 * @param       无
 * @return      无
 **************************************************************************************************
 */
void TcpClient_Init(void)
{
  Frame.RxStatus   =  FRAME_RX_START;  
  Frame.RxProgress =  FRAME_RX_SOP_START;
  
  Frame.CMD        =  0x00;
  Frame.LENG       =  0x00;
  Frame.SEQ        =  0x00;
  Frame.END_DEV    =  END_DEV_NONE;
  memset(Frame.DEV_ID,0x00,8);
  memset(Frame.DATA,0x00,Rx_BUFF);
  Frame.LRC        =  0x00;
  
  Frame.RESP       =  0x00;
  
  Frame.RxCount    =  0x00;
}





/**************************************************************************************************
 * @fn          TcpClient_Receive
 * @brief       客户端接受服务器发送的TCP数据字节
 * @param       Rx_Data -> 串口接收的数据
 * @return      无
 **************************************************************************************************
 */
void TcpClient_Receive(uint8_t Rx_Data)
{
  if(Frame.RxStatus == FRAME_RX_START)                                          //这个条件暂时不去掉，接收数据的实时性可能会受影响
  {
    TcpClient_Process(Rx_Data);
  }
}

/**************************************************************************************************
 * @fn          TcpClient_Process
 * @brief       接受数据帧解析
 * @param       Rx_Data -> 串口接收的数据
 * @return      无
 **************************************************************************************************
 */
void TcpClient_Process(uint8_t Rx_Data)
{
  /*-----------------------------------------------------------------------------------------------------*/
  /* Command    0      1        2      3       4         5       6             7            8     9      */
  /*-----------------------------------------------------------------------------------------------------*/
  /* Name      SOP    CMD     LENG    SEQ  START_DEV  END_DEV  DEV_ID   Data[0]_Data[n-1]  LRC    EOP    */
  /*-----------------------------------------------------------------------------------------------------*/
  /* Values    0xAA                                                                               0x0E   */
  /*-----------------------------------------------------------------------------------------------------*/
  /* No.Byte   2byte  1byte   1byte  1byte   1byte      1byte   8byte        n-byte       1-byte  1-byte */
  /*-----------------------------------------------------------------------------------------------------*/
  
  
  switch(Frame.RxProgress)
  {
    
    case FRAME_RX_SOP_START:          if(Rx_Data != SOP)
                                      {
                                        break;
                                      }

                                      Frame.RxProgress = FRAME_RX_SOP_CHECK;
                                      break;
                                      
    case FRAME_RX_SOP_CHECK:          if(Rx_Data == SOP)
                                      {
                                        Frame.LRC        = 0x00;
                                        Frame.RxProgress = FRAME_RX_CMD;
                                      }
                                      else
                                      {
                                        Frame.RESP = BASE_RESP_SOP_ERR;         //帧头错误
                                        Frame.RxProgress = FRAME_RX_CMD;        
                                      }
                                      break;
                                      
    //优先知道发送的命令                                 
     case FRAME_RX_CMD:               if(Frame.RESP == BASE_RESP_SOP_ERR)
                                      {
                                        Frame.CMD = Rx_Data;
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        //强行结束接收数据
                                      }

                                      else if(TcpClient_IsCmdExist(Rx_Data))    //查看命令是否存在
                                      {
                                        Frame.CMD = Rx_Data;
                                        Frame.LRC = Rx_Data;
                                        Frame.RxProgress = FRAME_RX_LENG;
                                      }
                                      else
                                      {
                                        Frame.CMD = Rx_Data;              
                                        Frame.RESP = BASE_RESP_CMD_ERR;         //命令不存在
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        
                                      }
                                      break;                                              
                                      
    case FRAME_RX_LENG:               if(Rx_Data >= 11)                         //从SEQ->Data[n-1]的数据长度，大于等于12Byte
                                      {
                                        Frame.LENG = Rx_Data;
                                        Frame.LRC = Frame.LRC ^ Rx_Data;
                                        Frame.RxProgress = FRAME_RX_SEQ;
                                      }
                                      else                          
                                      {
                                        Frame.RESP = BASE_RESP_LENG_ERR;        //长度缺失
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        
                                      }
                                      break;
                                      
    case FRAME_RX_SEQ:                if(Rx_Data == SEq)
                                      {
                                        Frame.SEQ = Rx_Data;
                                        Frame.LRC = Frame.LRC ^ Rx_Data;
                                        Frame.RxProgress = FRAME_RX_START_DEV;
                                      }
                                      else
                                      {
                                        Frame.RESP = BASE_RESP_SEQ_ERR;         //帧序列错误
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        
                                      }
                                      break;
                                                                         
    case FRAME_RX_START_DEV:          if(Rx_Data == START_DEV_PC)
                                      {
                                        Frame.LRC = Frame.LRC ^ Rx_Data;
                                        Frame.RxProgress = FRAME_RX_END_DEV;
                                      }
                                      else
                                      {
                                        Frame.RESP = BASE_START_DEV_ERR;        //起始设备识别错误
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        
                                      } 
                                      break;
                                      
    case FRAME_RX_END_DEV:            if(Rx_Data == END_DEV_BASE)               //命令只下发到基站
                                      {
                                        Frame.END_DEV = END_DEV_BASE; 
                                      }
                                      else if(Rx_Data == END_DEV_DOOR)          //命令下发到门锁
                                      {
                                        Frame.END_DEV = END_DEV_DOOR;
                                      }
                                      else
                                      {
                                        Frame.RESP = BASE_END_DEV_ERR;          //目的设备识别错误
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        
                                        break;
                                      }
                                      
                                      Frame.LRC = Frame.LRC ^ Rx_Data;
                                      Frame.RxCount    = 0;
                                      Frame.RxProgress = FRAME_RX_DEV_ID; 
                                      break;
                                      
    case FRAME_RX_DEV_ID:             Frame.DEV_ID[Frame.RxCount++] = Rx_Data;
                                      Frame.LRC = Frame.LRC ^ Rx_Data;
                                      
                                      if(Frame.RxCount ==  4)                   
                                      {
                                         if(Frame.DEV_ID[3] != BASE_ID4)        //检查BaseId的正确性
                                         {
                                            Frame.RxCount = 0;  
                                            Frame.RESP = BASE_END_DEV_ERR;      //仍然算目的设备识别错误
                                            Frame.RxStatus = FRAME_RX_ERR;
                                            Frame.RxProgress = FRAME_RX_END;    
                                            break;
                                         }
                                      }
      
                                      else if(Frame.RxCount == 8)
                                      {
                                        Frame.RxCount = 0;                      //这里还缺识别门锁ID的检查
                                        Frame.RxProgress = FRAME_RX_DATA;
                                      }
                                      break;     
                                      
                                      
    case FRAME_RX_DATA:               if(Frame.LENG == 11)                      //没有数据，只有CMD
                                      {
                                        Frame.RxProgress = FRAME_RX_LRC;
                                      }
                                       
                                      else
                                      { 
                                        
                                      }
                                      
                                      
    case FRAME_RX_LRC:                if(Frame.LRC == Rx_Data)
                                      {
                                        Frame.RxProgress = FRAME_RX_EOP;
                                      }       
                                      else
                                      {
                                        Frame.RESP = BASE_RESP_LRC_ERR;
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        //强行结束接收数据
                                      }
                                      break;
                                      
    case FRAME_RX_EOP:                if(Rx_Data == EOP)
                                      {
                                        Frame.RxStatus = FRAME_RX_OK;           //一帧数据正确
                                      }
                                      else
                                      {
                                        Frame.RESP = BASE_RESP_EOP_ERR;
                                        Frame.RxStatus = FRAME_RX_ERR;
                                        Frame.RxProgress = FRAME_RX_END;        //强行结束接收数据
                                      }     
                                      break;
 
                                      
    default:                          break;       
    
  }
  
  
  
  
  
}


/**************************************************************************************************
 * @fn          TcpClient_IsCmdExist
 * @brief       匹配命令是否存在
 * @param       RxCmd -> 接收的命令
 * @return      true  -> 存在
                false -> 不存在
 **************************************************************************************************
 */

bool TcpClient_IsCmdExist(uint8_t RxCmd)
{
  bool flag = false;
  
  switch(RxCmd){
    case BASE_CMD_READID:  flag = true;
                           break;
    
    
    default:               break;
  }
  
  return flag;
}







/* ------------------------------------------------------------------------------------------------
 *                                          main函数轮询数据帧
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 * @fn          TcpClient_IsFullFrame
 * @brief       判断是否是完整数据帧
 * @param       无
 * @return      无
 **************************************************************************************************
 */
void TcpClient_IsFullFrame(void)
{
  /*1.正确的数据帧*/
  if(Frame.RxStatus == FRAME_RX_OK)
  { 
    if(Frame.END_DEV == END_DEV_BASE)                                         //如果命令发送到基站
    {
      TcpClient_BaseCMD_Process();
    }
    
    else if(Frame.END_DEV == END_DEV_DOOR)                                    //如果命令发送到门锁
    {
      
    }    
    else
    {
      
    }
    
    TcpClient_Init();                                                           //允许重新接收数据帧
  }
  
  /*2.错误的数据帧*/
  else if(Frame.RxStatus == FRAME_RX_ERR)
  {
    TcpClient_ErrFrame_Process();
    TcpClient_Init();
  }
  
  /*3.数据帧后面如果有部分丢失，下一帧数据可能会补全switch，但是数据帧已经不对了*/
  else 
  {
    
  }
}


/* ------------------------------------------------------------------------------------------------
 *                                          基站接收命令处理[数据帧接收错误]
 * ------------------------------------------------------------------------------------------------
 */
void TcpClient_ErrFrame_Process(void)
{
  
  uint8_t Data = 0X00;
  uint8_t frame[18] = {0x00};
                                     
  TcpClient_createFrame(frame,SEq,Frame.CMD,Frame.RESP,&Data,0);
  Uart0_SendString(frame,18);                                                   //回执命令给PC
}










/* ------------------------------------------------------------------------------------------------
 *                                          基站接收命令处理[数据帧接收正确]
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 * @fn          TcpClient_BaseCMD_Process
 * @brief       PC发送给基站的命令处理
 * @param       无
 * @return      无
 **************************************************************************************************
 */

void TcpClient_BaseCMD_Process(void)
{
  switch(Frame.CMD)
  {
    case BASE_CMD_READID:      TcpClient_BaseCMD_ReadID();  
                               break;
    
    
    
    default:                   break;

  }
  
}



/**************************************************************************************************
 * @fn          TcpClient_BaseCMD_ReadID
 * @brief       基站ID查看
 * @param       无
 * @return      无
 **************************************************************************************************
 */
void TcpClient_BaseCMD_ReadID(void)
{
  uint8_t BaseId[4] = {BASE_ID1,BASE_ID2,BASE_ID3,BASE_ID4};
  uint8_t frame[22] = {0x00};
  
  //Data_BaseID_Read(BaseId);                                                     //读取基站ID
  TcpClient_createFrame(frame,Frame.SEQ,Frame.CMD,BASE_RESP_OK,BaseId,4);
  Uart0_SendString(frame,22);                                                   //回执命令给PC
}















/* ------------------------------------------------------------------------------------------------
 *                                          发送给PC的数据帧创建处理
 * ------------------------------------------------------------------------------------------------
 */

void TcpClient_createFrame(uint8_t* frame,uint8_t SEQ, uint8_t FPB_CMD, uint8_t RESP, uint8_t *data,uint8_t dataLength)
{
  /*------------------------------------------------------------------------------------------------------------*/
  /* Command    0       1        2       3       4        5       6       7          8            8     9       */
  /*------------------------------------------------------------------------------------------------------------*/
  /* Name      SOP    FPB_CMD   LENG    SEQ  START_DEV  END_DEV  DEV_ID  RESP   Data[0]_Data[n-1]  LRC    EOP   */
  /*------------------------------------------------------------------------------------------------------------*/
  /* Values    0xAA                                                                                       0x0E  */
  /*------------------------------------------------------------------------------------------------------------*/
  /* No.Byte   2byte   1byte    1byte   1byte   1byte     1byte   8byte  1byte     n-byte        1-byte  1-byte */
  /*------------------------------------------------------------------------------------------------------------*/
  uint8_t i;
  uint8_t LRC;
  uint8_t BaseId[4] = {BASE_ID1,BASE_ID2,BASE_ID3,BASE_ID4};
  uint8_t TxCount = 0;
 
  
  frame[TxCount++] = SOP;
  frame[TxCount++] = SOP;
  
  frame[TxCount++] = FPB_CMD;
  
  frame[TxCount++] = dataLength + 12;
  frame[TxCount++] = SEQ;
  frame[TxCount++] = START_DEV_BASE;                                            //起始设备为基站，是基站反馈给PC
  frame[TxCount++] = END_DEV_PC;
  
  
  //Data_BaseID_Read(BaseId);
  for(i=0; i<4; i++)
  {
    frame[TxCount++] = BaseId[i];
  }
  
  for(i=0; i<4; i++)                                                            //如果是基站返回
  {
    frame[TxCount++] = 0x00;
  }
  
  frame[TxCount++] = RESP;
  
  if(dataLength != 0x00)
  {
    for(i=0; i<dataLength;i++)
    {
      frame[TxCount++] = data[i];
    }  
  }
  
  LRC = frame[2];
  
  for(i=3; i<TxCount; i++)
  {
    LRC = LRC ^ frame[i];
  }
  
  frame[TxCount++] = LRC;
  
  frame[TxCount++] = EOP;
  
}