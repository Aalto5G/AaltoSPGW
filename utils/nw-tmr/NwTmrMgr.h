/*----------------------------------------------------------------------------*
 *                                                                            *
 * Copyright (c) 2010-2011 Amit Chawre                                        *
 * All rights reserved.                                                       *
 *                                                                            *
 * Redistribution and use in source and binary forms, with or without         *
 * modification, are permitted provided that the following conditions         *
 * are met:                                                                   *
 *                                                                            *
 * 1. Redistributions of source code must retain the above copyright          *
 *    notice, this list of conditions and the following disclaimer.           *
 * 2. Redistributions in binary form must reproduce the above copyright       *
 *    notice, this list of conditions and the following disclaimer in the     *
 *    documentation and/or other materials provided with the distribution.    *
 * 3. The name of the author may not be used to endorse or promote products   *
 *    derived from this software without specific prior written permission.   *
 *                                                                            *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR       *
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES  *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.    *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,           *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT   *
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF   *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          *
 *----------------------------------------------------------------------------*/

/** 
 * @file NwTmr.h
 * @brief 
*/

#include <stdio.h>

#include "NwEvt.h"
#include "NwTypes.h"
#include "NwError.h"
#include "NwLog.h"

#ifndef __NW_TMR_MGR_H__
#define __NW_TMR_MGR_H__


typedef NwPtrT NwTimerHandleT;
typedef NwPtrT NwTimerMgrHandleT;

#ifdef __cplusplus
extern "C" {
#endif

#define NW_TIMER_TYPE_ONE_SHOT                          (0)
#define NW_TIMER_TYPE_REPETITIVE                        (1)

extern NwRcT 
nwTmrMgrInitialize( NwTimerMgrHandleT *tmrMgrHandle);

extern NwRcT 
nwTmrMgrFinalize( NwTimerMgrHandleT tmrMgrHandle);

extern NwRcT 
nwTmrMgrStartTimer( NwTimerMgrHandleT tmrMgrHandle,
    NwU32T timeoutSec,
    NwU32T timeoutUsec,
    NwU32T tmrType,
    void   NW_TMR_CALLBACK((*nwTimerTimeout)),
    void*  timeoutArg,
    NwTimerHandleT* phTmr);

extern NwRcT 
nwTmrMgrStopTimer( NwTimerMgrHandleT tmrMgrHandle,
    NwTimerHandleT hTmr);

#ifdef __cplusplus
}
#endif

#endif
