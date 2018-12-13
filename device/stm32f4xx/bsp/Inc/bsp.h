/*
 * @Description: bsp head file 
 * @Version: v2.0 
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-06 21:37:52
 */

#ifndef BSP_H
#define BSP_H


#include <string.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_tim.h"
#include "bsp_flash.h"

#ifdef __cplusplus
extern "C" 
#endif


// Global macro definition   
//interrupt priority level defination. 
#define RTC_WAKEUP_IRQ_PRI          2
#define UART6_IRQ_PRI               3
#define BLE_EXTI_IRQ_PRI            4
#define UART1_IRQ_PRI               5
#define UART2_IRQ_PRI               6
#define PWM_IRQ_PRI                 7
#define GSENSOR_EXTI_IRQ_PRI        8
#define HIGH_PRCS_TIMER_IRQ_PRI     9
#define ACC_VAL_EXTI_IRQ_PRI        10
#define BAT_VAL_EXTI_IRQ_PRI        11
#define TIM2_FOR_SLEEP_IRQ_PRI      12
#define WWDG_IRQ_PRI                13


// Global function prototype    
void BSP_Init(void);
void BSP_InitBoot(void);
void BSP_InitClient(void);
void BSP_InitStation(void);
void BSP_SysClkCfg(void);

void _Error_Handler(char *file, int line);
int fputc(int ch, FILE *f);



#ifdef __cplusplus
    }
#endif

#endif


