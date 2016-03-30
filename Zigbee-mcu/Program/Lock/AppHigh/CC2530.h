#ifndef CC2530_H_
#define CC2530_H_


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "Driver.h"
#include "Device.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */
/*睡眠类型*/
#define GeneralSleep 1
#define LastSleep    2



/*中断类型*/
#define AllDisable   0
#define TimerEnable  1
#define KeyEnable    2
#define DoorEnable   3
#define RfEnable     4


/* ------------------------------------------------------------------------------------------------
 *                                          Prototype
 * ------------------------------------------------------------------------------------------------
 */
void CC2530_Init(void);
void CC2530_NoUseIO_Init(void);
void CC2530_Sleep_Config(uint8_t Type);
void CC2530_UnSleep_Config(void);
void CC2530_Int(uint8_t EnableFlag);


void CC2530_Sleep(void);
#endif