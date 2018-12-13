/*
 * @Description: bsp flash
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-30 10:03:03
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-07 10:30:33
 */

#include "bsp_flash.h"
#include "ota.h"

/**
 * @brief:erase a sector
 * @param sector_num, start sector num
 *        num, how many sectors you want to erase.
 * @return: 0,success else,failed
 */
S8 BSP_FlashEraseSector(U8 sector_num, U8 num)
{
    FLASH_EraseInitTypeDef flash;
    U32 Error = 0;
    S8 ret = 0;
    if((num <= 0) || (num > FLASH_SECTOR_NUM) || (sector_num > 5)){
        ret = -1;
        goto EXIT;
    }

    flash.TypeErase = FLASH_TYPEERASE_SECTORS;
    flash.Sector = sector_num;
    flash.NbSectors = num;
    flash.VoltageRange = FLASH_VOLTAGE_RANGE_1;
    HAL_FLASH_Unlock();
    ret = HAL_FLASHEx_Erase(&flash, &Error);
    if(ret || (Error != 0xFFFFFFFF)){
        ret = -2;
        goto EXIT;
    }
    else{
        ret = 0;
    }

    EXIT:
        HAL_FLASH_Lock();
        return ret;
}


/**
 * @brief: erase whole chip
 * @param {type}
 * @return:0,success, -1,failed.
 */
S8 BSP_FlashEraseChip(void)
{
    FLASH_EraseInitTypeDef flash;
    S8 ret = 0;
    U32 error;
    flash.TypeErase = FLASH_TYPEERASE_MASSERASE;
    flash.Banks = FLASH_BANK_1;
    HAL_FLASH_Unlock();
    ret = HAL_FLASHEx_Erase(&flash, &error);
    if(ret || (error != 0xFFFFFFFF))
    {
        ret = -1;
        goto EXIT;
    }
    EXIT:
        HAL_FLASH_Lock();
        return ret;
}

/**
 * @brief:write data to flash
 * @param addr, start addr to write
 *        size, size of data
 *        src , point to dest data.
 * @return: HAL_OK, success
 */
S8 BSP_FlashWrite(U32 addr, U32 size, U8 *src)
{
    U32 i;
    HAL_StatusTypeDef ret;
    if((addr < FLASH_START) || (addr > FLASH_END) || (size == 0) || (src == NULL))
    {
        ret = HAL_ERROR;
        goto EXIT;
    }
    HAL_FLASH_Unlock();
    for(i=0; i<size; i++)
    {
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,addr,src[i]);
        if(ret != HAL_OK)
        {
            printf("program error\n");
            goto EXIT;
        }
        addr++;
    }

    EXIT:
        HAL_FLASH_Lock();
        return ret;
}


/**
 * @brief: read value from a addr
 * @param addr, read addr
 *        value, store value point
 * @return: 0, success
 */
S8 BSP_FlashReadByte(U32 addr, U8* value)
{
    S8 ret = 0;
    if((addr < FLASH_START) || (addr > FLASH_END))
    {
        ret = -1;
        goto EXIT;
    }
    
    *value = *(__IO U8*)addr;
    
    EXIT:
        return ret;
}

/**
 * @brief:read multi-bytes
 * @param  addr, read start addr
 *         buf, store data
 *         size, size of read data
 * @return: 0, success
 */
S8 BSP_FlashReadData(U32 addr, U8 *buf, U32 size)
{
    S8 ret = 0;
    U32 i;
    if((addr < FLASH_START) || (addr > FLASH_END))
    {
        ret = -1;
        goto EXIT;
    }    
    
    for(i=0; i<size; i++)
    {
        if(0 == BSP_FlashReadByte(addr,&buf[i]))
        {
            addr++;
        }
        else
        {
            ret = -2;
            break;
        }    
    }
    
    EXIT:
        return ret;
}

