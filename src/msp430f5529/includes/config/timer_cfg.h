#ifndef INCLUDES_CONFIG_TIMER_CFG_H_
#define INCLUDES_CONFIG_TIMER_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* Function declarations */
void Init_Timers(void);
void Init_Timer_A(void);
void Init_Timer_B(void);

/***** GLOBALE VARS *****/
extern uint8_t g_timer_1hz_flag;
extern uint8_t g_timer_1sek_flag;

#endif /* INCLUDES_CONFIG_TIMER_CFG_H_ */
