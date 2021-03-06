#ifndef CARD_H_
#define CARD_H_


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "AppLow.h"
#include "Driver.h"
#include "Device.h"  
#include <string.h>

//配置头文件
#include "Congfig.h"
/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */

/********************M1卡区块************************/

/*62块放置卡号基本信息*/
#define BlockSize            16       //一个块16个字节
#define CARD_INFORMATION     62       //M1卡第62块，放入卡片的基本信息

/******************卡号基本信息*********************/
/*M1卡权限类型-CardType(2byte)*/
#define Authorization     0x01        //授权卡
#define UnAuthorizataion  0x02        //删权卡
//#define NullCard          0x03      //空卡
#define TotalCard         0x04        //总卡
#define FloorCard         0x05        //楼层卡
//#define OrdinaryCard      0x10      //普通卡


/******************M1卡操作类型*********************/
#define READ_ID       1               //读卡号ID
#define WRITE_BLOCK   2               //写入区块数据
#define READ_BLOCK    3               //读取区块数据



/* ------------------------------------------------------------------------------------------------
 *                                          variable
 * ------------------------------------------------------------------------------------------------
 */




/* ------------------------------------------------------------------------------------------------
 *                                          Applications
 * ------------------------------------------------------------------------------------------------
 */
void Card_Init(void);
uint8_t Card_Authorization(uint8_t CardOperType);

/* ------------------------------------------------------------------------------------------------
 *                                          Middle
 * ------------------------------------------------------------------------------------------------
 */
uint8_t Card_ReadID(uint8_t BlockAddr, uint8_t *CardId);
uint8_t Card_WriteBlock(uint8_t BlockAddr, uint8_t *BlockData);
uint8_t Card_ReadBlock(uint8_t BlockAddr, uint8_t *BlockData);

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
uint8_t Card_Process(uint8_t *CardId, uint8_t BlockAddr, uint8_t *BlockData, uint8_t ProcessType);


#endif