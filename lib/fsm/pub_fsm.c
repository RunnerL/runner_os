/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: Copyright (C) 7号电单车. 2017. All rights reserved.              */
/*                                                                            */
/* 文件名称: pub_fsm.c                                                        */
/* 内容摘要: 一种有限状态机的实现                                             */
/* 其它说明: 无                                                               */
/* 当前版本: v0.9                                                             */
/* 作    者: maag                                                             */
/* 完成日期: 2017-12-1                                                        */
/* 修改记录:                                                                  */
/*     修改日期          版本号      修改人          修改内容                 */
/* -------------------------------------------------------------------------- */
/*     2017-12-1         v0.9        maag            创建文件                 */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/
#include "pub_typedefs.h"
#include "pub_struct.h"
#include "pub_fsm.h"


/******************************************************************************/
/*                                外部引用声明                                */
/******************************************************************************/


/******************************************************************************/
/*                                 内部宏定义                                 */
/******************************************************************************/


/******************************************************************************/
/*                              内部数据类型定义                              */
/******************************************************************************/


/******************************************************************************/
/*                               全局(静态)变量                               */
/******************************************************************************/


/******************************************************************************/
/*                                内部函数原型                                */
/******************************************************************************/


/******************************************************************************/
/*                                全局函数实现                                */
/******************************************************************************/
/*<FUNC+>**********************************************************************/
/* 函数名称: PUB_FsmGetOperType                                               */
/* 功能描述: 提取消息中的消息码作为操作类型                                   */
/* 输入参数: ptMsg --- 指向消息的指针                                         */
/* 输出参数: 无                                                               */
/* 返 回 值: U16                                                              */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-1               v0.9          maag          创建函数           */
/*<FUNC->**********************************************************************/
U16 PUB_FsmGetOperType(void *ptMsg)
{
    U16    opType   = FSM_OPER_TYPE_UNKNOWN;
    T_MSG *ptRcvMsg = USR_NULL;

    ptRcvMsg = (T_MSG *)ptMsg;

    if (USR_NULL != ptRcvMsg)
    {
        opType = ptRcvMsg->code;
    }

    return opType;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: PUB_FsmExecute                                                   */
/* 功能描述: 状态机的执行函数                                                 */
/* 输入参数: ptMsg --- 指向收到的消息的指针                                   */
/*           ptCurStat --- 当前的状态机描述符                                 */
/* 输出参数: 无                                                               */
/* 返 回 值: void                                                             */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-1               v0.9          maag          创建函数           */
/*<FUNC->**********************************************************************/
void PUB_FsmExecute(void *ptMsg, T_FSM_DESC **ptCurStat)
{
    U32  cnts         = 0;
    U16  operType     = FSM_OPER_TYPE_UNKNOWN;
    U16  nextOperType = 0;
    BOOL isUpdateStat = USR_FALSE;

    if ((USR_NULL != ptMsg)
     && (USR_NULL != ptCurStat) && (USR_NULL != *ptCurStat))
    {
        /**********************************************************************/
        /* 获取操作类型                                                       */
        /**********************************************************************/
        operType = PUB_FsmGetOperType(ptMsg);

        /**********************************************************************/
        /* 根据操作类型，找到匹配性，从而获取其索引值                         */
        /**********************************************************************/
        do
        {
            if (operType == (*ptCurStat)[cnts].operType)
            {
                break;
            }
            else
            {
                cnts++;
                nextOperType = (*ptCurStat)[cnts].operType;
            }

        } while (FSM_OPER_TYPE_UNKNOWN != nextOperType);

        /**********************************************************************/
        /* 检查是否要更新当前状态机的状态值                                   */
        /**********************************************************************/
        if ((SAME != (*ptCurStat)[cnts].nextState)
         && (UNK  != (*ptCurStat)[cnts].nextState))
        {
            isUpdateStat = USR_TRUE;
        }

        /**********************************************************************/
        /* 执行状态机的处理函数                                               */
        /**********************************************************************/
        if (USR_NULL != (*ptCurStat)[cnts].pfFunc)
        {
            ((*ptCurStat)[cnts].pfFunc)(ptMsg);
        }

        /**********************************************************************/
        /* 更新状态机                                                         */
        /**********************************************************************/
        if (USR_TRUE == isUpdateStat)
        {
            *ptCurStat = (*ptCurStat)[cnts].nextState;
        }
    }
    else
    {
        while (1);
    }

    return;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: PUB_FsmSetNextState                                              */
/* 功能描述: 设置状态机切换到下一个状态                                       */
/* 输入参数: ptCurState --- 当前状态                                          */
/*           ptNextState --- 下一个状态                                       */
/* 输出参数: 无                                                               */
/* 返 回 值: void                                                             */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*     2017-12-1               v0.9          maag          创建函数           */
/*<FUNC->**********************************************************************/
void PUB_FsmSetNextState(T_FSM_DESC **ptCurState, T_FSM_DESC *ptNextState)
{
    if ((USR_NULL != ptCurState) && (USR_NULL != *ptCurState)
     && (USR_NULL != ptNextState))
    {
        *ptCurState = ptNextState;
    }

    return;
}


/******************************************************************************/
/*                                内部函数实现                                */
/******************************************************************************/


