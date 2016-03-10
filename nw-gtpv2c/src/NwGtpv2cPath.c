/*----------------------------------------------------------------------------*
 *                                                                            *
 *                              n w - g t p v 2 c                             *
 *    G P R S   T u n n e l i n g    P r o t o c o l   v 2 c    S t a c k     *
 *                                                                            *
 *                                                                            *
 * Copyright (c) 2010-2016 Vicent Ferrer Guasch                               *
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

#include <stdio.h>
#include <string.h>

#include "NwTypes.h"
#include "NwLog.h"
#include "NwUtils.h"
#include "NwGtpv2cLog.h"
#include "NwGtpv2c.h"
#include "NwGtpv2cPrivate.h"
#include "NwGtpv2cPath.h"

/*--------------------------------------------------------------------------*
 *                 P R I V A T E  D E C L A R A T I O N S                   *
 *--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

static NwGtpv2cPathT* gpGtpv2cPathPool = NULL;

/*--------------------------------------------------------------------------*
 *                   P R I V A T E      F U N C T I O N S                   *
 *--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * Send msg retransmission to peer via data request to UDP Entity
 *--------------------------------------------------------------------------*/

/* static NwRcT */
/* nwGtpv2cTrxnSendMsgRetransmission(NwGtpv2cTrxnT* thiz) */
/* { */
/*   NwRcT rc ; */

/*   NW_ASSERT(thiz); */

/*   NW_ASSERT(thiz->pMsg); */

/*   rc = thiz->pStack->udp.udpDataReqCallback(thiz->pStack->udp.hUdp, */
/*       thiz->pMsg->msgBuf, */
/*       thiz->pMsg->msgLen, */
/*       thiz->peerIp, */
/*       thiz->peerPort); */

/*   thiz->maxRetries--; */

/*   return rc; */
/* } */

/* static NwRcT */
/* nwGtpv2cTrxnPeerRspWaitTimeout(void* arg) */
/* { */
/*   NwRcT rc = NW_OK; */
/*   NwGtpv2cTrxnT* thiz; */
/*   NwGtpv2cStackT* pStack; */

/*   thiz = ((NwGtpv2cTrxnT*)arg); */
/*   pStack = thiz->pStack; */

/*   NW_ASSERT(pStack); */

/*   NW_LOG(pStack, NW_LOG_LEVEL_WARN, "T3 Response timer expired for transaction 0x%X", (NwU32T) thiz); */

/*   thiz->hRspTmr = 0; */

/*   if(thiz->maxRetries) */
/*   { */
/*     rc = nwGtpv2cTrxnSendMsgRetransmission(thiz); */
/*     NW_ASSERT(NW_OK == rc); */

/*     rc = nwGtpv2cStartTimer(thiz->pStack, thiz->t3Timer, 0, NW_GTPV2C_TMR_TYPE_ONE_SHOT, nwGtpv2cTrxnPeerRspWaitTimeout, thiz, &thiz->hRspTmr); */
/*   } */
/*   else */
/*   { */
/*     NwGtpv2cUlpApiT ulpApi; */

/*     ulpApi.hMsg                              = 0; */
/*     ulpApi.apiType                           = NW_GTPV2C_ULP_API_RSP_FAILURE_IND; */

/*     ulpApi.apiInfo.rspFailureInfo.hUlpTrxn   = thiz->hUlpTrxn; */
/*     ulpApi.apiInfo.rspFailureInfo.hUlpTunnel = ( (thiz->hTunnel) ? ((NwGtpv2cTunnelT*)(thiz->hTunnel))->hUlpTunnel : 0 ); */

/*     NW_LOG(pStack, NW_LOG_LEVEL_ERRO, "N3 retries expired for transaction 0x%X", (NwU32T) thiz); */
/*     RB_REMOVE(NwGtpv2cOutstandingTxSeqNumTrxnMap, &(pStack->outstandingTxSeqNumMap), thiz); */
/*     rc = nwGtpv2cTrxnDelete(&thiz); */

/*     rc = pStack->ulp.ulpReqCallback(pStack->ulp.hUlp, &ulpApi); */
/*   } */
/*   return rc; */
/* } */

/* static NwRcT */
/* nwGtpv2cTrxnDuplicateRequestWaitTimeout(void* arg) */
/* { */
/*   NwRcT rc = NW_OK; */
/*   NwGtpv2cTrxnT* thiz; */
/*   NwGtpv2cStackT* pStack; */

/*   thiz = ((NwGtpv2cTrxnT*)arg); */

/*   NW_ASSERT(thiz); */
/*   pStack = thiz->pStack; */

/*   NW_ASSERT(pStack); */

/*   NW_LOG(pStack, NW_LOG_LEVEL_DEBG, "Duplicate request hold timer expired for transaction 0x%X", (NwU32T) thiz); */

/*   thiz->hRspTmr = 0; */

/*   RB_REMOVE(NwGtpv2cOutstandingRxSeqNumTrxnMap, &(pStack->outstandingRxSeqNumMap), thiz); */
/*   rc = nwGtpv2cTrxnDelete(&thiz); */
/*   NW_ASSERT(NW_OK == rc); */

/*   return rc; */
/* } */

/* /\** */
/*  * Start timer to wait for rsp of a req message */
/*  * */
/*  * @param[in] thiz : Pointer to transaction */
/*  * @param[in] timeoutCallbackFunc : Timeout handler callback function. */
/*  * @return NW_OK on success. */
/*  *\/ */

