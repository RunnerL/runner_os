/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: Copyright (C) 7�ŵ絥��. 2017. All rights reserved.              */
/*                                                                            */
/* �ļ�����: pub_trace.c                                                      */
/* ����ժҪ: TRACE���ܵ�ʵ��                                                  */
/* ����˵��: ��                                                               */
/* ��ǰ�汾: v0.9                                                             */
/* ��    ��: maag                                                             */
/* �������: 2017-11-23                                                       */
/* �޸ļ�¼:                                                                  */
/*     �޸�����          �汾��      �޸���          �޸�����                 */
/* -------------------------------------------------------------------------- */
/*     2017-11-23        v0.9        maag            �����ļ�                 */
/*<FH->************************************************************************/

#if defined(DEBUG) && defined(FT_TRC_EN)
/******************************************************************************/
/*               #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���               */
/******************************************************************************/
#include <string.h>
#include "pub_include.h"


/******************************************************************************/
/*                                �ⲿ��������                                */
/******************************************************************************/


/******************************************************************************/
/*                                 �ڲ��궨��                                 */
/******************************************************************************/


/******************************************************************************/
/*                              �ڲ��������Ͷ���                              */
/******************************************************************************/
/*<STRUCT+>********************************************************************/
/* �ṹ: T_FN_NAME                                                            */
/* ע��: ������ջ�Ľṹ                                                       */
/*<STRUCT->********************************************************************/
typedef struct t_fn_name
{
    char fnName[TRC_MAX_NAME_SIZE];             /* ��������                   */
    char taskName[TRC_MAX_NAME_SIZE];           /* ִ�е�ǰ��������������     */
    struct t_fn_name *pNext;                    /* ָ����һ���������ṹ��ָ�� */

} T_FN_NAME;


/******************************************************************************/
/*                               ȫ��(��̬)����                               */
/******************************************************************************/
T_FN_NAME *gTrcNameStk;                                 /* ������ջ           */
OS_SEM     gTrcFnNameStkMutex        = USR_NULL;        /* ������ջ�ź���     */
OS_SEM     gTrcEvtMutex              = USR_NULL;        /* EVT TRACE�ź���    */
U32        gTrcLevel                 = TRC_CUR_LVL;     /* EVT TRACE�ȼ�      */
U32        gTrcFilterCnts            = 2;               /* �����ļ�����       */
char      *gTrcFilter[TRC_MAX_FILES] = {"tes","coms"};


/******************************************************************************/
/*                                �ڲ�����ԭ��                                */
/******************************************************************************/


