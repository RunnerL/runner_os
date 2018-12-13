/*<FH+>************************************************************************/
/*                                                                            */
/* Copyright (C) Orion. 2018. All rights reserved.                            */
/*                                                                            */
/* File Name    : fifo.h                                                      */
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
#ifndef FIFO_H
#define FIFO_H


/******************************************************************************/
/* #include£¨standard library header file,non-standard library header file£©  */
/******************************************************************************/


/******************************************************************************/
/*                      Other conditional compilation options                 */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                             Constant definition                            */
/******************************************************************************/


/******************************************************************************/
/*                           Global macro definition                          */
/******************************************************************************/


/******************************************************************************/
/*                         Global data type definitions                       */
/******************************************************************************/

/*<STRUCT+>********************************************************************/
/* STRUCT: T_FIFO                                                             */
/* ANT   : fifo structure defination                                          */
/*<STRUCT->********************************************************************/
typedef struct t_fifo
{
    unsigned char *buffer;                      /* buf used to save data.     */
    unsigned int   size;                        /* buf size , must be 2`s mul */
    unsigned int   in;                          /* enqueue index.             */
    unsigned int   out;                         /* dequeue index.             */

} T_FIFO;


/******************************************************************************/
/*                          Global variable declaration                       */
/******************************************************************************/


/******************************************************************************/
/*                            Global function prototype                       */
/******************************************************************************/
void FIFO_Init(T_FIFO *fifo, unsigned char *buf, unsigned int size);
unsigned int FIFO_Put(T_FIFO *fifo, unsigned char *buf, unsigned int len);
unsigned int FIFO_Get(T_FIFO *fifo, unsigned char *buf, unsigned int len);


#ifdef __cplusplus
    }
#endif

#endif


