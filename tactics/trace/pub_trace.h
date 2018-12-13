/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: Copyright (C) 7号电单车. 2017. All rights reserved.              */
/*                                                                            */
/* 文件名称: pub_trace.h                                                      */
/* 内容摘要: TRACE模块的头文件                                                */
/* 其它说明: 无                                                               */
/* 当前版本: v0.9                                                             */
/* 作    者: maag                                                             */
/* 完成日期: 2017-11-23                                                       */
/* 修改记录:                                                                  */
/*     修改日期          版本号      修改人          修改内容                 */
/* -------------------------------------------------------------------------- */
/*     2017-11-23        v0.9        maag            创建文件                 */
/*<FH->************************************************************************/
#ifndef PUB_TRACE_H
#define PUB_TRACE_H


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/


/******************************************************************************/
/*                              其他条件编译选项                              */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                                  常量定义                                  */
/******************************************************************************/
#if defined (DEBUG) && defined (FT_TRC_EN)

#define TRC_MAX_FILES           5               /* 可过滤的最大文件数目       */
#define TRC_MAX_NAME_SIZE       32              /* 名称字符串的最大长度       */
#define TRC_SIO                 1               /* 直接串口输出的开关         */

/******************************************************************************/
/* TRACE等级划分                                                              */
/******************************************************************************/
#define TRC_LVL_ALL             1               /* 输出所有TRACE信息          */
#define TRC_LVL_FLOW            2               /* 输出各分支流程信息         */
#define TRC_LVL_DETAIL          3               /* 输出详细数据信息           */
#define TRC_LVL_ABNORMAL        4               /* 输出异常信息(系统能允许的错*/
#define TRC_LVL_ERROR           5               /* 输出错误信息(能导致系统崩溃*/
#define TRC_LVL_NO              6               /* 无TRACE信息                */

#define TRC_CUR_LVL             TRC_LVL_ALL     /* 当前TRACE等级              */

#endif


/******************************************************************************/
/*                                 全局宏定义                                 */
/******************************************************************************/
#ifdef DEBUG
#define SYS_OUT(STR, ...)       m_printf(DBG_COM, (STR), ##__VA_ARGS__)
#else
#define SYS_OUT(STR, ...)       ((void)0U)
#endif

#if defined (DEBUG) && defined (FT_TRC_EN)
/******************************************************************************/
/* TRACE输出宏定义                                                            */
/******************************************************************************/
#define TRC_OUT(STR, ...)       m_printf(DBG_COM, STR, ##__VA_ARGS__)
#define TRC_REC(LVL, STR, ...)  do                                             \
                                {                                              \
                                  if ((gTrcLevel <= LVL)                       \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_RecordPrefix(__FILE__, __LINE__);    \
                                    TRC_OUT(STR, ##__VA_ARGS__);               \
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)

/******************************************************************************/
/* TRACE接口宏定义                                                            */
/******************************************************************************/
#define TRC_INIT()              TRACE_Init()
#define TRC_ENTRY(FN_NAME)      do                                             \
                                {                                              \
                                  if ((gTrcLevel <= TRC_LVL_ALL)               \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_Entry(__FILE__, __LINE__, (FN_NAME));\
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)
#define TRC_EXIT()              do                                             \
                                {                                              \
                                  if ((gTrcLevel <= TRC_LVL_ALL)               \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_Exit(__FILE__, __LINE__);            \
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)
#define TRC_FLOW(STR, ...)      TRC_REC(TRC_LVL_FLOW, (STR), ##__VA_ARGS__)
#define TRC_DET(STR, ...)       TRC_REC(TRC_LVL_DETAIL, (STR), ##__VA_ARGS__)
#define TRC_ABNM(STR, ...)      TRC_REC(TRC_LVL_ABNORMAL, (STR), ##__VA_ARGS__)
#define TRC_ERR(STR, ...)       TRC_REC(TRC_LVL_ERROR, (STR), ##__VA_ARGS__)
#define TRC_SHOW(BUF, SIZE)     do                                             \
                                {                                              \
                                  if ((gTrcLevel <= TRC_LVL_DETAIL)            \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_ShowBuff((BUF), (SIZE));             \
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)
#define TRC_DUMP(BUF, SIZE)     do                                             \
                                {                                              \
                                  if ((gTrcLevel <= TRC_LVL_DETAIL)            \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_HexDump((BUF), (SIZE));              \
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)
#define LOG(STR, ...)           do                                             \
                                {                                              \
                                  if ((gTrcLevel <= TRC_LVL_DETAIL)            \
                                   && (0 == TRACE_FileFilter(__FILE__)))       \
                                  {                                            \
                                    OS_SemWait(gTrcEvtMutex, WAIT_FOREVER);    \
                                    TRACE_RecordPrefix(__FILE__, __LINE__);    \
                                    TRC_OUT("<LOG:> ");                        \
                                    TRC_OUT((STR), ##__VA_ARGS__);             \
                                    OS_SemRelease(gTrcEvtMutex);               \
                                  }                                            \
                                } while (0)

#else

#define TRC_INIT()              ((void)0U)
#define TRC_ENTRY(FN_NAME)      ((void)0U)
#define TRC_EXIT()              ((void)0U)
#define TRC_FLOW(STR, ...)      ((void)0U)
#define TRC_DET(STR, ...)       ((void)0U)
#define TRC_ABNM(STR, ...)      ((void)0U)
#define TRC_ERR(STR, ...)       ((void)0U)
#define TRC_SHOW(BUF, SIZE)     ((void)0U)
#define TRC_DUMP(BUF, SIZE)     ((void)0U)
#define LOG(STR, ...)           ((void)0U)

#endif



/******************************************************************************/
/*                              全局数据类型定义                              */
/******************************************************************************/


/******************************************************************************/
/*                                全局变量声明                                */
/******************************************************************************/
#if defined (DEBUG) && defined (FT_TRC_EN)
extern OS_SEM gTrcEvtMutex;
extern U32    gTrcLevel;
#endif


/******************************************************************************/
/*                                 全局函数原型                               */
/******************************************************************************/
#if defined (DEBUG) && defined (FT_TRC_EN)
USR_STATUS TRACE_Init(void);
USR_STATUS TRACE_EvtInit(void);
void       TRACE_GetSelfTaskName(char *pTaskName, U32 size);
S32        TRACE_GetFnNameInStk(const char *pTaskName, char *pFuncName);
USR_STATUS TRACE_FileFilter(const char *filePath);
void       TRACE_RecordPrefix(const char *pFile, U32 line);
void       TRACE_Entry(const char *pFile, U32 line, const char *pFnName);
void       TRACE_Exit(const char *pFile, U32 line);
void       TRACE_ShowBuff(U8 *pBuf, U32 size);
void       TRACE_HexDump(U8 *pBuf, U32 size);
#endif


#ifdef __cplusplus
    }
#endif

#endif


