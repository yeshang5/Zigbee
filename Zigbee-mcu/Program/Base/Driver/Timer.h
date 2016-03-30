#ifndef TIMER_H
#define TIMER_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include<ioCC2530.h>
#include "Wdt.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */

#ifndef TIMER_C_
  #define TIMER_C_Extern extern
#else
  #define TIMER_C_Extern
#endif


/*时间状态标记*/
#define TIMEINIT    0     //计时初始化
#define TIMEEND     1     //停止计时
#define TIMESTART   2     //开始计时
#define TIMEOUT     3     //时间到

/*时间计数值，计数时有优先级的，当3s开始计数的同时1s计数时必须先让1s计数完才能计数3s*/
/*T1定时器*/
#define T1_100MS    2     //100ms
#define T1_1S       20    //1s
#define T1_3S       60    //3s

/*T3定时器*/
#define T3_10MS     5     //10ms


/* ------------------------------------------------------------------------------------------------
 *                                          Extern Variable
 * ------------------------------------------------------------------------------------------------
 */

/*T1定时器计时标志*/
TIMER_C_Extern uint8_t T1_100MS_Flag;
TIMER_C_Extern uint8_t T1_1S_Flag;
TIMER_C_Extern uint8_t T1_3S_Flag;

/*T3定时器计时标志*/
TIMER_C_Extern uint8_t T3_10MS_Flag;


/* ------------------------------------------------------------------------------------------------
 *                                      variable
 * ------------------------------------------------------------------------------------------------
 */
void Timer1_Init(void);
void Timer3_Init(void);
void Timer_Int_Enable(void);
void Timer_Int_Disable(void);


void Delay_Init(void);
void Delay_Ms(uint16_t n);
#endif