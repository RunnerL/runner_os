/*<FH+>************************************************************************/
/*                                                                            */
/* Copyright (C) Orion. 2018. All rights reserved.                            */
/*                                                                            */
/* File Name    : fifo.c                                                      */
/* Major Topics : fifo                                                        */
/* Other        : none                                                        */
/* Current Ver  : v2.0                                                        */
/* Author       : runner                                                      */
/* Finish Date  : 2018-11-7                                                   */
/* Modify Record:                                                             */
/*         Date          Ver        Name          Details                     */
/* -------------------------------------------------------------------------- */
/*     2018-11-7         v2.0        runner          Create File              */
/*<FH->************************************************************************/


/******************************************************************************/
/* #include£¨standard library header file,non-standard library header file£©  */
/******************************************************************************/
#include <string.h>
#include "fifo.h"

/******************************************************************************/
/*                         External reference declaration                     */
/******************************************************************************/


/******************************************************************************/
/*                            Internal macro definition                       */
/******************************************************************************/
#define __max(a, b)                 (((a) > (b)) ? (a) : (b))
#define __min(a, b)                 (((a) < (b)) ? (a) : (b))


/******************************************************************************/
/*                          Internal data type definitions                    */
/******************************************************************************/


/******************************************************************************/
/*                             Global (static)variable                        */
/******************************************************************************/


/******************************************************************************/
/*                           Internal function prototype                      */
/******************************************************************************/


/******************************************************************************/
/*                          Global function implementation                    */
/******************************************************************************/


/******************************************************************************/
/*                         Internal function implementation                   */
/******************************************************************************/


/*<FUNC+>**********************************************************************/
/* Func Name       : FIFO_Init                                                */
/* Func Desc       : fifo init.                                               */
/* Input parameter : fifo --- point to fifo struct.                           */
/*                  buf --- point to fifo buf.                                */
/*                  size --- fifo buf size.                                   */
/* Output Parameter: none                                                     */
/* Return Value    : void                                                     */
/* Opt process     :                                                          */
/* Other           : none                                                     */
/* Modify          :                                                          */
/* -------------------------------------------------------------------------- */
/*     2018-11-7               v2.0          runner        Create Func        */
/*<FUNC->**********************************************************************/
void FIFO_Init(T_FIFO *fifo, unsigned char *buf, unsigned int size)
{
    fifo->buffer = buf;
    fifo->size   = size;
    fifo->in     = 0;
    fifo->out    = 0;

    return;
}

/*<FUNC+>**********************************************************************/
/* Func Name       : FIFO_Put                                                 */
/* Func Desc       : enqueue                                                  */
/* Input parameter : fifo --- fifo you want to use.                           */
/*                  buf --- use to store datas.                               */
/*                  len --- size of you want to enqueue.                      */
/* Output Parameter: none                                                     */
/* Return Value    : unsigned int                                             */
/* Opt process     :                                                          */
/* Other           : none                                                     */
/* Modify          :                                                          */
/* -------------------------------------------------------------------------- */
/*     2018-11-8               v2.0          runner        Create Func        */
/*<FUNC->**********************************************************************/
unsigned int FIFO_Put(T_FIFO *fifo, unsigned char *buf, unsigned int len)
{
    unsigned int n;

    /**************************************************************************/
    /* get remain size                                                        */
    /**************************************************************************/
    len = __min(len, fifo->size - fifo->in + fifo->out);

    /**************************************************************************/
    /* size to buf end.                                                       */
    /**************************************************************************/
    n = __min(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buf, n);

    /**************************************************************************/
    /* make a fresh start to store, size is len-n.                            */
    /**************************************************************************/
    memcpy(fifo->buffer, buf + n, len - n);

    fifo->in += len;

    return len;
}


/*<FUNC+>**********************************************************************/
/* Func Name       : FIFO_Get                                                 */
/* Func Desc       : dequeue                                                  */
/* Input parameter : fifo --- fifo you want to dequeue.                       */
/*                  buf --- use to store datas.                               */
/*                  len --- size of you want to dequeue.                      */
/* Output Parameter: none                                                     */
/* Return Value    : unsigned int                                             */
/* Opt process     :                                                          */
/* Other           : none                                                     */
/* Modify          :                                                          */
/* -------------------------------------------------------------------------- */
/*     2018-11-8               v2.0          runner        Create Func        */
/*<FUNC->**********************************************************************/
unsigned int FIFO_Get(T_FIFO *fifo, unsigned char *buf, unsigned int len)
{
    unsigned int n;


    /**************************************************************************/
    /* get size of you could dequeue.                                         */
    /**************************************************************************/
    len = __min(len, fifo->in - fifo->out);


    /**************************************************************************/
    /* size to buf end.                                                       */
    /**************************************************************************/
    n = __min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buf, fifo->buffer + (fifo->out & (fifo->size - 1)), n);

    /**************************************************************************/
    /* make a fresh start to dequeue.                                         */
    /**************************************************************************/
    memcpy(buf + n, fifo->buffer, len - n);

    fifo->out += len;

    return len;
}


