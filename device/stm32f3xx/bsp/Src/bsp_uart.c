/*
 * @Description: uart bsp source file 
 * @Version: v2.0 
 * @Company: Orion
 * @Author: runner
 * @Date: 2018-11-16 17:55:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2018-12-04 14:22:15
 */

#include "bsp.h"

// Global (static)variable 
#if USART1_EN
static T_DEV_UART gtUsart1;
static uint8_t    gUsart1TxBuf[UART1_TX_BUF_SIZE];
static uint8_t    gUsart1RxBuf[UART1_RX_BUF_SIZE];
#endif

#if USART2_EN
static T_DEV_UART gtUsart2;
static uint8_t    gUsart2TxBuf[UART2_TX_BUF_SIZE];
static uint8_t    gUsart2RxBuf[UART2_RX_BUF_SIZE];
#endif

#if USART3_EN
static T_DEV_UART gtUsart3;
static uint8_t    gUsart3TxBuf[UART3_TX_BUF_SIZE];
static uint8_t    gUsart3RxBuf[UART3_RX_BUF_SIZE];
#endif

#if UART4_EN
static T_DEV_UART gtUart4;
static uint8_t    gUart4TxBuf[UART4_TX_BUF_SIZE];
static uint8_t    gUart4RxBuf[UART4_RX_BUF_SIZE];
#endif

#if UART5_EN
static T_DEV_UART gtUart5;
static uint8_t    gUart5TxBuf[UART5_TX_BUF_SIZE];
static uint8_t    gUart5RxBuf[UART5_RX_BUF_SIZE];
#endif

#if USART6_EN
static T_DEV_UART gtUsart6;
static uint8_t    gUsart6TxBuf[UART6_TX_BUF_SIZE];
static uint8_t    gUsart6RxBuf[UART6_RX_BUF_SIZE];
#endif


/**
 * @brief: uart init 
 * @param none 
 * @return: none 
 */
void BSP_UartInit(void)
{
    BSP_UartVarInit();
    BSP_UartHwInit();

    return;
}

/**
 * @brief:get uart/usart device struct point.  
 * @param ePort --- port of uart/usart.  
 * @return: none 
 */
static T_DEV_UART *BSP_UartGetDEV(E_COM_PORT ePort)
{
    T_DEV_UART *pRet = 0;

    switch (ePort)
    {
        case COM1:
            #if USART1_EN
            pRet = &gtUsart1;
            #endif
            break;

        case COM2:
            #if USART2_EN
            pRet = &gtUsart2;
            #endif
            break;

        case COM3:
            #if USART3_EN
            pRet = &gtUsart3;
            #endif
            break;

        case COM4:
            #if UART4_EN
            pRet = &gtUart4;
            #endif
            break;

        case COM5:
            #if UART5_EN
            pRet = &gtUart5;
            #endif
            break;

        case COM6:
            #if USART6_EN
            pRet = &gtUsart6;
            #endif
            break;

        default:
            pRet = 0;
            break;
    }

    return (pRet);
}

/**
 * @brief:open usart/uart.   
 * @param {type} ePort --- port of uart. 
 *               setBits --- task notify bit. 
 * @return: 
 */
uint32_t BSP_UartOpen(E_COM_PORT ePort, uint32_t setBits)
{
    uint32_t    ret    = 0;
    T_DEV_UART *ptUart = (T_DEV_UART *)0;

    ptUart = BSP_UartGetDEV(ePort);

    if ((T_DEV_UART *)0 == ptUart)
    {
        ret = 1;
        goto EXIT_LABEL;
    }

    if (0 != ptUart->isOpened)
    {
        ret = 2;
        goto EXIT_LABEL;
    }

    ptUart->isOpened = 1;

EXIT_LABEL:
    return ret;
}

/**
 * @brief: when uart idle irq happend, this api can get uart recvd data
 * @param  com, uart_num rxbuf, store uart recv data
 * @return: return recv data len
 */
