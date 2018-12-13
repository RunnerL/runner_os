/*
 * @Description: byte order
 * @Version:v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-12-03 21:53:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-04 10:20:47
 */


#include "swap.h"

/**
 * @brief: test byte order
 * @param {type}
 * @return: true big-Endian false small-Endian
 */
uint32_t PUB_CheckEndian(void)
{
    union
    {
        uint16_t usVal;
        uint8_t  ucArray[2];
    } uChkStrut;

    uChkStrut.usVal = 0x1234;

    return (0x12 == uChkStrut.ucArray[0]);
}

/**
 * @brief: machine sequence convert to net-order
 * @param value, convert dest data
 * @return: result of converting
 */
uint16_t PUB_HTONS(uint16_t value)
{
    return PUB_CheckEndian() ? value : PUB_SWAP16(value);
}

/**
 * @brief: net-order convert to machine sequence
 * @param  value , convert dest data
 * @return: result of converting
 */
uint16_t PUB_NTOHS(uint16_t value)
{
    return PUB_CheckEndian() ? value : PUB_SWAP16(value);
}

/**
 * @brief: net-order convert to machine sequence
 * @param value, convert dest data
 * @return: result of converting
 */
uint32_t PUB_NTOHL(uint32_t value)
{
    return PUB_CheckEndian() ? value : PUB_SWAP32(value);
}

/**
 * @brief: machine sequence convert to net-order
 * @param value, convert dest data
 * @return: result of converting
 */
uint32_t PUB_HTONL(uint32_t value)
{
    return PUB_CheckEndian() ?  value : PUB_SWAP32(value);
}

