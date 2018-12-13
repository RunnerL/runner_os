/*
 * @Description: hal_msp.c
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-11-29 14:26:52
 */

#include "bsp.h"

/**
 * @brief: sys clock, HSE,LSE,gpio clock enable
 * @param {none}
 * @return:none
 */
void HAL_MspInit(void)
{
    //enable sys clk.
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    //enable HSE gpio clk
    __HAL_RCC_GPIOC_CLK_ENABLE();
    //enable LSE gpio clk.
    __HAL_RCC_GPIOH_CLK_ENABLE();

    return;
}

/**
 * @brief: usart clock, gpio clock enable, usart gpio config
 * @param {huart, point to uart}
 * @return:none
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpioInit;

    if (USART1 == huart->Instance)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**USART1 GPIO Configuration
        PA9      ------> USART1_TX
        PA10     ------> USART1_RX
        */
        gpioInit.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        gpioInit.Mode = GPIO_MODE_AF_PP;
        gpioInit.Pull = GPIO_PULLUP;
        gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
        gpioInit.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &gpioInit);
    }

    if (UART4 == huart->Instance)
    {
        __HAL_RCC_UART4_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /**UART4 GPIO Configuration
        PC10     ------> UART4_TX
        PC11     ------> UART4_RX
        */
        gpioInit.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        gpioInit.Mode = GPIO_MODE_AF_PP;
        gpioInit.Pull = GPIO_PULLUP;
        gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpioInit.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOC, &gpioInit);
    }

    return;
}

/**
 * @brief:tim12 pwm channel gpio config.
 * @param timHandle, point to timer handle.
 * @return: none
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if (timHandle->Instance == TIM12)
    {
        __HAL_RCC_TIM12_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

/**
 * @brief: tim base count config
 * @param {type}
 * @return:
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM7)
    {
        /* TIM7 clock enable */
        __HAL_RCC_TIM7_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
}
/**
 * @brief: tim4 input capture channel gpio config
 * @param :tim_icHandle, point to timer handle.
 * @return:none
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *tim_icHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if (tim_icHandle->Instance == TIM4)
    {
        /* TIM4 clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();

        /**TIM4 GPIO Configuration
        PB6     ------> TIM4_CH1
        PB7     ------> TIM4_CH2
        PB8     ------> TIM4_CH3
        PB9     ------> TIM4_CH4
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* TIM4 interrupt Init */
        HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
}
