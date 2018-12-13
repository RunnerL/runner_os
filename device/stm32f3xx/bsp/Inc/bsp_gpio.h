/*
 * @Description: bsp gpio head file
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-11-29 14:28:04
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

// global macro definition
//bit operation.
#define BIT_BAND(ADR, BNUM)         (((ADR) & 0xF0000000) + 0x2000000 +        \
                                    (((ADR) & 0xFFFFF) << 5) + ((BNUM) << 2))
#define MEM_ADDR(ADR)               *((volatile unsigned long  *)(ADR))
#define BIT_ADDR(ADR, BNUM)         MEM_ADDR(BIT_BAND((ADR), (BNUM)))

//GPIO OUTPUT data register addr.
#define GPIOA_ODR_ADDR              (GPIOA_BASE + 0x14)
#define GPIOB_ODR_ADDR              (GPIOB_BASE + 0x14)
#define GPIOC_ODR_ADDR              (GPIOC_BASE + 0x14)
#define GPIOD_ODR_ADDR              (GPIOD_BASE + 0x14)

//GPIO Input data register addr.
#define GPIOA_IDR_ADDR              (GPIOA_BASE + 0x10)
#define GPIOB_IDR_ADDR              (GPIOB_BASE + 0x10)
#define GPIOC_IDR_ADDR              (GPIOC_BASE + 0x10)
#define GPIOD_IDR_ADDR              (GPIOD_BASE + 0x10)

//IO Output or Input.
#define PA_OUT(BITX)                BIT_ADDR(GPIOA_ODR_ADDR, (BITX))
#define PA_IN(BITX)                 BIT_ADDR(GPIOA_IDR_ADDR, (BITX))

#define PB_OUT(BITX)                BIT_ADDR(GPIOB_ODR_ADDR, (BITX))
#define PB_IN(BITX)                 BIT_ADDR(GPIOB_IDR_ADDR, (BITX))

#define PC_OUT(BITX)                BIT_ADDR(GPIOC_ODR_ADDR, (BITX))
#define PC_IN(BITX)                 BIT_ADDR(GPIOC_IDR_ADDR, (BITX))

#define PD_OUT(BITX)                BIT_ADDR(GPIOD_ODR_ADDR, (BITX))
#define PD_IN(BITX)                 BIT_ADDR(GPIOD_IDR_ADDR, (BITX))


//function statement

void BSP_GPIOInitClient(void);
void BSP_GPIOInitStation(void);


#ifdef __cplusplus
    }
#endif

#endif


