/*
 * @Description: bsp uart head file 
 * @Version: v2.0 
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-03 21:48:33
 */

#ifndef BSP_UART_H
#define BSP_UART_H


#include "fifo.h"

#ifdef __cplusplus
extern "C" {
#endif


/*                           Global macro definition                          */
/* uart enable switch.                                                        */
#define USART1_EN                        1
#define USART2_EN                        0
#define USART3_EN                        0
#define UART4_EN                         0
#define UART5_EN                         0
#define USART6_EN                        0

/* uart baudrate and r/t buf size definition.                                 */
#if USART1_EN
#define UART1_BAUD                      9600
#define UART1_TX_BUF_SIZE               1*1024
#define UART1_RX_BUF_SIZE               1*1024
#endif

#if USART2_EN
#define UART2_BAUD                      9600
#define UART2_TX_BUF_SIZE               4
#define UART2_RX_BUF_SIZE               1*1024
#endif

#if USART3_EN
#define UART3_BAUD                      115200
#define UART3_TX_BUF_SIZE               4
#define UART3_RX_BUF_SIZE               1*1024
#endif

#if UART4_EN
#define UART4_BAUD                      115200
#define UART4_TX_BUF_SIZE               4
#define UART4_RX_BUF_SIZE               1*1024
#endif

#if UART5_EN
#define UART5_BAUD                      115200
#define UART5_TX_BUF_SIZE               4
#define UART5_RX_BUF_SIZE               1*1024
#endif

#if USART6_EN
#define UART6_BAUD                      115200
#define UART6_TX_BUF_SIZE               4
#define UART6_RX_BUF_SIZE               1*1024
#endif

/*                         Global data type definitions                       */

/* STRUCT: T_DEV_UART                                                         */
/* ANT   : uart/usart struct                                                  */
typedef struct t_dev_uart
{
    UART_HandleTypeDef tDev;                    /* point to uart/usart device.*/
    T_FIFO            txFIFO;                  /* transfer fifo.             */
    T_FIFO            rxFIFO;                  /* receive fifo.              */

    void (*fnSndBefor)(void);                   /* the callback before you se */
    void (*fnSndOver)(void);                    /* the callback finish send.  */
    void (*fnRcvNew)(uint8_t data);             /* the callback recv data.    */

    uint32_t           isOpened;                /* the device opened or not.  */

    uint8_t            idle_flag;               /* uart idle irq flag */

} T_DEV_UART;

/* ENUM: E_COM_PORT                                                           */
/* ANT : port of uart/usart.                                                  */
typedef enum e_com_port
{
    COM1 = 1,
    COM2 = 2,
    COM3 = 3,
    COM4 = 4,
    COM5 = 5,
    COM6 = 6,
    COM7 = 7

} E_COM_PORT;


//Global function prototype
void BSP_UartInit(void);
static void BSP_UartVarInit(void);
static void BSP_UartHwInit(void);
uint32_t BSP_UartOpen(E_COM_PORT ePort, uint32_t setBits);
static T_DEV_UART *BSP_UartGetDEV(E_COM_PORT ePort);
uint32_t BSP_UartGetData(E_COM_PORT ePort, uint8_t *buffer, uint16_t len);
uint32_t BSP_UartSendData(E_COM_PORT ePort, uint8_t *buffer, uint16_t len);

uint32_t BSP_UartNtfRecvData(E_COM_PORT com, uint8_t *rxbuf);

static void BSP_UartIRQ(T_DEV_UART *ptUart);
void BSP_ShowBuff(uint8_t *buf, uint32_t size);

#ifdef __cplusplus
    }
#endif

#endif


