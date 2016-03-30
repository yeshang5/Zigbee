#define DATA_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "Data.h"

/***************************数据信息初始化*************************************/

/*******************************************
 * @fn          Data_BaseID_Init
 * @brief       基站ID信息初始化
 * @param       无
 * @return      无
 *******************************************
 */
void Data_BaseID_Init(void)
{
  uint8_t BaseId[4] = {0x00};
  AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR,BASE_ID1);
  AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+1,BASE_ID2);
  AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+2,BASE_ID3);
  AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+3,BASE_ID4);
  
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR,BaseId,4);
  
  if((BaseId[0] != BASE_ID1) || (BaseId[1] != BASE_ID2) ||
     (BaseId[2] != BASE_ID3) || (BaseId[3] != BASE_ID4) )
  {
    AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR,BASE_ID1);
    AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+1,BASE_ID2);
    AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+2,BASE_ID3);
    AT24C256_WriteByte(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR+3,BASE_ID4);
  }
}




/***************************门锁ID数据操作*************************************/

/*******************************************
 * @fn          Data_BaseID_Read
 * @brief       基站ID信息读取
 * @param       BaseId -> 基站ID
 * @return      无
 *******************************************
 */
void Data_BaseID_Read(uint8_t *BaseId)
{
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,BASEID_BASE_ADDR,BaseId,4);
}














/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

///*******************************************
// * @fn          Data_Storage
// * @brief       存储判断
// * @param       WriteId   -> 需要存储的卡号
//                WriteAddr -> 存储的位置
//
// * @return      Result
//                 -> AT24C256_OK     存储成功
//                 -> AT24C256_ERR    存储失败
// *******************************************
// */
//uint8_t Data_Storage(uint8_t *WriteId, uint16_t WriteAddr)
//{
//  uint8_t ReadId[4] = {0x00};
//  uint8_t DataResult = DATA_OK;
//  
//  AT24C256_WriteBuff(AT24C256_WRITE_ADDR,WriteAddr,WriteId,4);
//    
//  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,WriteAddr,ReadId,4);
//  if(strncmp((const char*)ReadId,(const char*)WriteId,4) != 0)                  //判断存储及是否成功
//  {
//    AT24C256_WriteBuff(AT24C256_WRITE_ADDR,WriteAddr,WriteId,4);
//    if(strncmp((const char*)ReadId,(const char*)WriteId,4) != 0)                //再次判断存储及是否成功
//    {
//      DataResult = DATA_ERR;                                                    //存储失败
//    }
//  } 
//
//  return DataResult;
//}