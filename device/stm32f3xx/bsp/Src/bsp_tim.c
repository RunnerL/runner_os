/*
 * @Description: 
 * @Version: 
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-04 11:20:18
 */

#include "bsp.h"
#include "IR.h"

//External reference declaration
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim12;

/**
 * @brief: DWT init
 * @param none
 * @return:none
 */
void BSP_DWTInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    return;
}

/**
 * @brief: delay 1us.
 * @param nus, time. delay time = time*1us
 * @return: none
 */
void BSP_DelayUs(uint32_t nus)
{
    uint32_t cnts;
    uint32_t delayCnt;
    uint32_t start;

    start = DWT->CYCCNT;

    cnts = 0;
    delayCnt = nus * (SystemCoreClock / 1000000);

    while (cnts < delayCnt)
    {
        cnts = DWT->CYCCNT - start;
    }

    return;
}

/**
 * @brief: test delay func
 * @param time
 * @return: none
 */
void BSP_MyDelay(uint32_t time)
{
    uint32_t i, j;
    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 1000; j++)
        {
        }
    }
}

/**
 * @brief:init timer, for ac_station infrared output pwm.
 * @param : none
 * @return:none
 */
void BSP_Tim12Init(void)
{
    TIM_OC_InitTypeDef sConfigOC;

    htim12.Instance = TIM12;
    //APB1 ==> 84MHz
    htim12.Init.Prescaler = 84 - 1;
    htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
    //period : 84MHz/((84-1)*26) = 38KHz
    htim12.Init.Period = 26;
    htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    //PWM 1/3
    sConfigOC.Pulse = 8;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

/**
 * @brief: this timer used for ac_client infrared input capture.
 * @param {type}
 * @return:
 */
void BSP_Tim4Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_IC_InitTypeDef sConfigIC;

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 84 - 1;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 10000 - 1;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 8;
    if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

/**
 * @brief: timer input capture callback.
 * @param : htim , point to timer handle.
 * @return: none
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//    if (TIM4 == htim->Instance)
//    {
//        IR_InputAnalyzeByTimCapt(htim);
//    }
}

/**
 * @brief: ir recv analyze timer
 * @param {type}
 * @return:
 */
void BSP_Tim7Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig;

    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 84 - 1;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = IR_INPUT_TIMER_PERIOD - 1;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // HAL_TIM_Base_Start_IT(&htim7);
}

/**
 * @brief: tim7 count irq
 * @param {type}
 * @return:
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim7)
    {
#if STATION_ROLE
        ir_input_analyze[IR_INPUT_LEFT_CHANNEL].counter++;
        ir_input_analyze[IR_INPUT_LEFT_MID_CHANNEL].counter++;
#elif CLIENT_ROLE
        ir_input_analyze[IR_INPUT_LEFT_CHANNEL].counter++;
        ir_input_analyze[IR_INPUT_LEFT_MID_CHANNEL].counter++;
        ir_input_analyze[IR_INPUT_RIGHT_MID_CHANNEL].counter++;
        ir_input_analyze[IR_INPUT_RIGHT_CHANNEL].counter++;
#endif
    }
}

/**
 * @brief: start the pwm channel.
 * @param htim, point to timer.
 *        Channel, pwm channel.
 * @return: none
 */
void BSP_TimPwmStart(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    HAL_TIM_PWM_Start(htim, Channel);
}

/**
 * @brief: stop the pwm channel output.
 * @param htim, point to timer.
 *        Channel, pwm channel.
 * @return: none
 */
void BSP_TimPwmStop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    HAL_TIM_PWM_Stop(htim, Channel);
}

/**
 * @brief: start timer count
 * @param htim, point to timer
 * @return:none
 */
void BSP_TimCountStart(TIM_HandleTypeDef *htim)
{
    BSP_TimCountStart(htim);
}

/**
 * @brief: stop timer count
 * @param htim, point to timer
 * @return:none
 */
void BSP_TimCountStop(TIM_HandleTypeDef *htim)
{
    HAL_TIM_Base_Stop(htim);
}

/**
 * @brief: set timer counter
 * @param htim, counter
 * @return: none
 */
void BSP_TimSetCount(TIM_HandleTypeDef *htim, uint32_t counter)
{
    __HAL_TIM_SET_COUNTER(htim, counter);
}

/**
 * @brief: get timer counter
 * @param htim
 * @return: counter
 */
uint32_t BSP_TimGetCount(TIM_HandleTypeDef *htim)
{
    return __HAL_TIM_GET_COUNTER(htim);
}
