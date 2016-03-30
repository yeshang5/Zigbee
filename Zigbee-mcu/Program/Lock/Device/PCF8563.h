#ifndef PCF8563_H_
#define PCF8563_H_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "I2c.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */
/*I2C总线从机地址*/
#define PCF8563_WRITE_ADDR      0xA2   //写器件地址
#define PCF8563_READ_ADDR       0xA3   //读器件地址，其实就是写地址+1嘛

/*PCF8563寄存器地址*/
#define PCF8563_CON1_ADDR       0x00   //状态寄存器1
#define PCF8563_CON2_ADDR       0x01   //状态寄存器2
#define PCF8563_SEC_ADDR        0x02   //秒
#define PCF8563_MIN_ADDR        0x03   //分
#define PCF8563_HOUR_ADDR       0x04   //时
#define PCF8563_DAY_ADDR        0x05   //日
#define PCF8563_WEEK_ADDR       0x06   //周
#define PCF8563_MONTH_ADDR      0x07   //月
#define PCF8563_YEAR_ADDR       0x08   //年
#define PCF8563_MIN_ALARM_ADDR  0x09   //分报警
#define PCF8563_HOUR_ALARM_ADDR 0x0A   //时报警
#define PCF8563_DAY_ALARM_ADDR  0x0B
#define PCF8563_WEEK_ALARM_ADDR 0x0C
#define PCF8563_CLKOUT_ADDR     0x0D   //CLK频率寄存器
#define PCF8563_TIME_ADDR       0x0E   //倒计数定时器寄存器
#define PCF8563_TIMECOUNT_ADDR  0x0F   //定时器倒计数计数值

/*时间设定状态标识*/
#define PCF8563_FAIL      1
#define PCF8563_SUCCESS   2


/* ------------------------------------------------------------------------------------------------
 *                                          variable
 * ------------------------------------------------------------------------------------------------
 */
typedef struct 
{  
  uint8_t Year;
  uint8_t Month;
  uint8_t Day;
  uint8_t Week;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
}PCF8563_Time;


/* ------------------------------------------------------------------------------------------------
 *                                          Applications
 * ------------------------------------------------------------------------------------------------
 */
void PCF8563_Init(void);
uint8_t PCF8563_SetTime(PCF8563_Time Time);
PCF8563_Time PCF8563_ReadTime(void);

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
uint8_t PCF8563_SendByte(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t Data);
uint8_t PCF8563_ReceiveByte(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t *ch);
uint8_t PCF8563_ReceiveBuff(uint8_t SlaveAddr,uint8_t RegAddr,uint8_t *Buff,uint8_t Len);




















#endif