/******************************************************************************/
/*                                ȫ�ֺ���ʵ��                                */
/******************************************************************************/
/*<FUNC+>**********************************************************************/
/* ��������: TRACE_Init                                                       */
/* ��������: TRACEģ��ĳ�ʼ��                                                */
/* �������: ��                                                               */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-11-23              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
USR_STATUS TRACE_Init(void)
{
    USR_STATUS res = 0;

    /**************************************************************************/
    /* �������ڲ���������ջ���ź���                                           */
    /**************************************************************************/
    gTrcFnNameStkMutex = OS_SemCreateMutex();
    if (USR_NULL == gTrcFnNameStkMutex)
    {
        gTrcLevel = TRC_LVL_NO;
        res = 1;
        goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* ��ʼ��������ջ                                                         */
    /**************************************************************************/
    gTrcNameStk = USR_NULL;

    /**************************************************************************/
    /* evt trace��ʼ��                                                        */
    /**************************************************************************/
    TRACE_EvtInit();

EXIT_LABEL:
    return res;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_EvtInit                                                    */
/* ��������: ��ʼ��evt trace                                                  */
/* �������: ��                                                               */
/* �������: ��                                                               */
/* �� �� ֵ: USR_STATUS                                                       */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-19              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
USR_STATUS TRACE_EvtInit(void)
{
    USR_STATUS res = 0;
    T_RTC tSystime;

    /**************************************************************************/
    /* ��������evt trace���ź���                                              */
    /**************************************************************************/
    gTrcEvtMutex = OS_SemCreateMutex();
    if (USR_NULL == gTrcEvtMutex)
    {
        gTrcLevel = TRC_LVL_NO;
        res = 1;
        goto EXIT_LABEL;
    }

    if (TRC_LVL_NO != gTrcLevel)
    {
#if TRC_SIO
        BSP_RTCGetDateTime(&tSystime, RTC_FMT_BIN);
        TRC_OUT("ver: 1.00\r\ntracing start at %d-%d-%d %d-%d-%d\r\n",
                tSystime.year, tSystime.month, tSystime.day,
                tSystime.hours, tSystime.minutes, tSystime.seconds);
        TRC_OUT("%-20s %-5s %-12s %-16s %-24s %-10s\r\n",
                "file", "line", "time", "task", "function", "content");
        TRC_OUT("-------------------------------------------------------------"\
                "-------------------------------------------------------------");
#endif
    }

EXIT_LABEL:
    return res;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_GetSelfTaskName                                            */
/* ��������: ��ȡ��ǰ���������                                               */
/* �������: pTaskName --- ָ�����������Ƶ�ָ��                             */
/*           size --- ��������С                                              */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-19              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_GetSelfTaskName(char *pTaskName, U32 size)
{
    char *pName;

    if (USR_NULL != pTaskName)
    {
        pName = pcTaskGetName(0);
        strncpy(pTaskName, pName, TRC_MAX_NAME_SIZE-1);
        pTaskName[TRC_MAX_NAME_SIZE-1] = '\0';
    }

    return;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_GetFnNameInStk                                             */
/* ��������: �ں�����ջ�в��Һ�����                                           */
/* �������: pTaskName --- ��������                                           */
/* �������: pFuncName --- ����������ƥ��ĵ�һ��������                       */
/* �� �� ֵ: S32                                                              */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-19              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
S32 TRACE_GetFnNameInStk(const char *pTaskName, char *pFuncName)
{
    S32        res   = -1;
    T_FN_NAME *pName = USR_NULL;

    /**************************************************************************/
    /* ��������������                                                       */
    /**************************************************************************/
    if (USR_NULL == pFuncName || USR_NULL == pTaskName)
    {
        goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* �ں�����ջ�в��ҵ�һ����������ƥ��ĺ�����                             */
    /**************************************************************************/
    pName = gTrcNameStk;

    while (NULL != pName)
    {
        USR_ASSERT((USR_NULL != pName) && (USR_NULL != pName->taskName));

        //res = strcmp(pTaskName, pName->taskName);
        res = memcmp(pTaskName, pName->taskName, strlen(pTaskName));

        if (0 == res)
        {
            strcpy(pFuncName, pName->fnName);
            break;
        }
        else
        {
            pName = pName->pNext;
        }
    }

    /**************************************************************************/
    /* �����������ջ��û���ҵ���Ӧ�ĺ������������������0����              */
    /**************************************************************************/
    if (0 != res)
    {
        pFuncName[0] = '\0';
    }

EXIT_LABEL:
    return res;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_FileFilter                                                 */
/* ��������: TRACE�ļ����˼�顣                                              */
/* �������: pcFilePath --- �ļ���·��                                        */
/* �������: ��                                                               */
/* �� �� ֵ: 0 - �������������1 - �������������                             */
/* ��������:                                                                  */
/* ����˵��: �������������ʾ�������TRACE��Ϣ��                              */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-11-23              v0.9          maag           ��������          */
/*<FUNC->**********************************************************************/
USR_STATUS TRACE_FileFilter(const char *filePath)
{
    U32 ret = 1;
    U32 cnts;
    const char *fileName;

    /**************************************************************************/
    /* �����ļ���Ϊ0�����ǳ������������ļ�������Ϊ�����ˣ����������������  */
    /**************************************************************************/
    if ((0 == gTrcFilterCnts) || (gTrcFilterCnts > TRC_MAX_FILES))
    {
        ret = 0;
    }
    /**************************************************************************/
    /* �����ļ�����Ϊ0����ôֻ���ڹ����б��е��ļ�����������                  */
    /**************************************************************************/
    else
    {
        fileName = strrchr(filePath, '\\');

        if (0 == fileName)
        {
            fileName = strrchr(filePath, '/');
        }

        if (0 == fileName)
        {
            fileName = filePath;
        }
        else
        {
            fileName++;
        }

        for (cnts=0; cnts<gTrcFilterCnts; cnts++)
        {
            if (0 == strncmp(fileName, gTrcFilter[cnts],
                             strlen(gTrcFilter[cnts])))
            {
                ret = 0;
                break;
            }
        }
    }

    return ret;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_RecordPrefix                                               */
/* ��������: ��¼TRACE��Ϣ��ǰ׺�ַ���                                        */
/* �������: pFile --- ָ���ļ�����ָ��                                       */
/*           line --- ��ǰ�ļ�������                                          */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-21              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_RecordPrefix(const char *pFile, U32 line)
{
    char *pos;
    U32   len;
    char  fileName[32];
    char  fnName[32];
    char  tmp[32];
    T_RTC tSysTime;

    /**************************************************************************/
    /* ��ȡ�ļ���                                                             */
    /**************************************************************************/
    pos = strrchr((char *)pFile, '\\');
    if (USR_NULL == pos)
    {
        pos = strrchr((char *)pFile, '/');
        if (USR_NULL == pos)
        {
            goto EXIT_LABEL;
        }
    }

    len = strlen(pos+1);
    strncpy(fileName, pos+1, len+1);
    fileName[31] = '\0';

    /**************************************************************************/
    /* ��ȡ�������ơ�ϵͳʱ���                                               */
    /**************************************************************************/
    TRACE_GetSelfTaskName(tmp, sizeof(tmp));
    TRACE_GetFnNameInStk(tmp, fnName);

    BSP_RTCGetDateTime(&tSysTime, RTC_FMT_BIN);

#if TRC_SIO
    /**************************************************************************/
    /* ֱ��ͨ���������                                                       */
    /**************************************************************************/
    TRC_OUT("\r\n%-20s %-5d %02d:%02d:%02d.%03d %-16s %-24s ",
            fileName, line,
            tSysTime.hours, tSysTime.minutes, tSysTime.seconds,
            tSysTime.subseconds,
            tmp, fnName);
#endif

EXIT_LABEL:
    return;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_Entry                                                      */
/* ��������: TRACE���ʵ��                                                    */
/* �������: pFile --- ָ���ļ�����ָ��                                       */
/*           line --- �ļ�����                                                */
/*           pFnName --- ָ���������ַ�����ָ��                             */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-21              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_Entry(const char *pFile, U32 line, const char *pFnName)
{
    BOOL res;
    T_FN_NAME *ptList;

    /**************************************************************************/
    /* ����������                                                           */
    /**************************************************************************/
    if ((USR_NULL == pFile) || (USR_NULL == pFnName))
    {
        goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* ��ȡ�ź���                                                             */
    /**************************************************************************/
    res = OS_SemWait(gTrcFnNameStkMutex, WAIT_FOREVER);
    if (!res)
    {
        goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* ��������ѹջ                                                           */
    /**************************************************************************/
    ptList = (T_FN_NAME *)PUB_GetUB(sizeof(T_FN_NAME));
    if (USR_NULL != ptList)
    {
        TRACE_GetSelfTaskName(ptList->taskName, sizeof(ptList->taskName));
        strncpy(ptList->fnName, pFnName, TRC_MAX_NAME_SIZE-1);
        ptList->fnName[TRC_MAX_NAME_SIZE-1] = '\0';
        ptList->pNext = gTrcNameStk;
        gTrcNameStk = ptList;
    }
    else
    {
        gTrcLevel = TRC_LVL_NO;
    }

    /**************************************************************************/
    /* �ͷ��ź���                                                             */
    /**************************************************************************/
    OS_SemRelease(gTrcFnNameStkMutex);

    if ((TRC_LVL_NO != gTrcLevel) && (gTrcLevel <= TRC_LVL_ALL))
    {
        TRACE_RecordPrefix(pFile, line);
#if TRC_SIO
        TRC_OUT("Entry {");
#endif
    }

EXIT_LABEL:
    return;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_Exit                                                       */
/* ��������: TRACE���ڵ�ʵ��                                                  */
/* �������: pFile --- ָ���ļ����Ƶ�ָ��                                     */
/*           line --- �ļ�����                                                */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-21              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_Exit(const char *pFile, U32 line)
{
    BOOL res;
    T_FN_NAME *pName = USR_NULL;
    T_FN_NAME *pList = USR_NULL;
    char       taskName[32];

    /**************************************************************************/
    /* ���TRACE��Ϣ                                                          */
    /**************************************************************************/
    TRACE_RecordPrefix(pFile, line);
#if TRC_SIO
    TRC_OUT("Exit }");
#endif

    /**************************************************************************/
    /* ��ȡ�ź���                                                             */
    /**************************************************************************/
    res = OS_SemWait(gTrcFnNameStkMutex, WAIT_FOREVER);
    if (!res)
    {
        goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* �����������Ʋ��Һ�����ջ�е�һ����������ƥ��ĺ�����                   */
    /**************************************************************************/
    TRACE_GetSelfTaskName(taskName, sizeof(taskName));
    pName = gTrcNameStk;
    pList = gTrcNameStk;

    while (USR_NULL != pName)
    {
        if (0 == strcmp(taskName, pName->taskName))
        {
            break;
        }
        else
        {
            pList = pName;
            pName = pName->pNext;
        }
    }

    /**************************************************************************/
    /* �Ӻ�����ջ��ɾ��������                                                 */
    /**************************************************************************/
    if (USR_NULL != pName)
    {
        if (gTrcNameStk == pName)
        {
            gTrcNameStk = pName->pNext;
        }
        else
        {
            pList->pNext = pName->pNext;
        }

        PUB_RetUB(pName);
        pName = USR_NULL;
    }

    /**************************************************************************/
    /* �ͷ��ź���                                                             */
    /**************************************************************************/
    OS_SemRelease(gTrcFnNameStkMutex);

EXIT_LABEL:
    return;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_ShowBuff                                                   */
/* ��������: TRACE������������ݵĺ���                                        */
/* �������: pBuf --- ָ�򻺳�����ָ��                                        */
/*           size --- ��������С                                              */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-11-23              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_ShowBuff(U8 *pBuf, U32 size)
{
    unsigned int  cnts;
    unsigned char data;

    if ((0 != pBuf) && (0 != size))
    {
        for (cnts=0; cnts<size; cnts++)
        {
            if (0 == (cnts % 8))
            {
                m_printf(DBG_COM, "\r\n%82s", " ");
            }

            data = pBuf[cnts];
            m_printf(DBG_COM, "0x%02x ", data);
        }
    }

    return;
}

/*<FUNC+>**********************************************************************/
/* ��������: TRACE_HexDump                                                    */
/* ��������: ��ָ����ʽ���16���Ƶ��ڴ�����                                   */
/* �������: pBuf --- ָ�򻺳�����ָ��                                        */
/*           size --- ��������С                                              */
/* �������: ��                                                               */
/* �� �� ֵ: void                                                             */
/* ��������:                                                                  */
/* ����˵��: ��                                                               */
/* �޸ļ�¼:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-11-23              v0.9          maag          ��������           */
/*<FUNC->**********************************************************************/
void TRACE_HexDump(U8 *pBuf, U32 size)
{
    const unsigned char *c = pBuf;
    unsigned int i;

    if (USR_NULL == pBuf)
    {
        goto EXIT_LABEL;
    }

    m_printf(DBG_COM, "\r\n%82s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                      "~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", " ");
    m_printf(DBG_COM, "%82sOFFSET     0  1  2  3  4  5  6  7  8  9  A  B  C  D"
                      "  E  F  ASCII\r\n", " ");
    m_printf(DBG_COM, "%82s---------------------------------------------------"
                      "------------------------\r\n", " ");

    while (size > 0)
    {
        m_printf(DBG_COM, "%82s%08ph: ", " ", c);

        for (i=0; i<16; i++)
        {
            if (i < size)
            {
                m_printf(DBG_COM, "%02x ", c[i]);
            }
            else
            {
                m_printf(DBG_COM, "   ");
            }
        }

        for (i=0; i<16; i++)
        {
            if (i<size)
            {
                m_printf(DBG_COM, "%c", (c[i]>=32 && c[i] < 127) ? c[i] : '.');
            }
            else
            {
                m_printf(DBG_COM, " ");
            }
        }

        m_printf(DBG_COM, "\r\n");

        c += 16;

        if (size <= 16)
        {
            break;
        }

        size -= 16;
    }

    m_printf(DBG_COM, "%82s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                      "~~~~~~~~~~~~~~~~~~~~~~~~", " ");

EXIT_LABEL:
    return;
}


/******************************************************************************/
/*                                �ڲ�����ʵ��                                */
/******************************************************************************/

#endif

