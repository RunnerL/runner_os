/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: Copyright (C) 7�ŵ絥��. 2017. All rights reserved.              */
/*                                                                            */
/* �ļ�����: pub_trace.h                                                      */
/* ����ժҪ: TRACEģ���ͷ�ļ�                                                */
/* ����˵��: ��                                                               */
/* ��ǰ�汾: v0.9                                                             */
/* ��    ��: maag                                                             */
/* �������: 2017-11-23                                                       */
/* �޸ļ�¼:                                                                  */
/*     �޸�����          �汾��      �޸���          �޸�����                 */
/* -------------------------------------------------------------------------- */
/*     2017-11-23        v0.9        maag            �����ļ�                 */
/*<FH->************************************************************************/
#ifndef PUB_TRACE_H
#define PUB_TRACE_H


/******************************************************************************/
/*               #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���               */
/******************************************************************************/


/******************************************************************************/
/*                              ������������ѡ��                              */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                                  ��������                                  */
/******************************************************************************/
#if defined (DEBUG) && defined (FT_TRC_EN)

#define TRC_MAX_FILES           5               /* �ɹ��˵�����ļ���Ŀ       */
#define TRC_MAX_NAME_SIZE       32              /* �����ַ�������󳤶�       */
#define TRC_SIO                 1               /* ֱ�Ӵ�������Ŀ���         */

/******************************************************************************/
/* TRACE�ȼ�����                                                              */
/******************************************************************************/
#define TRC_LVL_ALL             1               /* �������TRACE��Ϣ          */
#define TRC_LVL_FLOW            2               /* �������֧������Ϣ         */
#define TRC_LVL_DETAIL          3               /* �����ϸ������Ϣ           */
#define TRC_LVL_ABNORMAL        4               /* ����쳣��Ϣ(ϵͳ������Ĵ�*/
#define TRC_LVL_ERROR           5               /* ���������Ϣ(�ܵ���ϵͳ����*/
#define TRC_LVL_NO              6               /* ��TRACE��Ϣ                */

#define TRC_CUR_LVL             TRC_LVL_ALL     /* ��ǰTRACE�ȼ�              */

#endif


/******************************************************************************/
/*                                 ȫ�ֺ궨��                                 */
/******************************************************************************/
#ifdef DEBUG
#define SYS_OUT(STR, ...)       m_printf(DBG_COM, (STR), ##__VA_ARGS__)
#else
#define SYS_OUT(STR, ...)       ((void)0U)
#endif

#if defined (DEBUG) && defined (FT_TRC_EN)
/******************************************************************************/
/* TRACE����궨��                                                            */
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
/* TRACE�ӿں궨��                                                            */
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
/*                              ȫ���������Ͷ���                              */
/******************************************************************************/


/******************************************************************************/
/*                                ȫ�ֱ�������                                */
/******************************************************************************/
#if defined (DEBUG) && defined (FT_TRC_EN)
extern OS_SEM gTrcEvtMutex;
extern U32    gTrcLevel;
#endif


/******************************************************************************/
/*                                 ȫ�ֺ���ԭ��                               */
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


