/*
 * @Description: bsp flash head file
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-30 10:03:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-05 18:16:11
 */
#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

#include "stm32f4xx_hal.h"
#include "pub_typedefs.h"


//function statement
S8 BSP_FlashWrite(uint32_t addr, uint32_t size, uint8_t *src);
S8 BSP_FlashEraseChip(void);
S8 BSP_FlashEraseSector(U8 sector_num, U8 num);
S8 BSP_FlashReadByte(U32 addr, U8* value);
S8 BSP_FlashReadData(U32 addr, U8 *buf, U32 size);

#endif