/* NwRcT */
/* nwGtpv2cTrxnStartPeerRspWaitTimer(NwGtpv2cTrxnT* thiz) */
/* { */
/*   NwRcT rc; */
/*   rc = nwGtpv2cStartTimer(thiz->pStack, thiz->t3Timer, 0, NW_GTPV2C_TMR_TYPE_ONE_SHOT, nwGtpv2cTrxnPeerRspWaitTimeout, thiz, &thiz->hRspTmr); */
/*   return rc; */
/* } */

/* /\** */
/*   Start timer to wait before pruginf a req tran for which response has been sent */

/*   @param[in] thiz : Pointer to transaction */
/*   @return NW_OK on success. */
/*  *\/ */

/* NwRcT */
/* nwGtpv2cTrxnStartDulpicateRequestWaitTimer(NwGtpv2cTrxnT* thiz) */
/* { */
/*   NwRcT rc; */

/*   rc = nwGtpv2cStartTimer(thiz->pStack, thiz->t3Timer * thiz->maxRetries, 0, NW_GTPV2C_TMR_TYPE_ONE_SHOT, nwGtpv2cTrxnDuplicateRequestWaitTimeout, thiz, &thiz->hRspTmr); */

/*   return rc; */
/* } */

/* /\** */
/*   Send timer stop request to TmrMgr Entity. */

/*   @param[in] thiz : Pointer to transaction */
/*   @return NW_OK on success. */
/*  *\/ */

/* static NwRcT */
/* nwGtpv2cTrxnStopPeerRspTimer(NwGtpv2cTrxnT* thiz) */
/* { */
/*   NwRcT rc; */

/*   NW_ASSERT(thiz->pStack->tmrMgr.tmrStopCallback != NULL); */

/*   rc = nwGtpv2cStopTimer(thiz->pStack, thiz->hRspTmr); */
/*   thiz->hRspTmr = 0; */

/*   return rc; */
/* } */


/**
  Send Echo request periodically as a keep alive.

  @param[in] thiz : Pointer to path
  @return NW_OK on success.
 */
static NwRcT
nwGtpv2cPathPeriodicEchoReq(NwGtpv2cPathT* thiz)
{
  NwRcT rc = NW_OK;;
  NW_LOG(thiz->pStack, NW_LOG_LEVEL_INFO, "Test Periodic echo to "NW_IPV4_ADDR,
         NW_IPV4_ADDR_FORMAT(thiz->ipv4Address));
  return rc;
}

/*--------------------------------------------------------------------------*
 *                      P U B L I C    F U N C T I O N S                    *
 *--------------------------------------------------------------------------*/

/**
 * Constructor
 *
 * @param[in] thiz : Pointer to stack
 * @param[out] ppTrxn : Pointer to pointer to Trxn object.
 * @return NW_OK on success.
 */
NwGtpv2cPathT*
nwGtpv2cPathNew( NW_IN  NwGtpv2cStackT* pStack,
                 NW_IN  NwU32T ipv4Remote)
{
  NwRcT rc = NW_OK;
  NwGtpv2cPathT *thiz;

  if(gpGtpv2cPathPool)
  {
    thiz = gpGtpv2cPathPool;
    gpGtpv2cPathPool = gpGtpv2cPathPool->next;
  }
  else
  {
    NW_GTPV2C_MALLOC(pStack, sizeof(NwGtpv2cPathT), thiz, NwGtpv2cPathT*);
  }

  if (thiz)
  {
    thiz->pStack           = pStack;
    thiz->ipv4Address      = ipv4Remote;
    thiz->restartCounter   = 0;
    thiz->t3ResponseTimout = 2;
    thiz->n3RequestCount   = 2;

    rc = nwGtpv2cStartTimer(pStack, 3*60, 0, NW_GTPV2C_TMR_TYPE_REPETITIVE,
                            nwGtpv2cPathPeriodicEchoReq, thiz,
                            &thiz->hKeepAliveTmr);

    RB_INSERT(NwGtpv2cPathMap, &(pStack->pathMap), thiz);
  }
  else
  {
    rc = NW_FAILURE;
  }

  NW_LOG(pStack, NW_LOG_LEVEL_DEBG, "Created Path 0x%X", (NwU32T) thiz->ipv4Address);
  return thiz;
}

/**
 * Destructor
 *
 * @param[out] pthiz : Pointer to pointer to Trxn object.
 * @return NW_OK on success.
 */
NwRcT
nwGtpv2cPathDelete( NW_INOUT NwGtpv2cPathT **pthiz)
{
  NwRcT rc = NW_OK;
  NwGtpv2cStackT* pStack;
  NwGtpv2cPathT *thiz = *pthiz;

  pStack = thiz->pStack;

  if(thiz->hKeepAliveTmr)
  {
    NW_ASSERT(pStack->tmrMgr.tmrStopCallback != NULL);

    rc = nwGtpv2cStopTimer(thiz->pStack, thiz->hKeepAliveTmr);
    thiz->hKeepAliveTmr = 0;
  }

  NW_LOG(pStack, NW_LOG_LEVEL_DEBG, "Purging path ", NW_IPV4_ADDR,
         NW_IPV4_ADDR_FORMAT(thiz->ipv4Address));
  thiz->next = gpGtpv2cPathPool;
  gpGtpv2cPathPool = thiz;

  *pthiz = NULL;
  return rc;
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*
 *                          E N D   O F   F I L E                           *
 *--------------------------------------------------------------------------*/
