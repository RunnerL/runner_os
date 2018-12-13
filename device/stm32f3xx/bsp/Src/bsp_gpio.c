/*
 * @Description: bsp gpio source file
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-04 11:19:57
 */

#include "bsp.h"
#include "IR.h"

/**
 * @brief: client bsp gpio init
 * @param {type}
 * @return:
 */
void BSP_GPIOInitClient(void)
{
    GPIO_InitTypeDef gpioInit;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /**************************************************************************/
    /* PB3 : output, ir left led.                                             */
    /* PB4 : output, ir middle led.                                           */
    /**************************************************************************/
    gpioInit.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull = GPIO_PULLDOWN;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

#if HW_VER_H201

    /**************************************************************************/
    /* PB11 : input, ir left led.                                             */
    /* PB12 : input, ir left mid led.                                         */
    /* PB13 : input, ir right mid led.                                        */
    /* PB14 : input, ir right led.                                            */
    /**************************************************************************/
    gpioInit.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    gpioInit.Mode = GPIO_MODE_IT_FALLING;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

    // HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

#endif 
#if HW_VER_H301
    /**************************************************************************/
    /* PB6 : input, ir left led.                                             */
    /* PB7 : input, ir left mid led.                                         */
    /* PB8 : input, ir right mid led.                                        */
    /* PB9 : input, ir right led.                                            */
    /**************************************************************************/
    gpioInit.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    gpioInit.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpioInit.Pull = GPIO_PULLDOWN;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

    // HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

#endif
}

/**
 * @brief: station gpio init
 * @param {type}
 * @return:
 */
void BSP_GPIOInitStation(void)
{
    GPIO_InitTypeDef gpioInit;
    /**************************************************************************/
    /* PB3 : output, ir left led.                                             */
    /* PB4 : output, ir middle led.                                           */
    /* PB5 : output, ir right led.                                            */
    /* PB13 : input, ir left led.                                             */
    /* PB14 : input, ir right led.                                            */
    /**************************************************************************/
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpioInit.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull = GPIO_PULLDOWN;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

    gpioInit.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    gpioInit.Mode = GPIO_MODE_IT_FALLING;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

    // HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    return;
}

/**
 * @brief: input channel irq handle, to get pulse width.
 * @param  GPIO_Pin
 * @return: none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

#if CLIENT_ROLE
#if HW_VER_H201
    if (GPIO_Pin == ir_client_left_in)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_left_mid_in)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_MID_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_right_mid_in)
    {
        IR_InputGetPulseTime(IR_INPUT_RIGHT_MID_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_right_in)
    {
        IR_InputGetPulseTime(IR_INPUT_RIGHT_CHANNEL);
    }
    else
        ;
#endif
#if HW_VER_H301
    if (GPIO_Pin == ir_client_left_in1)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_left_mid_in1)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_MID_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_right_mid_in1)
    {
        IR_InputGetPulseTime(IR_INPUT_RIGHT_MID_CHANNEL);
    }
    else if (GPIO_Pin == ir_client_right_in1)
    {
        IR_InputGetPulseTime(IR_INPUT_RIGHT_CHANNEL);
    }
    else
        ;
#endif

#endif
#if STATION_ROLE
    if (GPIO_Pin == ir_station_left_in)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_CHANNEL);
    }
    else if (GPIO_Pin == ir_station_right_in)
    {
        IR_InputGetPulseTime(IR_INPUT_LEFT_MID_CHANNEL);
    }
    else
        return;
#endif
}
