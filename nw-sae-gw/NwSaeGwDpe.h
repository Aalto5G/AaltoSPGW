/*----------------------------------------------------------------------------*
 *                                                                            *
 *                                n w - e p c                                 *
 *       L T E / S A E        S E R V I N G / P D N       G A T E W A Y       *
 *                                                                            *
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
 * @file nwSaeGwDpe.h
 */

#ifndef __NW_SAEGW_DPE_H__
#define __NW_SAEGW_DPE_H__

#include <stdio.h>
#include <assert.h>

#include "NwEvt.h"
#include "NwLog.h"
#include "NwTypes.h"
#include "NwUtils.h"
#include "NwSdp.h"
#include "NwIpv4If.h"
#include "NwGtpv1uIf.h"

typedef struct {
  NwSdpHandleT  hSdp;
  NwIpv4IfT     ipv4If;
  NwGtpv1uIfT   gtpuIf;
  NwEventT      evGtpuIf;
  NwEventT      evIpv4;
  NwSdpServiceHandleT hGtpu;
  NwSdpServiceHandleT hIpv4;
} NwSaeGwDpeT;

#ifdef __cplusplus
extern "C" {
#endif

extern NwSaeGwDpeT*
nwSaeGwDpeInitialize();

extern NwRcT
nwSaeGwDpeDestroy(NwSaeGwDpeT* thiz);

NwRcT
nwSaeGwDpeGetIpv4Addr(NwSaeGwDpeT* thiz, NwU32T  *pIpv4Addr);

extern NwRcT
nwSaeGwDpeCreateGtpuService( NwSaeGwDpeT* thiz, NwU32T ipv4Addr );

extern NwRcT
nwSaeGwDpeCreateIpv4Service( NwSaeGwDpeT* thiz, NwU8T* nwIfName);

/**
 * Create Gtpu to Ipv4 flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeCreateGtpuIpv4Flow(NwSaeGwDpeT*   thiz,
                         NwSdpUlpSessionHandleT         hSession,
                         NwU32T         teidIngress,
                         NwU32T         *pTeidIngress,
                         NwU32T         *pIpv4Ingress,
                         NwSdpSessionHandleT         *phBearer);

/**
 * Create Ipv4 to Gtpu flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeCreateIpv4GtpuFlow(NwSaeGwDpeT*   thiz,
                         NwSdpUlpSessionHandleT         hSession,
                         NwU32T         teidEgress,
                         NwU32T         ipv4Egress,
                         NwU32T         ipv4Ingress,
                         NwSdpSessionHandleT         *phBearer);

/**
 * Create Ipv4 to Gtpu flow with Soft Data Plane
 */

NwRcT
nwSaeGwDpeModifyIpv4GtpuFlow(NwSaeGwDpeT*   thiz,
                             NwSdpUlpSessionHandleT      hSession,
                             NwU32T         teidEgress,
                             NwU32T         ipv4Egress,
                             NwSdpSessionHandleT         hBearer);

/**
 * Create Gtpu to Gtpu flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeCreateGtpuGtpuFlow(NwSaeGwDpeT*   thiz,
                         NwSdpUlpSessionHandleT         hSession,
                         NwU32T         teidIngress,
                         NwU32T         teidEgress,
                         NwU32T         ipv4Egress,
                         NwU32T         *pTeidIngress,
                         NwU32T         *pIpv4Ingress,
                         NwSdpSessionHandleT         *phBearer);

/**
 * Modify Gtpu to Gtpu flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeModifyGtpuGtpuFlow(NwSaeGwDpeT*   thiz,
                             NwSdpUlpSessionHandleT         hSession,
                             NwU32T         teidEgress,
                             NwU32T         ipv4Egress,
                             NwSdpSessionHandleT         hBearer);

/**
 * Release Gtpu Endpoint flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeReleaseEndpointFlow(NwSaeGwDpeT*   thiz,
                              NwSdpUlpSessionHandleT         hSession,
                              NwSdpSessionHandleT         hBearer);

/**
 * Destroy a flow with Soft Data Plane
 */

extern NwRcT
nwSaeGwDpeDestroyFlow(NwSaeGwDpeT*   thiz,
                      NwSdpUlpSessionHandleT      hSession,
                      NwSdpSessionHandleT         hBearer);
#ifdef __cplusplus
}
#endif

#endif