uint32_t BSP_UartNtfRecvData(E_COM_PORT com, uint8_t *rxbuf)
{
    uint32_t i,len = 0;
    T_DEV_UART *ptUart;

    if(0 == com || NULL == rxbuf)
        return 0;
    
    //get uart handle
    ptUart = BSP_UartGetDEV(com);
    if(NULL == ptUart)
        return 0 ;

    //uart idle irq happend
    if(1 == ptUart->idle_flag)
    {
        //clear idle irq flag;
        ptUart->idle_flag = 0;
        //store data to rxbuf
        len = BSP_UartGetData(COM1, rxbuf, ptUart->rxFIFO.size);
        //our protocol pack min len is 5
        if(len < 5)
        {
            memset(rxbuf, 0, ptUart->rxFIFO.size);
            return 0;
        }
        printf("recv[%d]:",len);
        for(i=0; i<len; i++)
        {
            printf("%02x ", rxbuf[i]);
        }
        printf("\n");
    }

    return len;
}

/**
 * @brief: usart send data 
 * @param eport, buffer, len 
 * @return:  
 */
uint32_t BSP_UartSendData(E_COM_PORT ePort, uint8_t *buffer, uint16_t len)
{
    uint32_t    ret    = 0;
    uint32_t    cnts;
    T_DEV_UART *ptUart = (T_DEV_UART *)0;

    ptUart = BSP_UartGetDEV(ePort);

    if ((T_DEV_UART *)0 == ptUart)
    {
        goto EXIT_LABEL;
    }

    if (0 != ptUart->fnSndBefor)
    {
        ptUart->fnSndBefor();
    }

#if 0
    ret = BSP_KfifoPut(&(ptUart->txFIFO), buffer, len);
    __HAL_UART_ENABLE_IT(&(ptUart->tDev), UART_IT_TXE);
#else
    for (cnts=0; cnts<len; cnts++)
    {
        while (0 == __HAL_UART_GET_FLAG(&(ptUart->tDev), UART_FLAG_TXE));
        ptUart->tDev.Instance->DR = buffer[cnts];
    }
#endif

EXIT_LABEL:
    return ret;
}

/**
 * @brief: recv uart data
 * @param eport, port of uart
 *        buffer recv data buf
 *        len, recv data size
 * @return:
 */
uint32_t BSP_UartGetData(E_COM_PORT ePort, uint8_t *buffer, uint16_t len)
{
    uint32_t    n      = 0;
    T_DEV_UART *ptUart = (T_DEV_UART *)0;

    ptUart = BSP_UartGetDEV(ePort);

    if ((T_DEV_UART *)0 == ptUart)
    {
        goto EXIT_LABEL;
    }

    n = FIFO_Get(&(ptUart->rxFIFO), buffer, len);

EXIT_LABEL:
    return n;
}

/**
 * @brief: init global data of usart/uart.
 * @param {type}
 * @return:
 */
