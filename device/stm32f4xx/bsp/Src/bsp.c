/*
 * @Description: bsp source file
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-06 21:37:30
 */

#include "bsp.h"
#include "IR.h"

//Global (static)variable


/**
 * @brief:This function is executed in case of error occurrence.
 * @param :file --- The file name as string.
 *         line --- The line in file as a number.
 * @return:none
 */
void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

/**
 * @brief: printf redirect
 * @param : none
 * @return: none
 */
int fputc(int ch, FILE *f)
{     
    while((USART1->SR&0X40)==0);
    USART1->DR = (uint8_t) ch;
    return ch;
}



/**
 * @brief: bsp init
 * @param none
 * @return: none
 */
void BSP_Init(void)
{
    HAL_Init();
    BSP_SysClkCfg();
    BSP_UartInit();
    BSP_DWTInit();
    IR_PIN_INIT;
    BSP_Tim12Init();
    return;
}

/**
 * @brief: boot bsp init
 * @param none
 * @return: none
 */
void BSP_InitBoot(void)
{
    HAL_Init();
    BSP_SysClkCfg();
    BSP_UartInit();
    return;
}
/**
 * @brief: station bsp init
 * @param {type}
 * @return:
 */
void BSP_InitStation(void)
{
    HAL_Init();
    BSP_SysClkCfg();
    BSP_UartInit();
    BSP_DWTInit();
    BSP_GPIOInitStation();
    IR_PIN_INIT;
    BSP_Tim12Init();
    BSP_Tim7Init();
    return;
}
/**
 * @brief:client bsp init
 * @param none
 * @return: none
 */
void BSP_InitClient(void)
{
    HAL_Init();
    BSP_SysClkCfg();
    BSP_UartInit();
    BSP_DWTInit();
    BSP_GPIOInitClient();
    IR_PIN_INIT;
    BSP_Tim12Init();
    BSP_Tim7Init();
    return;
}

/**
 * @brief: config system clock.
 * @param {type}
 * @return:
 */
void BSP_SysClkCfg(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


    //Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType    = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState          = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState      = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource     = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM          = 4;
    RCC_OscInitStruct.PLL.PLLN          = 168;
    RCC_OscInitStruct.PLL.PLLP          = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ          = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType         = RCC_CLOCKTYPE_HCLK   |
                                          RCC_CLOCKTYPE_SYSCLK |
                                          RCC_CLOCKTYPE_PCLK1  |
                                          RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource      = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider     = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider    = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider    = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //Configure the Systick interrupt time
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    // Configure the Systick
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    //SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


