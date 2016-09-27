/*----------------------------------------------------------------------------*
 *                                                                            *
 *                              n w - g t p v 2 c                             *
 *    G P R S   T u n n e l i n g    P r o t o c o l   v 2 c    S t a c k     *
 *                                                                            *
 *                                                                            *
 * Copyright (c) 2010-2016 Vicent Ferrer Guasch                               *
 * Copyright (c) 2010-2016 Jesus Llorente Santos                              *
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
 * @file NwSaeGwUeStateAwaitMmeDownlinkDataNt.c
 */


#include <stdio.h>
#include <assert.h>

#include "NwTypes.h"
#include "NwError.h"
#include "NwMem.h"
#include "NwUtils.h"
#include "NwLog.h"
#include "NwLogMgr.h"
#include "NwSaeGwUeLog.h"
#include "NwSaeGwUeState.h"
#include "NwGtpv2cIe.h"
#include "NwGtpv2cMsg.h"
#include "NwGtpv2cMsgParser.h"
#include "NwSaeGwUlp.h"

#ifdef __cplusplus
extern "C" {
#endif


static NwRcT
nwSaeGwStateAwaitMmeDownlinkDataNtEntryAction(NwSaeGwUeT* thiz, NwSaeGwUeEventInfoT* pEv)
{
  NwRcT rc;
  NwGtpv2cUlpApiT       ulpReq;
  NwGtpv2cUlpApiT       *pUlpApi = pEv->arg;

  NW_UE_LOG(NW_LOG_LEVEL_DEBG, "Sending Downlink Data Notification to MME");
  /*
   * Send Message Request to Gtpv2c Stack Instance
   */
  ulpReq.apiType = NW_GTPV2C_ULP_API_INITIAL_REQ;

  ulpReq.apiInfo.initialReqInfo.hTunnel  = thiz->s11cTunnel.hSgwLocalTunnel;
  ulpReq.apiInfo.initialReqInfo.hUlpTrxn = (NwGtpv2cUlpTrxnHandleT)pUlpApi->apiInfo.initialReqIndInfo.hTrxn;
  ulpReq.apiInfo.initialReqInfo.peerIp   = htonl(thiz->s11cTunnel.fteidMme.ipv4Addr);

  rc = nwGtpv2cMsgNew( thiz->hGtpv2cStackSgwS11,
                       NW_TRUE,
                       NW_GTP_DOWNLINK_DATA_NOTIFICATION ,
                       thiz->s11cTunnel.fteidMme.teidOrGreKey,
                       0,
                       &(ulpReq.hMsg));

  NW_ASSERT( NW_OK == rc );

  rc = nwGtpv2cMsgAddIeTV1((ulpReq.hMsg), NW_GTPV2C_IE_EBI, 0, 5);
  NW_ASSERT( NW_OK == rc );

  NwU8T arp = 0 |
      (0<<6)&0x40   | /*PCI (Pre-emption Capability)*/
      (15<<2)&0x3C  | /*PL  (Priority Level):)*/
      0&0x01;         /*PVI (Pre-emption Vulnerability)*/

  rc = nwGtpv2cMsgAddIe((ulpReq.hMsg),
                        NW_GTPV2C_IE_ALLOCATION_RETENTION_PRIORITY,
                        1,
                        0,
                        &arp);
  NW_ASSERT( NW_OK == rc );

  rc = nwGtpv2cProcessUlpReq(thiz->hGtpv2cStackSgwS11, &ulpReq);
  NW_ASSERT( NW_OK == rc );

  return NW_OK;
}


static NwRcT
nwSaeGwUeHandleSaeS1uDataNotification(NwSaeGwUeT* thiz, NwSaeGwUeEventInfoT* pEv)
{
  NwRcT              rc = NW_OK;
  NwSdpUlpApiT       *pUlpApi = pEv->arg;
  NwGtpv2cUlpApiT    ulpReq;

  NW_UE_LOG(NW_LOG_LEVEL_DEBG, "Data Notification received from GTP-U Stack!");

  return rc;
}

static NwRcT
nwSaeGwUeHandleSaeS11DownlinkDataNotificationAck(NwSaeGwUeT* thiz, NwSaeGwUeEventInfoT* pEv)
{
  NwRcT              rc = NW_OK;
  NwSdpUlpApiT       *pUlpApi = pEv->arg;
  NwGtpv2cUlpApiT    ulpReq;

  NW_UE_LOG(NW_LOG_LEVEL_DEBG, "Downlink Data Notification Ack received from MME!");
  if(thiz->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SGW)
  {
    thiz->state = NW_SAE_GW_UE_STATE_SGW_SESSION_CREATED;
  }
  else
  {
    thiz->state = NW_SAE_GW_UE_STATE_SAE_SESSION_CREATED;
  }
  return rc;
}

static NwRcT
nwSaeGwUeHandleLLError(NwSaeGwUeT* thiz, NwSaeGwUeEventInfoT* pEv)
{
  NwRcT                 rc = NW_OK;
  NwGtpv2cUlpApiT       *pUlpApi = pEv->arg;

  /* TODO */
  return rc;
}

/**
 * Constructor
 */

NwSaeUeStateT*
nwSaeGwStateAwaitMmeDownlinkDataNtNew()
{
  NwRcT rc;
  NwSaeUeStateT* thiz = nwSaeGwStateNew();

  rc = nwSaeGwStateSetEntryAction(thiz,
      nwSaeGwStateAwaitMmeDownlinkDataNtEntryAction);
  NW_ASSERT(NW_OK == rc);

  rc = nwSaeGwStateSetEventHandler(thiz,
      NW_SAE_GW_UE_EVENT_SGW_GTPU_S1_DATA_NOT,
      nwSaeGwUeHandleSaeS1uDataNotification);
  NW_ASSERT(NW_OK == rc);

  rc = nwSaeGwStateSetEventHandler(thiz,
      NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_DOWNLINK_DATA_NOTIFICATION_ACK,
      nwSaeGwUeHandleSaeS11DownlinkDataNotificationAck);
  NW_ASSERT(NW_OK == rc);

  rc = nwSaeGwStateSetEventHandler(thiz,
      NW_SAE_GW_UE_EVENT_LOW_LAYER_ERROR,
      nwSaeGwUeHandleLLError);
  NW_ASSERT(NW_OK == rc);

  return thiz;
}

/**
 * Destructor
 */

NwRcT
nwSaeGwStateAwaitMmeDownlinkDataNtDelete(NwSaeUeStateT* thiz);


#ifdef __cplusplus
}
#endif
