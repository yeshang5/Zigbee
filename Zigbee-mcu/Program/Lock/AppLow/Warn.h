#ifndef WARN_H_
#define WARN_H_


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "Driver.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */
/*音阶和频率*/
#define     GamutC         0x3c         
#define     GamutD         0x35
#define     GamutDp        0x32
#define     GamutE         0x30
#define     GamutF         0x2d
#define     GamutG         0x28
#define     GamutA         0x23
#define     GamutB         0x20


/*LED灯亮灭标志*/
#define LedOn   1
#define LedOff  2

/* ------------------------------------------------------------------------------------------------
 *                                          Prototype
 * ------------------------------------------------------------------------------------------------
 */
void Warn_System_Start(void);

void Warn_One_Buzzer(void);
void Warn_OneBuzzer_Led(uint8_t Led);
void Warn_Two_Buzzer(void);
void Warn_Three_Buzzer(void);

void Warn_Door_Open(void);
void Warn_Door_Close(void);
void Warn_Door_Lock(void);
void Warn_Key_Open(void);
void Warn_Key_Close(void);

void Warn_Card_Full(void);
void Warn_Card_Success(void);
void Warn_Card_Fail(void);
void Warn_Door_Init(void);
#endif