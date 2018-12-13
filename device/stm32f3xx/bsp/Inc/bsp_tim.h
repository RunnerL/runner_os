/*
 * @Description: bsp timer head file
 * @Version:v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-11-29 14:28:44
 */

#ifndef BSP_TIM_H
#define BSP_TIM_H

#ifdef __cplusplus
extern "C" {
#endif



//Global function prototype 
void BSP_DWTInit(void);
void BSP_DelayUs(uint32_t nus);
void BSP_MyDelay(uint32_t time);
void BSP_Tim4Init(void);
void BSP_Tim7Init(void);
void BSP_Tim12Init(void);
void BSP_TimPwmStart(TIM_HandleTypeDef *htim, uint32_t Channel);
void BSP_TimPwmStop(TIM_HandleTypeDef *htim, uint32_t Channel);
void BSP_TimCountStop(TIM_HandleTypeDef *htim);
void BSP_TimCountStart(TIM_HandleTypeDef *htim);
void BSP_TimSetCount(TIM_HandleTypeDef *htim, uint32_t counter);
uint32_t BSP_TimGetCount(TIM_HandleTypeDef *htim);


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* timHandle);
#ifdef __cplusplus
    }
#endif

#endif


