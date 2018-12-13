/*
 * @Description: byte order convert
 * @Version: v2.0
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-12-03 21:54:24
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-04 10:50:42
 */

#ifndef _SWAP_H
#define _SWAP_H

#include <stdint.h>

#define PUB_SWAP16(VAL)         ((((uint16_t)(VAL) & 0xFF00) >> 8) |                \
                                 (((uint16_t)(VAL) & 0x00FF) << 8))

#define PUB_SWAP32(VAL)         ((((uint32_t)(VAL) & 0xFF000000) >> 24) |           \
                                 (((uint32_t)(VAL) & 0x00FF0000) >>  8) |           \
                                 (((uint32_t)(VAL) & 0x0000FF00) <<  8) |           \
                                 (((uint32_t)(VAL) & 0x000000FF) << 24))



uint32_t PUB_HTONL(uint32_t value);
uint32_t PUB_NTOHL(uint32_t value);
uint16_t PUB_HTONS(uint16_t value);
uint16_t PUB_NTOHS(uint16_t value); 



#endif