static void BSP_UartVarInit(void)
{
#if USART1_EN
    FIFO_Init(&gtUsart1.txFIFO, gUsart1TxBuf, UART1_TX_BUF_SIZE);
    FIFO_Init(&gtUsart1.rxFIFO, gUsart1RxBuf, UART1_RX_BUF_SIZE);

    gtUsart1.fnSndBefor = 0;
    gtUsart1.fnSndOver  = 0;
    gtUsart1.fnRcvNew   = 0;

    gtUsart1.isOpened = 0;
    gtUsart1.idle_flag = 0;
#endif

#if USART2_EN
    FIFO_Init(&gtUsart2.txFIFO, gUsart2TxBuf, UART2_TX_BUF_SIZE);
    FIFO_Init(&gtUsart2.rxFIFO, gUsart2RxBuf, UART2_RX_BUF_SIZE);

    gtUsart2.fnSndBefor = 0;
    gtUsart2.fnSndOver  = 0;
    gtUsart2.fnRcvNew   = 0;

    gtUsart2.isOpened = 0;
#endif

#if USART3_EN
    FIFO_Init(&gtUsart3.txFIFO, gUsart3TxBuf, UART3_TX_BUF_SIZE);
    FIFO_Init(&gtUsart3.rxFIFO, gUsart3RxBuf, UART3_RX_BUF_SIZE);

    gtUsart3.fnSndBefor = 0;
    gtUsart3.fnSndOver  = 0;
    gtUsart3.fnRcvNew   = 0;

    gtUsart3.isOpened = 0;
#endif

#if UART4_EN
    FIFO_Init(&gtUart4.txFIFO, gUart4TxBuf, UART4_TX_BUF_SIZE);
    FIFO_Init(&gtUart4.rxFIFO, gUart4RxBuf, UART4_RX_BUF_SIZE);

    gtUart4.fnSndBefor = 0;
    gtUart4.fnSndOver  = 0;
    gtUart4.fnRcvNew   = 0;

    gtUart4.isOpened = 0;
#endif

#if UART5_EN
    FIFO_Init(&gtUart5.txFIFO, gUart5TxBuf, UART5_TX_BUF_SIZE);
    FIFO_Init(&gtUart5.rxFIFO, gUart5RxBuf, UART5_RX_BUF_SIZE);

    gtUart5.fnSndBefor = 0;
    gtUart5.fnSndOver  = 0;
    gtUart5.fnRcvNew   = 0;

    gtUart5.isOpened = 0;
#endif

#if USART6_EN
    FIFO_Init(&gtUsart6.txFIFO, gUsart6TxBuf, UART6_TX_BUF_SIZE);
    FIFO_Init(&gtUsart6.rxFIFO, gUsart6RxBuf, UART6_RX_BUF_SIZE);

    gtUsart6.fnSndBefor = 0;
    gtUsart6.fnSndOver  = 0;
    gtUsart6.fnRcvNew   = 0;

    gtUsart6.isOpened = 0;
#endif

    return;
}

/**
 * @brief: init params of usart/uart. 
 * @param {type} 
 * @return: 
 */
static void BSP_UartHwInit(void)
{
#if USART1_EN
    gtUsart1.tDev.Instance = USART1;
    gtUsart1.tDev.Init.BaudRate   = UART1_BAUD;
    gtUsart1.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUsart1.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUsart1.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUsart1.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUsart1.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUsart1.tDev);

    __HAL_UART_ENABLE_IT(&(gtUsart1.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUsart1.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(USART1_IRQn, UART1_IRQ_PRI, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif

#if USART2_EN
    gtUsart2.tDev.Instance = USART2;
    gtUsart2.tDev.Init.BaudRate   = UART2_BAUD;
    gtUsart2.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUsart2.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUsart2.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUsart2.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUsart2.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUsart2.tDev);

    __HAL_UART_ENABLE_IT(&(gtUsart2.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUsart2.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(USART2_IRQn, UART2_IRQ_PRI, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
#endif

#if USART3_EN
    gtUsart3.tDev.Instance = USART3;
    gtUsart3.tDev.Init.BaudRate   = UART3_BAUD;
    gtUsart3.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUsart3.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUsart3.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUsart3.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUsart3.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUsart3.tDev);

    __HAL_UART_ENABLE_IT(&(gtUsart3.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUsart3.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
#endif

#if UART4_EN
    gtUart4.tDev.Instance = UART4;
    gtUart4.tDev.Init.BaudRate   = UART4_BAUD;
    gtUart4.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUart4.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUart4.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUart4.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUart4.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUart4.tDev);

    __HAL_UART_ENABLE_IT(&(gtUart4.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUart4.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(UART4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
#endif

#if UART5_EN
    gtUart5.tDev.Instance = UART5;
    gtUart5.tDev.Init.BaudRate   = UART5_BAUD;
    gtUart5.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUart5.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUart5.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUart5.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUart5.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUart5.tDev);

    __HAL_UART_ENABLE_IT(&(gtUart5.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUart5.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(USART5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART5_IRQn);
#endif
    
#if USART6_EN
    gtUsart6.tDev.Instance = USART6;
    gtUsart6.tDev.Init.BaudRate   = UART6_BAUD;
    gtUsart6.tDev.Init.WordLength = UART_WORDLENGTH_8B;
    gtUsart6.tDev.Init.StopBits   = UART_STOPBITS_1;
    gtUsart6.tDev.Init.Parity     = UART_PARITY_NONE;
    gtUsart6.tDev.Init.Mode       = UART_MODE_TX_RX;
    gtUsart6.tDev.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    HAL_UART_Init(&gtUsart6.tDev);

    __HAL_UART_ENABLE_IT(&(gtUsart6.tDev), UART_IT_RXNE|UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&(gtUsart6.tDev), UART_FLAG_TC);

    HAL_NVIC_SetPriority(USART6_IRQn, USART6_IRQ_PRI, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif

    return;
}

#if USART1_EN
void USART1_IRQHandler(void)
{
    BSP_UartIRQ(&gtUsart1);
}
#endif
#if USART2_EN
void USART2_IRQHandler(void)
{
    BSP_UartIRQ(&gtUsart2);
}
#endif
#if USART3_EN
void USART3_IRQHandler(void)
{
    BSP_UartIRQ(&gtUsart3);
}
#endif
#if UART4_EN
void USART4_IRQHandler(void)
{
    BSP_UartIRQ(&gtUart4);
}
#endif
#if UART5_EN
void USART5_IRQHandler(void)
{
    BSP_UartIRQ(&gtUart5);
}
#endif
#if USART6_EN
void USART6_IRQHandler(void)
{
    BSP_UartIRQ(&gtUsart6);
}
#endif


/**
 * @brief: uart irq handle
 * @param {type}
 * @return:
 */
static void BSP_UartIRQ(T_DEV_UART *ptUart)
{
    uint8_t  ch;
    uint32_t n;
#if USE_FREERTOS
    BaseType_t xHighPriorityTaskWoken = pdFALSE;
#endif
    //uart recv irq handle
    if (0 != __HAL_UART_GET_FLAG(&(ptUart->tDev), UART_FLAG_RXNE))
    {
        ch = ptUart->tDev.Instance->DR;
        FIFO_Put(&(ptUart->rxFIFO), &ch, 1);

        if (0 != ptUart->fnRcvNew)
        {
            ptUart->fnRcvNew(ch);
        }
    }
    //uart idle irq handle
    if (0 != __HAL_UART_GET_FLAG(&(ptUart->tDev), UART_FLAG_IDLE))
    {
        ch = ptUart->tDev.Instance->DR;
        ptUart->idle_flag = 1;

#if USE_FREERTOS
        if (0 != ptUart->taskHndl)
        {
            xTaskNotifyFromISR(ptUart->taskHndl, ptUart->setBits, eSetBits,
                               &xHighPriorityTaskWoken);
            portYIELD_FROM_ISR(xHighPriorityTaskWoken);
        }
#endif
    }
    //uart send irq handle
    if ((0 != __HAL_UART_GET_IT_SOURCE(&(ptUart->tDev), UART_IT_TXE)) 
     && (0 != __HAL_UART_GET_FLAG(&(ptUart->tDev), UART_FLAG_TXE)))
    {
        n = FIFO_Get(&(ptUart->txFIFO), &ch, 1);

        if (0 == n)
        {
            __HAL_UART_DISABLE_IT(&(ptUart->tDev), UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&(ptUart->tDev), UART_IT_TC);
        }
        else
        {
            ptUart->tDev.Instance->DR = ch;
        }
    }
    //uart send complate irq
    else if (0 != __HAL_UART_GET_FLAG(&(ptUart->tDev), UART_FLAG_TC))
    {
        __HAL_UART_DISABLE_IT(&(ptUart->tDev), UART_IT_TC);

        if (0 != ptUart->fnSndOver)
        {
            ptUart->fnSndOver();
        }
    }

    return;
}

/**
 * @brief:
 * @param {type} 
 * @return: 
 */
void BSP_ShowBuff(uint8_t *buf, uint32_t size)
{
    uint32_t i;

    if(NULL == buf || 0 == size)
        return;
    
    for(i=0; i<size; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

