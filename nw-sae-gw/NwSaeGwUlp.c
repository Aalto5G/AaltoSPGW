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
 * @file NwSaeGwUlp.c
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "NwEvt.h"
#include "NwLog.h"
#include "NwMem.h"
#include "NwUtils.h"
#include "NwTmrMgr.h"
#include "NwSaeGwLog.h"
#include "NwLogMgr.h"
#include "NwGtpv2c.h"
#include "NwGtpv2cMsg.h"
#include "NwSaeGwUe.h"
#include "NwSaeGwUlp.h"
#include "NwGtpv2cIf.h"
#include "NwSdp.h"
#include "NwGtpv2cMsg.h"


#ifdef __cplusplus
extern "C" {
#endif

static NwSaeGwUeEventT
gSgwS11GtpcMsgToUeEventMap[] =
{
  /* 0 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 5 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 10 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 15 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 20 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 25 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 30 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 32 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_CREATE_SESSION_REQ,
  /* 33 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* 34 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_MODIFY_BEARER_REQ,

  /* 35 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* 36 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_DELETE_SESSION_REQ,
  /* 37 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_DELETE_SESSION_RSP,
  /* 38 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 40 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 45 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 50 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 55 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 60 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 65 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 70 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 75 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 80 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 85 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 90 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 95 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 100 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 105 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 110 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 115 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 120 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 125 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 130 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 135 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 140 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 145 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 150 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 155 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 160 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 165 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 170 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_RELEASE_ACCESS_BEARERS_REQ,
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_RELEASE_ACCESS_BEARERS_RSP,
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 175 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S11_DOWNLINK_DATA_NOTIFICATION_ACK,
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 180 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 185 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 190 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 195 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 200 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 205 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 210 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 215 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 220 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 225 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 230 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 235 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 240 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 245 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 250 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 255 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* End.. phewww !! */
};

static NwSaeGwUeEventT
gSgwS5GtpcMsgToUeEventMap[] =
{
  /* 0 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 5 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 10 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 15 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 20 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 25 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 30 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 33 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S5_CREATE_SESSION_RSP, NW_SAE_GW_UE_EVENT_NULL,

  /* 35 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S5_MODIFY_BEARER_RSP,
  /* 36 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S5_DELETE_SESSION_REQ,
  /* 37 */
  NW_SAE_GW_UE_EVENT_SGW_GTPC_S5_DELETE_SESSION_RSP,
  /* 38 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 40 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 45 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 50 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 55 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 60 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 65 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 70 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 75 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 80 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 85 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 90 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 95 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 100 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 105 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 110 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 115 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 120 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 125 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 130 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 135 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 140 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 145 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 150 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 155 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 160 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 165 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 170 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 175 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 180 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 185 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 190 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 195 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 200 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 205 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 210 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 215 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 220 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 225 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 230 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 235 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 240 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 245 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 250 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 255 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* End.. phewww !! */
};

static NwSaeGwUeEventT
gPgwS5GtpcMsgToUeEventMap[] =
{
  /* 0 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 5 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 10 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 15 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 20 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 25 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 30 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,
  /* 32 */
  NW_SAE_GW_UE_EVENT_PGW_GTPC_S5_CREATE_SESSION_REQ,
  /* 33 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* 34 */
  NW_SAE_GW_UE_EVENT_PGW_GTPC_S5_MODIFY_BEARER_REQ,

  /* 35 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* 36 */
  NW_SAE_GW_UE_EVENT_PGW_GTPC_S5_DELETE_SESSION_REQ,
  /* 37 */
  NW_SAE_GW_UE_EVENT_PGW_GTPC_S5_DELETE_SESSION_RSP,
  /* 38 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 40 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 45 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 50 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 55 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 60 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 65 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 70 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 75 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 80 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 85 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 90 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 95 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 100 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 105 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 110 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 115 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 120 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 125 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 130 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 135 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 140 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 145 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 150 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 155 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 160 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 165 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 170 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 175 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 180 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 185 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 190 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 195 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 200 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 205 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 210 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 215 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 220 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 225 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 230 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 235 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 240 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 245 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 250 */
  NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL, NW_SAE_GW_UE_EVENT_NULL,

  /* 255 */
  NW_SAE_GW_UE_EVENT_NULL,
  /* End.. phewww !! */
};

/*---------------------------------------------------------------------------
 * RBTree Search Functions
 *--------------------------------------------------------------------------*/

/**
 * Comparator funtion for comparing two UE sessions.
 *
 * @param[in] a: Pointer to session a.
 * @param[in] b: Pointer to session b.
 * @return  An integer greater than, equal to or less than zero according to whether the
 * object pointed to by a is greater than, equal to or less than the object pointed to by b.
 */

static inline NwS32T
nwSaeGwUlpCompareUeSession(struct NwSaeGwUe* a, struct NwSaeGwUe* b)
{
  return memcmp(a->imsi, b->imsi, 8);
}

RB_GENERATE(NwUeSgwSessionRbtT, NwSaeGwUe, ueSgwSessionRbtNode, nwSaeGwUlpCompareUeSession)
RB_GENERATE(NwUePgwSessionRbtT, NwSaeGwUe, uePgwSessionRbtNode, nwSaeGwUlpCompareUeSession)

static NwRcT
nwSaeGwUlpCreateUeSession(NwSaeGwUlpT* thiz, NwSaeGwUeT **ppUe)
{
  NwSaeGwUeT* pUe = NULL;

  if(thiz->saeGwType == NW_SAE_GW_TYPE_SGW)
  {
    pUe = nwSaeGwUeNew(thiz->sgw.s11c.hGtpv2cStack, thiz->sgw.s5c.hGtpv2cStack, thiz->pgw.s5c.hGtpv2cStack);
  }
  else if(thiz->saeGwType == NW_SAE_GW_TYPE_PGW)
  {
    pUe = nwSaeGwUeNew(0, 0, thiz->pgw.s5c.hGtpv2cStack);
  }

  if(pUe)
  {
    if(thiz->saeGwType == NW_SAE_GW_TYPE_SGW)
    {
      pUe->hSgw       = (NwHandleT) thiz;
      pUe->hPgw       = (NwHandleT) NULL;
    }
    else
    {
      pUe->hPgw       = (NwHandleT) thiz;
      pUe->hSgw       = (NwHandleT) NULL;
    }

    pUe->state = NW_SAE_GW_UE_STATE_INIT;
    *ppUe = pUe;
    return NW_OK;
  }

  return NW_FAILURE;
}

static NwRcT
nwSaeGwUlpDestroyUeSession(NwSaeGwUlpT* thiz, NwSaeGwUeT **ppUe)
{
  NwRcT rc = NW_OK;
  NwSaeGwUeT *pUe = *ppUe;
  if(pUe->sessionType & NW_SAE_GW_UE_SESSION_TYPE_PGW)
  {
    rc = nwSaeGwUlpPgwDeregisterUeSession(pUe->hPgw, pUe);
  }
  if(pUe->sessionType & NW_SAE_GW_UE_SESSION_TYPE_SGW)
  {
    rc = nwSaeGwUlpSgwDeregisterUeSession(pUe->hSgw, pUe);
  }
  rc = nwSaeGwUeDelete(pUe);
  return rc;
}

static NwRcT
nwSaeGwUlpAllocateIpv4Address(NwSaeGwUlpT* thiz, NwU32T* pIpv4Addr)
{
  return nwIpv4PoolMgrAlloc(thiz->hIpv4Pool, pIpv4Addr);
}

static NwRcT
nwSaeGwUlpFreeIpv4Address(NwSaeGwUlpT* thiz, NwU32T ipv4Addr)
{
  return nwIpv4PoolMgrFree(thiz->hIpv4Pool, ipv4Addr);
}

static NwRcT
nwSaeGwUlpSgwDeregisterAllUeSessionFromPeer(NwSaeGwUlpT* hSaeGw, NwGtpv2cUlpApiT *pUlpApi)
{
  NwRcT                         rc = NW_OK;
  NwSaeGwUlpT* thiz = hSaeGw;
  NwSaeGwUeT *pUe = NULL, *nxt = NULL;
  NwUeStateT                    ueState;
  NwSaeGwUeEventInfoT           eventInfo;
  NwU32T peerAddr;
  NwU64T imsi;

  NW_ASSERT( thiz->saeGwType == NW_SAE_GW_TYPE_SGW);

  peerAddr = pUlpApi->apiInfo.peerChangeInfo.peerIp;

  eventInfo.event = NW_SAE_GW_UE_EVENT_LOW_LAYER_ERROR;
  eventInfo.arg   = pUlpApi;

  for (pUe = RB_MIN(NwUeSgwSessionRbtT, &thiz->ueSgwSessionRbt); pUe != NULL; pUe = nxt) {
    nxt = RB_NEXT(NwUeSgwSessionRbtT, &thiz->ueSgwSessionRbt, pUe);

    memcpy(&imsi, pUe->imsi, sizeof(NwU64T));

    NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                  "Evaluate deletion of UE (%p) (IMSI %llx). UE registered in peer "NW_IPV4_ADDR
                  ", peer down "NW_IPV4_ADDR,
                  pUe,
                  NW_NTOHLL(imsi),
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->s11cTunnel.fteidMme.ipv4Addr)),
                  NW_IPV4_ADDR_FORMAT(peerAddr));
    if(pUe->s11cTunnel.fteidMme.ipv4Addr == peerAddr)
    {
      /* TODO Tell PGW*/
      NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                    "UE (IMSI %llx) should be removed, MME down/restarted",
                    NW_NTOHLL(imsi));

      rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
    }
    else if(pUe->s5s8cTunnel.fteidPgw.ipv4Addr == peerAddr)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                    "UE (IMSI %llx) should be removed, PGW down/restarted",
                    NW_NTOHLL(imsi));

      rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
    }
  }
  return rc;
}


static NwRcT
nwSaeGwUlpPgwDeregisterAllUeSessionFromPeer(NwSaeGwUlpT* hSaeGw, NwGtpv2cUlpApiT *pUlpApi)
{
  NwRcT                         rc = NW_OK;
  NwSaeGwUlpT* thiz = hSaeGw;
  NwSaeGwUeT *pUe, *nxt;
  NwUeStateT                    ueState;
  NwSaeGwUeEventInfoT           eventInfo;
  NwU32T peerAddr = pUlpApi->apiInfo.peerChangeInfo.peerIp;

  NW_ASSERT( thiz->saeGwType == NW_SAE_GW_TYPE_PGW);

  eventInfo.event =  NW_SAE_GW_UE_EVENT_LOW_LAYER_ERROR;
  eventInfo.arg   = pUlpApi;

  for (pUe = RB_MIN(NwUePgwSessionRbtT, &thiz->uePgwSessionRbt); pUe != NULL; pUe =	nxt) {
    nxt = RB_NEXT(NwUePgwSessionRbtT, &thiz->uePgwSessionRbt, pUe);
    if(pUe->s5s8cTunnel.fteidSgw.ipv4Addr == peerAddr)
    {
        rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
    }
  }
  return rc;
}

static NwRcT
nwSaeGwUlpSgwS11GtpcStackIndication (NwGtpv2cUlpHandleT hUlp,
                       NwGtpv2cUlpApiT *pUlpApi)
{
  NwRcT                         rc = NW_OK;
  NwSaeGwUlpT*                  thiz;
  NwUeStateT                    ueState;
  NwSaeGwUeT                    *pUe = NULL;
  NwSaeGwUeEventInfoT           eventInfo;
  NwU64T imsi;
  NW_ASSERT(pUlpApi != NULL);

  thiz = (NwSaeGwUlpT*) hUlp;

  NW_ASSERT(thiz->saeGwType == NW_SAE_GW_TYPE_SGW);

  switch(pUlpApi->apiType)
  {
    case NW_GTPV2C_ULP_API_INITIAL_REQ_IND:
      {
        eventInfo.event = gSgwS11GtpcMsgToUeEventMap[pUlpApi->apiInfo.initialReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "pUlpApi->apiInfo.initialReqIndInfo.msgType = %u eventInfo.event = %u!", pUlpApi->apiInfo.initialReqIndInfo.msgType, eventInfo.event);

        if((NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel)
        {
          /* If it is a request ULP and tunnel handle is set, it is new message on existing session. */
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel;
        }
        else
        {
          /* If it is a request ULP and tunnel handle is NOT set, it is probably new session request */
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_INITIAL_REQ_IND from S11 GTPv2c stack for non-existent UE context!");
          rc = nwSaeGwUlpCreateUeSession(thiz, &pUe);
          if(rc != NW_OK)
          {
            NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Failed to create UE context!");
          }
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND:
      {
        eventInfo.event = gSgwS11GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredRspIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        if((NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)
        {
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel;
          NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from S11 GTPv2c stack for TEID 0x%08x!", ntohl((NwU32T)(pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)));
        }
        else
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from GTPv2c stack for non-existent UE context! Ignoring.");
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND:
      {
        eventInfo.event = gSgwS11GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND from S11 GTPv2c stack!");

        rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
      }
      break;


    case NW_GTPV2C_ULP_API_RSP_FAILURE_IND:
      {
        eventInfo.event   = NW_SAE_GW_UE_EVENT_NACK;
        eventInfo.arg     = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_RSP_FAILURE from S11 GTPv2c stack for session %p and transaction %p!",
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel,
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTrxn);

        if((NwSaeGwUeT*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel)
        {
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel;
          NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_RSP_FAILURE_IND from S11 GTPv2c stack for TEID 0x%08x!", ntohl((NwU32T)(pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel)));
        }
        else
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from GTPv2c stack for non-existent UE context! Ignoring.");
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, (NwSaeGwUeT*)pUlpApi->apiInfo.rspFailureInfo.hUlpTrxn, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE context with IMSI %llx!", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_PEER_CHANGE_IND:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                      "Received NW_GTPV2C_ULP_API_PEER_CHANGE_IND from S11 GTPv2c stack!");
        /* TODO remove all sessions related to this peer */
        rc = nwSaeGwUlpSgwDeregisterAllUeSessionFromPeer(thiz, pUlpApi);
      }
      break;

    default:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN, "Received undefined api type %x from GTPv2c stack!", pUlpApi->apiType);
        rc = NW_FAILURE;
      }
  }

  if(pUlpApi->hMsg)
    rc = nwGtpv2cMsgDelete(thiz->sgw.s11c.hGtpv2cStack, (pUlpApi->hMsg));

  return rc;
}

static NwSdpRcT
nwSaeGwUlpSgwSdpStackIndication (NwSdpUlpHandleT hUlp,  NwSdpUlpApiT *pUlpApi)
{
  NwSdpRcT              rc = NW_SDP_OK;
  NwSaeGwUlpT           *thiz;
  NwSaeGwUeT            *pUe;
  NwUeStateT            ueState;
  NwSaeGwUeEventInfoT   eventInfo;
  NwU64T                imsi=0;

  NW_ASSERT(pUlpApi != NULL);

  thiz = (NwSaeGwUlpT*) hUlp;

  NW_ASSERT(thiz->saeGwType == NW_SAE_GW_TYPE_SGW);

  switch(pUlpApi->apiType)
  {
    case NW_SDP_ULP_API_DATA_IND:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_SDP_ULP_API_DATA_IND from SGW S1U GTPv1u stack");
        eventInfo.event = NW_SAE_GW_UE_EVENT_SGW_GTPU_S1_DATA_NOT;
        eventInfo.arg   = pUlpApi;

        pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.dataIndInfo.hUlpSession;
        NW_ASSERT(pUe);

        rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
        if(ueState == NW_SAE_GW_UE_STATE_END)
        {
          memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
          NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
          rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
        }
      }
      break;
    default:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN, "Received undefined API type %x from SDP stack!", pUlpApi->apiType);
        rc = NW_SDP_FAILURE;
      }
  }
  return rc;
}

static NwRcT
nwSaeGwUlpSgwS5GtpcStackIndication (NwGtpv2cUlpHandleT hUlp,
                       NwGtpv2cUlpApiT *pUlpApi)
{
  NwRcT                         rc = NW_OK;
  NwSaeGwUlpT*                  thiz;
  NwUeStateT                    ueState;
  NwSaeGwUeT                    *pUe = NULL;
  NwSaeGwUeEventInfoT           eventInfo;
  NwU64T     imsi;

  NW_ASSERT(pUlpApi != NULL);

  thiz = (NwSaeGwUlpT*) hUlp;

  NW_ASSERT(thiz->saeGwType == NW_SAE_GW_TYPE_SGW);

  switch(pUlpApi->apiType)
  {
    case NW_GTPV2C_ULP_API_INITIAL_REQ_IND:
      {
        eventInfo.event = gSgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.initialReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        if((NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel)
        {
          /* If it is a request ULP and tunnel handle is set, it is new message on existing session. */
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel;
        }
        else
        {
          /* If it is a request ULP and tunnel handle is NOT set, it is probably new session request */
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_INITIAL_REQ_IND from SGW S5 GTPv2c stack for non-existent UE context!");
          rc = nwSaeGwUlpCreateUeSession(thiz, &pUe);
          if(rc != NW_OK)
          {
            NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Failed to create UE context!");
          }
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND:
      {
        eventInfo.event = gSgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredRspIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "pUlpApi->apiInfo.triggeredRspIndInfo.msgType = %u eventInfo.event = %u!", pUlpApi->apiInfo.initialReqIndInfo.msgType, eventInfo.event);
        if((NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from SGW S5 GTPv2c stack for TEID 0x%08x!", ntohl((NwU32T)(pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)));
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel;
        }
        else
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from GTPv2c stack for non-existent UE context! Ignoring.");
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND:
      {
        eventInfo.event = gSgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND from SGG S5 GTPv2c stack!");

        pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.triggeredReqIndInfo.hUlpTunnel;
        rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
      }
      break;


    case NW_GTPV2C_ULP_API_RSP_FAILURE_IND:
      {
        eventInfo.event   = NW_SAE_GW_UE_EVENT_NACK;
        eventInfo.arg     = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_RSP_FAILURE from SGW S5 GTPv2c stack for session %p and transaction %p!",
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel,
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTrxn);

        pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel;
        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE context with IMSI %llx!",NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_PEER_CHANGE_IND:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                      "Received NW_GTPV2C_ULP_API_PEER_CHANGE_IND from S5/S8 GTPv2c stack!");
        /* TODO remove all sessions related to this peer */
        nwSaeGwUlpSgwDeregisterAllUeSessionFromPeer(thiz, pUlpApi);
      }
      break;

    default:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN, "Received undefined api type %x from SGW S5 GTPv2c stack!", pUlpApi->apiType);
        rc = NW_FAILURE;
      }
  }

  if(pUlpApi->hMsg)
    rc = nwGtpv2cMsgDelete(thiz->sgw.s5c.hGtpv2cStack, (pUlpApi->hMsg));

  return rc;
}

static NwRcT
nwSaeGwUlpPgwS5GtpcStackIndication (NwGtpv2cUlpHandleT hUlp,
                       NwGtpv2cUlpApiT *pUlpApi)
{
  NwRcT                         rc = NW_OK;
  NwSaeGwUlpT*                  thiz;
  NwUeStateT                    ueState;
  NwSaeGwUeT                    *pUe = NULL;
  NwSaeGwUeEventInfoT           eventInfo;
  NwU64T                        imsi;
  NW_ASSERT(pUlpApi != NULL);

  thiz = (NwSaeGwUlpT*) hUlp;

  NW_ASSERT(thiz->saeGwType == NW_SAE_GW_TYPE_PGW);

  switch(pUlpApi->apiType)
  {
    case NW_GTPV2C_ULP_API_INITIAL_REQ_IND:
      {
        eventInfo.event = gPgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.initialReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "pUlpApi->apiInfo.initialReqIndInfo.msgType = %u eventInfo.event = %u!", pUlpApi->apiInfo.initialReqIndInfo.msgType, eventInfo.event);
        if((NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel)
        {
          /* If it is a request ULP and tunnel handle is set, it is new message on existing session. */
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.initialReqIndInfo.hUlpTunnel;
        }
        else
        {
          /* If it is a request ULP and tunnel handle is NOT set, it is probably new session request */
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_INITIAL_REQ_IND from PGW S5 GTPv2c stack for non-existent UE context!");
          rc = nwSaeGwUlpCreateUeSession(thiz, &pUe);
          if(rc != NW_OK)
          {
            NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Failed to create UE context!");
          }
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND:
      {
        eventInfo.event = gPgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredRspIndInfo.msgType];
        eventInfo.arg   = pUlpApi;

        if((NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from PGW S5 GTPv2c stack for TEID 0x%08x!", ntohl((NwU32T)(pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel)));
          pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.triggeredRspIndInfo.hUlpTunnel;
        }
        else
        {
          NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Received NW_GTPV2C_ULP_API_TRIGGERED_RSP_IND from PGW S5 GTPv2c stack for non-existent UE context! Ignoring.");
        }

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE session with IMSI %llx", NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND:
      {
        eventInfo.event = gPgwS5GtpcMsgToUeEventMap[pUlpApi->apiInfo.triggeredReqIndInfo.msgType];
        eventInfo.arg   = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_TRIGGERED_REQ_IND from PGW S5 GTPv2c stack!");
        /* TODO: Process Triggered Request */
        rc = NW_FAILURE;
      }
      break;


    case NW_GTPV2C_ULP_API_RSP_FAILURE_IND:
      {
        eventInfo.event   = NW_SAE_GW_UE_EVENT_NACK;
        eventInfo.arg     = pUlpApi;
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Received NW_GTPV2C_ULP_API_RSP_FAILURE from PGW S5 GTPv2c stack for session %p and transaction %p!",
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel,
                      (void*)pUlpApi->apiInfo.rspFailureInfo.hUlpTrxn);

        pUe = (NwSaeGwUeT*)pUlpApi->apiInfo.rspFailureInfo.hUlpTunnel;

        if(pUe)
        {
          rc = nwSaeGwUeFsmRun(thiz->pUeFsm, pUe, &eventInfo, &ueState);
          if(ueState == NW_SAE_GW_UE_STATE_END)
          {
            memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
            NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Purging UE context with IMSI %llx!",NW_NTOHLL(imsi));
            rc = nwSaeGwUlpDestroyUeSession(thiz, &pUe);
          }
        }
      }
      break;

    case NW_GTPV2C_ULP_API_PEER_CHANGE_IND:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                      "Received NW_GTPV2C_ULP_API_PEER_CHANGE_IND from S5/S8 GTPv2c stack!");
        /* TODO remove all sessions related to this peer */
        nwSaeGwUlpPgwDeregisterAllUeSessionFromPeer(thiz, pUlpApi);
      }
      break;

    default:
      {
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN, "Received undefined api type %x from PGW S5 GTPv2c stack!", pUlpApi->apiType);
        rc = NW_FAILURE;
      }
  }

  if(pUlpApi->hMsg)
    rc = nwGtpv2cMsgDelete(thiz->pgw.s5c.hGtpv2cStack, (pUlpApi->hMsg));

  return rc;
}

static
NwRcT nwSaeGwHandleGtpcv2LogRequest (NwGtpv2cLogMgrHandleT hlogMgr,
    NwU32T logLevel,
    NwCharT* file,
    NwU32T line,
    NwCharT* logStr)
{
  NwLogMgrT* thiz = (NwLogMgrT*) hlogMgr;
  if(thiz->logLevel >= logLevel)
  {
    nwLogMgrLog(&_gLogMgr, "NWGTPCv2 ", logLevel, file, line, logStr);
  }
  return NW_OK;
}

static void NW_EVT_CALLBACK(nwSaeGwHandleGtpcv2StackTimerTimeout)
{
  /* Send Timeout Request to GTPv2c Stack Instance  */
  (void) nwGtpv2cProcessTimeout(arg);
  return;
}

NwRcT nwSaeGwUlpGtpv2TimerStartIndication( NwGtpv2cTimerMgrHandleT tmrMgrHandle,
    NwU32T timeoutSec,
    NwU32T timeoutUsec,
    NwU32T tmrType,
    void*  timeoutArg,
    NwGtpv2cTimerHandleT* phTmr)
{
  return nwTmrMgrStartTimer(tmrMgrHandle, timeoutSec, timeoutUsec, tmrType, nwSaeGwHandleGtpcv2StackTimerTimeout, timeoutArg, (NwTimerHandleT*)phTmr);
}

NwRcT nwSaeGwUlpGtpv2TimerStopIndication( NwGtpv2cTimerMgrHandleT tmrMgrHandle,
    NwGtpv2cTimerHandleT hTmr)
{
  return nwTmrMgrStopTimer(tmrMgrHandle, (NwTimerHandleT)hTmr);
}

void* nwSaeGwUlpGtpv2cMemNewIndication(NwGtpv2cMemMgrHandleT hMemMgr, NwU32T size, NwCharT* fn, NwU32T ln)
{
  return _nwMemNew(size, fn, ln);
}

void  nwSaeGwUlpGtpv2cMemFreeIndication(NwGtpv2cMemMgrHandleT hMemMgr, void* mem, NwCharT* fn, NwU32T ln)
{
  return _nwMemDelete(mem, fn, ln);
}

static NwRcT
nwSaeGwUlpCreateGtpv2cStackInstance(NwSaeGwUlpT               *thiz,
                              NwGtpv2cStackHandleT      *phGtpv2cStack)
{
  NwRcT                         rc;
  NwGtpv2cStackHandleT          hGtpv2cStack;
  NwGtpv2cMemMgrEntityT         memMgr;
  NwGtpv2cLogMgrEntityT         logMgr;
  NwGtpv2cTimerMgrEntityT       tmrMgr;

  /* Intialize the GTPv2c stack */
  rc = nwGtpv2cInitialize(&hGtpv2cStack);

  if(rc != NW_OK)
  {
    NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Failed to create GTPv2c stack instance. Error '%u' occured", rc);
    exit(1);
  }

  NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "GTP-Cv2 Stack Handle %p Creation Successful!", (void*)hGtpv2cStack);

  /* Set up Log Entity for GTPv2C Stack */

  logMgr.logMgrHandle   = (NwGtpv2cLogMgrHandleT) nwLogMgrGetInstance();
  logMgr.logReqCallback  = nwSaeGwHandleGtpcv2LogRequest;

  rc = nwGtpv2cSetLogMgrEntity(hGtpv2cStack, &logMgr);
  NW_ASSERT( NW_OK == rc );

  rc = nwGtpv2cSetLogLevel(hGtpv2cStack, nwLogMgrGetLogLevel(nwLogMgrGetInstance()));
  NW_ASSERT( NW_OK == rc );

  /* Set up Memory Manager Entity for GTPv2C Stack */

  tmrMgr.tmrMgrHandle    = 0;
  tmrMgr.tmrStartCallback= nwSaeGwUlpGtpv2TimerStartIndication;
  tmrMgr.tmrStopCallback = nwSaeGwUlpGtpv2TimerStopIndication;

  rc = nwGtpv2cSetTimerMgrEntity(hGtpv2cStack, &tmrMgr);
  NW_ASSERT( NW_OK == rc );


  /* Set up Timer Manager Entity for GTPv2C Stack */

  memMgr.hMemMgr        = 0;
  memMgr.memAlloc       = nwSaeGwUlpGtpv2cMemNewIndication;
  memMgr.memFree        = nwSaeGwUlpGtpv2cMemFreeIndication;

  rc = nwGtpv2cSetMemMgrEntity(hGtpv2cStack, &memMgr);
  NW_ASSERT( NW_OK == rc );

  *phGtpv2cStack = hGtpv2cStack;
  return rc;
}

/*
 * Constructor
 */

NwSaeGwUlpT*
nwSaeGwUlpNew()
{
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) nwMemNew (sizeof(NwSaeGwUlpT));
  if(thiz)
  {
    memset(thiz, 0, sizeof(NwSaeGwUlpT));
  }
  thiz->pUeFsm = NwSaeGwUeFsmNew();

  return thiz;
}

/*
 * Destrcutor
 */

NwRcT
nwSaeGwUlpDelete(NwSaeGwUlpT*  thiz)
{
  memset(thiz, 0, sizeof(NwSaeGwUlpT));
  nwMemDelete(thiz);
  return NW_OK;
}


static NwRcT
nwSaeGwGetRestartCounter(const char *stateDir, NwU32T ipAddr, NwU8T *restartCounter)
{
  int fd = 0;
  NwU8T r=0;
  char filename[512];
  char addr[INET_ADDRSTRLEN];
  NwU32T _ipAddr = htonl(ipAddr);
  bzero(filename, 512);
  bzero(addr, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &_ipAddr, addr, 50);

  strcat(strcat(strcpy(filename, stateDir), "/recovery-"), addr);
  NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Accessing state file %s", filename);

  if(access(filename, R_OK|W_OK)==0)
  {
    fd = open(filename, O_RDWR);
    if(fd == -1)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Couldn't read file %s, %s ",
                    filename, strerror(errno));
      return NW_FAILURE;
    }
    read(fd, &r, sizeof(NwU8T));
    NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Read from %s (fd=%d) restartCounter %d", filename, fd, r);
    r++;
    close(fd);
  }
  fd = open(filename, O_TRUNC | O_RDWR | O_CREAT, 0644);
  if(fd == -1)
  {
    NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Couldn't write/create file %s, %s ",
                  filename, strerror(errno));
    return NW_FAILURE;
  }
  NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Updating restart Counter (fd=%d) %d", fd, r);
  write(fd, &r, sizeof(NwU8T));
  close(fd);

  *restartCounter = r;
  return NW_OK;
}

NwRcT
nwSaeGwUlpInitialize(NwSaeGwUlpT*     thiz,
                     NwU32T           saeGwType,
                     NwSaeGwUlpConfigT *pCfg)
{
  NwRcT                         rc;
  NwSdpRcT                      sdpRc;
  NwGtpv2cUlpEntityT            ulp;
  NwGtpv2cUdpEntityT            udp;
  NwSdpUlpEntityT               sdpUlp;
  NwU32T                        gtpcIfSelObj;
  NwU8T                         restartCounter=0;

  NW_ASSERT(thiz);

  thiz->saeGwType               = saeGwType;
  thiz->maxUeSessions           = pCfg->maxUeSessions;
  thiz->sgw.s11c.ipv4Addr       = pCfg->s11cIpv4Addr;
  thiz->sgw.s5c.ipv4Addr        = pCfg->s5cIpv4AddrSgw;
  thiz->sgw.s4c.ipv4Addr        = pCfg->s4cIpv4AddrSgw;
  thiz->pgw.s5c.ipv4Addr        = pCfg->s5cIpv4AddrPgw;
  thiz->pDpe                    = pCfg->pDpe;
  memcpy(thiz->apn, pCfg->apn, 1024);


  if(thiz->saeGwType == NW_SAE_GW_TYPE_SGW)
  {
    /* Create SGW-S11 Gtpv2c Service Access Point*/

    rc = nwSaeGwUlpCreateGtpv2cStackInstance(thiz, &thiz->sgw.s11c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwSaeGwGetRestartCounter((const char *)pCfg->stateDir, thiz->sgw.s11c.ipv4Addr, &restartCounter);
    NW_ASSERT( NW_OK == rc );
    rc = nwGtpv2cSetRestartCounter(thiz->sgw.s11c.hGtpv2cStack, restartCounter);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set UDP Entity */
    rc = nwGtpv2cIfInitialize(&thiz->sgw.s11c.udpIf, thiz->sgw.s11c.ipv4Addr, thiz->sgw.s11c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwGtpv2cIfGetSelectionObject(&thiz->sgw.s11c.udpIf, &gtpcIfSelObj);
    NW_ASSERT( NW_OK == rc );

    NW_EVENT_ADD((thiz->sgw.s11c.ev), gtpcIfSelObj, nwGtpv2cIfDataIndicationCallback, &thiz->sgw.s11c.udpIf, (NW_EVT_READ | NW_EVT_PERSIST));

    udp.hUdp               = (NwGtpv2cUdpHandleT)&thiz->sgw.s11c.udpIf;
    udp.udpDataReqCallback = nwGtpv2cIfDataReq;
    rc = nwGtpv2cSetUdpEntity(thiz->sgw.s11c.hGtpv2cStack, &udp);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set ULP Entity */
    ulp.hUlp = (NwGtpv2cUlpHandleT) thiz;
    ulp.ulpReqCallback = nwSaeGwUlpSgwS11GtpcStackIndication;

    rc = nwGtpv2cSetUlpEntity(thiz->sgw.s11c.hGtpv2cStack, &ulp);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set ULP Entity for Data plane*/
    sdpUlp.hUlp = (NwSdpUlpHandleT) thiz;
    sdpUlp.ulpReqCallback = nwSaeGwUlpSgwSdpStackIndication;

    sdpRc = nwSdpSetUlpEntity(thiz->pDpe->hSdp, &sdpUlp);
    NW_ASSERT( NW_SDP_OK == sdpRc );

    /* Create SGW-S5 Gtpv2c Service Access Point*/

    rc = nwSaeGwUlpCreateGtpv2cStackInstance(thiz, &thiz->sgw.s5c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwSaeGwGetRestartCounter((const char *)pCfg->stateDir, thiz->sgw.s5c.ipv4Addr, &restartCounter);
    NW_ASSERT( NW_OK == rc );
    rc = nwGtpv2cSetRestartCounter(thiz->sgw.s5c.hGtpv2cStack, restartCounter);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set UDP Entity */
    rc = nwGtpv2cIfInitialize(&thiz->sgw.s5c.udpIf, thiz->sgw.s5c.ipv4Addr, thiz->sgw.s5c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwGtpv2cIfGetSelectionObject(&thiz->sgw.s5c.udpIf, &gtpcIfSelObj);
    NW_ASSERT( NW_OK == rc );

    NW_EVENT_ADD((thiz->sgw.s5c.ev), gtpcIfSelObj, nwGtpv2cIfDataIndicationCallback, &thiz->sgw.s5c.udpIf, (NW_EVT_READ | NW_EVT_PERSIST));

    udp.hUdp               = (NwGtpv2cUdpHandleT)&thiz->sgw.s5c.udpIf;
    udp.udpDataReqCallback = nwGtpv2cIfDataReq;
    rc = nwGtpv2cSetUdpEntity(thiz->sgw.s5c.hGtpv2cStack, &udp);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set ULP Entity */
    ulp.hUlp = (NwGtpv2cUlpHandleT) thiz;
    ulp.ulpReqCallback = nwSaeGwUlpSgwS5GtpcStackIndication;

    rc = nwGtpv2cSetUlpEntity(thiz->sgw.s5c.hGtpv2cStack, &ulp);
    NW_ASSERT( NW_OK == rc );
  }
  else if(thiz->saeGwType == NW_SAE_GW_TYPE_PGW)
  {
    strncpy((char *)thiz->ue_dns1, (const char *)pCfg->ue_dns1, INET6_ADDRSTRLEN);
    strncpy((char *)thiz->ue_dns2, (const char *)pCfg->ue_dns2, INET6_ADDRSTRLEN);
    thiz->mtu = pCfg->mtu;

    /* Create PGW-S5 Gtpv2c Service Access Point*/

    rc = nwSaeGwUlpCreateGtpv2cStackInstance(thiz, &thiz->pgw.s5c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwSaeGwGetRestartCounter((const char *)pCfg->stateDir, thiz->pgw.s5c.ipv4Addr, &restartCounter);
    NW_ASSERT( NW_OK == rc );
    rc = nwGtpv2cSetRestartCounter(thiz->pgw.s5c.hGtpv2cStack, restartCounter);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set UDP Entity */
    rc = nwGtpv2cIfInitialize(&thiz->pgw.s5c.udpIf, thiz->pgw.s5c.ipv4Addr, thiz->pgw.s5c.hGtpv2cStack);
    NW_ASSERT( NW_OK == rc );

    rc = nwGtpv2cIfGetSelectionObject(&thiz->pgw.s5c.udpIf, &gtpcIfSelObj);
    NW_ASSERT( NW_OK == rc );

    NW_EVENT_ADD((thiz->pgw.s5c.ev), gtpcIfSelObj, nwGtpv2cIfDataIndicationCallback, &thiz->pgw.s5c.udpIf, (NW_EVT_READ | NW_EVT_PERSIST));

    udp.hUdp               = (NwGtpv2cUdpHandleT)&thiz->pgw.s5c.udpIf;
    udp.udpDataReqCallback = nwGtpv2cIfDataReq;
    rc = nwGtpv2cSetUdpEntity(thiz->pgw.s5c.hGtpv2cStack, &udp);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set ULP Entity */
    ulp.hUlp = (NwGtpv2cUlpHandleT) thiz;
    ulp.ulpReqCallback = nwSaeGwUlpPgwS5GtpcStackIndication;

    rc = nwGtpv2cSetUlpEntity(thiz->pgw.s5c.hGtpv2cStack, &ulp);
    NW_ASSERT( NW_OK == rc );

    /* Initialize and Set ULP Entity for Data plane*/ /*TODO*/
    /* sdpUlp.hUlp = (NwSdpUlpHandleT) thiz; */
    /* sdpUlp.ulpReqCallback = nwSaeGwUlpPgwSdpStackIndication; */

    /* sdpRc = nwSdpSetUlpEntity(thiz->pDpe->hSdp, &sdpUlp); */
    /* NW_ASSERT( NW_SDP_OK == sdpRc ); */

    /*  Initialize IP Pool */
    thiz->hIpv4Pool = nwIpv4PoolMgrNew(pCfg->ippoolSubnet + 1, (pCfg->ippoolSubnet + (~pCfg->ippoolMask) - 1), pCfg->ippoolMask);
    if(!thiz->hIpv4Pool)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO, "Failed to create IPv4 pool!");
      NW_ASSERT(0);
    }
  }
  else
  {
    rc = NW_OK;
    NW_ASSERT(0);
  }

  /* Initialize Lists */
  TAILQ_INIT(&(thiz->collocatedPgwList));

  /* Initialize Search RBTree */
  RB_INIT(&(thiz->ueSgwSessionRbt));
  RB_INIT(&(thiz->uePgwSessionRbt));

  return rc;
}

NwRcT
nwSaeGwUlpDestroy(NwSaeGwUlpT*     thiz)
{
  NwRcT rc = NW_OK;

  /*---------------------------------------------------------------------------
   *  TODO: Destroy SAE GW entity.
   *--------------------------------------------------------------------------*/
  return rc;
}

NwRcT
nwSaeGwUlpRegisterCollocatedPgw(NwSaeGwUlpT* thiz, NwSaeGwUlpT* pCollocatedPgw)
{
  NW_ASSERT(thiz);
  NW_ASSERT(pCollocatedPgw);
  NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Registering Collocated PGW %p with IPv4 address "NW_IPV4_ADDR , (void*) pCollocatedPgw, NW_IPV4_ADDR_FORMAT(ntohl(pCollocatedPgw->pgw.s5c.ipv4Addr)));
  TAILQ_INSERT_TAIL(&thiz->collocatedPgwList, pCollocatedPgw, collocatedPgwListNode);
  return NW_OK;
}

NwRcT
nwSaeGwUlpDeregisterCollocatedPgw(NwSaeGwUlpT* thiz, NwSaeGwUlpT* pCollocatedPgw)
{
  NW_ASSERT(thiz);
  NW_ASSERT(pCollocatedPgw);
  NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "De-registering Collocated PGW with IPv4 address "NW_IPV4_ADDR , NW_IPV4_ADDR_FORMAT(ntohl(pCollocatedPgw->pgw.s5c.ipv4Addr)));
  TAILQ_REMOVE(&thiz->collocatedPgwList, pCollocatedPgw, collocatedPgwListNode);
  return NW_OK;
}

NwRcT
nwSaeGwUlpRegisterSgwUeSession(NwHandleT hSgw, NwSaeGwUeT *pUe, NwU32T pgwIpv4Addr, NwHandleT *hPgw)
{
  NwSaeGwUlpT *pPgwListIter;
  NwSaeGwUlpT *thiz = (NwSaeGwUlpT*) hSgw;
  NwSaeGwUeT *pCollision;
  NwU64T      imsi;

  NW_ASSERT(hSgw);
  memcpy(&imsi, pUe->imsi, sizeof(NwU64T));

  NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO, "Registering SGW %p UE with IMSI %llx", (void*)hSgw, NW_NTOHLL(imsi));

  pCollision = RB_INSERT(NwUeSgwSessionRbtT, &thiz->ueSgwSessionRbt, pUe);
  if(pCollision)
  {
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "UE with IMSI %llx was already registered. Rewriting",
                  NW_NTOHLL(imsi));
    nwSaeGwUeDestroyUeSessionTransport(pCollision);
    nwSaeGwUlpDestroyUeSession(thiz, &pCollision);
    pCollision = RB_INSERT(NwUeSgwSessionRbtT, &thiz->ueSgwSessionRbt, pUe);
    NW_ASSERT(pCollision == NULL);
  }

  TAILQ_FOREACH(pPgwListIter, &thiz->collocatedPgwList, collocatedPgwListNode)
  {
    if(pPgwListIter->pgw.s5c.ipv4Addr == pgwIpv4Addr)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG, "Collocated PGW %p found!", (void*) pPgwListIter);
      break;
    }
  }

  pUe->s11cTunnel.fteidSgw.ipv4Addr    = thiz->sgw.s11c.ipv4Addr;
  pUe->s5s8cTunnel.fteidSgw.ipv4Addr   = thiz->sgw.s5c.ipv4Addr;

  *hPgw = (NwHandleT) pPgwListIter;

  pUe->sessionType |= NW_SAE_GW_UE_SESSION_TYPE_SGW;
  return NW_OK;
}

NwRcT
nwSaeGwUlpRegisterPgwUeSession(NwHandleT hPgw, NwSaeGwUeT *pUe)
{
  NwSaeGwUlpT *thiz = (NwSaeGwUlpT*) hPgw;
  NwSaeGwUeT *pCollision;
  NwU64T     imsi;
  memcpy(&imsi, pUe->imsi, sizeof(NwU64T));

  pCollision = RB_INSERT(NwUePgwSessionRbtT, &thiz->uePgwSessionRbt, pUe);
  if(pCollision)
  {
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "UE with IMSI %llx was already registered. Rewriting",
                  NW_NTOHLL(imsi));
    nwSaeGwUeDestroyUeSessionTransport(pCollision);
    nwSaeGwUlpDestroyUeSession(thiz, &pCollision);
    pCollision = RB_INSERT(NwUePgwSessionRbtT, &thiz->uePgwSessionRbt, pUe);
    NW_ASSERT(pCollision == NULL);
  }

  pUe->paa.pdnType                      = 0x01; /* PDN Type IPv4 */
  pUe->s5s8cTunnel.fteidPgw.ipv4Addr    = thiz->pgw.s5c.ipv4Addr;

  pUe->sessionType |= NW_SAE_GW_UE_SESSION_TYPE_PGW;

  return nwSaeGwUlpAllocateIpv4Address(thiz, (NwU32T*)&pUe->paa.ipv4Addr);
}

NwRcT
nwSaeGwUlpSgwDeregisterUeSession(NwHandleT hSaeGw, NwSaeGwUeT *pUe)
{
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;
  NwSaeGwUeT *pCollision;
  NwU64T     imsi;
  memcpy(&imsi, pUe->imsi, sizeof(NwU64T));

  NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Deregistering SGW UE session with IMSI %llx", NW_NTOHLL(imsi));
  pCollision = RB_REMOVE(NwUeSgwSessionRbtT, &thiz->ueSgwSessionRbt, pUe);
  NW_ASSERT(pCollision == pUe);

  pUe->sessionType &= (~NW_SAE_GW_UE_SESSION_TYPE_SGW);

  return NW_OK;
}

NwRcT
nwSaeGwUlpPgwDeregisterUeSession(NwHandleT hSaeGw, NwSaeGwUeT *pUe)
{
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;
  NwSaeGwUeT *pCollision;
  NwU64T     imsi;
  NwU32T     ipv4Addr;
  memcpy(&imsi, pUe->imsi, sizeof(NwU64T));

  NW_SAE_GW_LOG(NW_LOG_LEVEL_NOTI, "Deregistering PGW UE session with IMSI %llx", NW_NTOHLL(imsi));
  pCollision = RB_REMOVE(NwUePgwSessionRbtT, &thiz->uePgwSessionRbt, pUe);
  NW_ASSERT(pCollision == pUe);

  pUe->sessionType &= (~NW_SAE_GW_UE_SESSION_TYPE_PGW);

  memcpy(&ipv4Addr, pUe->paa.ipv4Addr, sizeof(NwU32T));
  return nwSaeGwUlpFreeIpv4Address(thiz, ipv4Addr);
}

NwRcT
nwSaeGwUlpAllocateTeidOrGreKeys(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;

  if(thiz->saeGwType == NW_SAE_GW_TYPE_SGW)
  {
    /* Allocate SGW-GTPU uplink user plane flow GRE key */
    pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey = (NwU32T) &pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey;
    pUe->epsBearer[ebi].s1uTunnel.fteidSgw.ipv4Addr = thiz->pDpe->gtpuIf.ipAddr;
    /* Allocate SGW-GTPU downlink user plane flow GRE key */
    pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey = (NwU32T) &pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey;
    pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr = thiz->pDpe->gtpuIf.ipAddr; //AMIT
  }
  else if(thiz->saeGwType == NW_SAE_GW_TYPE_PGW)
  {
    /* Allocate PGW-GTPU uplink user plane flow GRE key */
    pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey = (NwU32T)&pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey;
    pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.ipv4Addr = thiz->pDpe->gtpuIf.ipAddr;
  }
  else
  {
    NW_ASSERT(0);
  }

  return NW_OK;
}

NwRcT
nwSaeGwUlpInstallUplinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwRcT                 rc;
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;

  NW_ASSERT(thiz);

  /*
   * Create uplink user plane flow
   */
  if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SAE)
  {
    rc = nwSaeGwDpeCreateGtpuIpv4Flow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        (NwU32T)&pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
        &pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
        &pUe->epsBearer[ebi].s1uTunnel.fteidSgw.ipv4Addr,
        &pUe->epsBearer[ebi].hSgwUplink);

    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Created S/P-GW Uplink Bearer (%p) for EBI %u ingress IP "NW_IPV4_ADDR" TEID 0x%08x "
                  "to egress IPue " NW_IPV4_ADDR,
                  (void*)pUe->epsBearer[ebi].hSgwUplink,
                  ebi,
                  NW_IPV4_ADDR_FORMAT(htonl(pUe->epsBearer[ebi].s1uTunnel.fteidSgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SGW)
  {
    rc = nwSaeGwDpeCreateGtpuGtpuFlow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        (NwU32T)&pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.ipv4Addr,
        &pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
        &pUe->epsBearer[ebi].s1uTunnel.fteidSgw.ipv4Addr,
        &pUe->epsBearer[ebi].hSgwUplink);
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Created S-GW Uplink Bearer (%p) for EBI %u ingress IP "NW_IPV4_ADDR
                  " TEID 0x%08x to egress IP "NW_IPV4_ADDR" TEID 0x%08x for IPue "NW_IPV4_ADDR,
                  (void*)pUe->epsBearer[ebi].hSgwUplink,
                  ebi,
                  NW_IPV4_ADDR_FORMAT(htonl(pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMAT(htonl(pUe->epsBearer[ebi].s1uTunnel.fteidSgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s1uTunnel.fteidSgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_PGW)
  {
    rc = nwSaeGwDpeCreateGtpuIpv4Flow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        (NwU32T)&pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey,
        &pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey,
        &pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.ipv4Addr,
        &pUe->epsBearer[ebi].hPgwUplink);
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Created P-GW Uplink Bearer (%p) for EBI %u ingress IP "NW_IPV4_ADDR" TEID 0x%08x "
                  "to egress IPue "NW_IPV4_ADDR,
                  (void*)pUe->epsBearer[ebi].hPgwUplink,
                  ebi,
                  NW_IPV4_ADDR_FORMAT(htonl(pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s5s8uTunnel.fteidPgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
  }
  else
  {
    NW_ASSERT(0);
    rc = NW_FAILURE;
  }

  return rc;
}

NwRcT
nwSaeGwUlpRemoveUplinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwRcT                 rc = NW_OK;
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;

  NW_ASSERT(thiz);

  /*
   * Remove user plane flow
   */

  if(pUe->epsBearer[ebi].isValid)
  {
    if(pUe->epsBearer[ebi].hSgwUplink && thiz->saeGwType == NW_SAE_GW_TYPE_SGW)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Destroying S-GW Uplink Bearer (%p) EBI %u for IPue "NW_IPV4_ADDR,
                    (void*)pUe->epsBearer[ebi].hSgwUplink,
                    ebi, NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
      rc = nwSaeGwDpeDestroyFlow(thiz->pDpe,
                                 (NwSdpUlpSessionHandleT)pUe,
                                 pUe->epsBearer[ebi].hSgwUplink);
    }

    if(pUe->epsBearer[ebi].hPgwUplink && thiz->saeGwType == NW_SAE_GW_TYPE_PGW)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Destroying P-GW Uplink Bearer (%p) EBI %u for IPue "NW_IPV4_ADDR,
                    (void*)pUe->epsBearer[ebi].hPgwUplink,
                    ebi, NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
      rc = nwSaeGwDpeDestroyFlow(thiz->pDpe,
                                 (NwSdpUlpSessionHandleT)pUe,
                                 pUe->epsBearer[ebi].hPgwUplink);
    }
  }

  return rc;
}

NwRcT
nwSaeGwUlpInstallDownlinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwRcT                 rc;
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;
  NwU32T ipv4Addr;

  /*
   * Create downlink user plane flow
   */
  if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SAE)
  {
    if(!pUe->epsBearer[ebi].hSgwDownlink)
    {
      /*
       * Create PGW-IPv4 to eNodeB-GTPU downlink user plane flow
       */
      memcpy(&ipv4Addr, pUe->paa.ipv4Addr, sizeof(NwU32T));

      rc = nwSaeGwDpeCreateIpv4GtpuFlow(thiz->pDpe,
                                        (NwSdpUlpSessionHandleT)pUe,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
                                        ipv4Addr,
                                        &(pUe->epsBearer[ebi].hSgwDownlink));
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Created S/P-GW Downlink Bearer (%p) for EBI %u ingress IPue "NW_IPV4_ADDR
                    " to egress IP "NW_IPV4_ADDR" TEID 0x%08x",
                    (void*)pUe->epsBearer[ebi].hSgwDownlink,
                    ebi,
                    NW_IPV4_ADDR_FORMAT(ipv4Addr),
                    NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                    pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey);
    }
    else
    {
      /*
       * Update PGW-IPv4 to eNodeB-GTPU downlink user plane flow
       */
      rc = nwSaeGwDpeModifyIpv4GtpuFlow(thiz->pDpe,
                                        (NwSdpUlpSessionHandleT)pUe,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
                                        pUe->epsBearer[ebi].hSgwDownlink);
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Modified S/P-GW Downlink Bearer (%p) for EBI %u ingress IPue "NW_IPV4_ADDR
                    " to egress IP "NW_IPV4_ADDR" TEID 0x%08x",
                    (void*)pUe->epsBearer[ebi].hSgwDownlink,
                    ebi,
                    NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr),
                    NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                    pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey);
    }

    /*
     * No need to install a GTPU to GTPU tunnel for SGW-Only case
     * because PGW is never relocated.
     */
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SGW)
  {
    if(!pUe->epsBearer[ebi].hSgwDownlink)
    {
      /*
       * Create SGW-GTPU to eNodeB-GTPU downlink user plane flow
       */
        rc = nwSaeGwDpeCreateGtpuGtpuFlow(thiz->pDpe,
                                          (NwSdpUlpSessionHandleT)pUe,
                                          (NwU32T)&pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                                          pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                                          pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
                                          &pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                                          &pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr,
                                          &pUe->epsBearer[ebi].hSgwDownlink);
        NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                      "Created S-GW Downlink Bearer (%p) for EBI %u ingress IP "NW_IPV4_ADDR
                      " TEID 0x%08x to egress IP "NW_IPV4_ADDR" TEID 0x%08x for IPue "NW_IPV4_ADDR,
                      (void*)pUe->epsBearer[ebi].hSgwDownlink,
                      ebi,
                      NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr)),
                      pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                      NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                      pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                      NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
    }
    else
    {
      /*
       * Modify SGW-GTPU to eNodeB-GTPU downlink user plane flow
       */
      rc = nwSaeGwDpeModifyGtpuGtpuFlow(thiz->pDpe,
                                        (NwSdpUlpSessionHandleT)pUe,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                                        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
                                        pUe->epsBearer[ebi].hSgwDownlink);
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Updated S-GW Downlink Bearer (%p) for EBI %u ingress IP "NW_IPV4_ADDR
                    " TEID 0x%08x to egress IP "NW_IPV4_ADDR" TEID 0x%08x for IPue "NW_IPV4_ADDR,
                    (void*)pUe->epsBearer[ebi].hSgwDownlink,
                    ebi,
                    NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr)),
                    pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                    NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                    pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                    NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
    }
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_PGW)
  {
    NW_ASSERT(pUe->epsBearer[ebi].hSgwDownlink == 0);
    /*
     * Create PGW-IPv4 to SGW-GTPU downlink user plane flow
     */
    memcpy(&ipv4Addr, pUe->paa.ipv4Addr, sizeof(NwU32T));
    rc = nwSaeGwDpeCreateIpv4GtpuFlow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr,
        ipv4Addr,
        &pUe->epsBearer[ebi].hPgwDownlink);

    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Created P-GW Downlink Bearer (%p) for EBI %u ingress IPue "NW_IPV4_ADDR
                  " to egress IP "NW_IPV4_ADDR" TEID 0x%08x",
                  (void*)pUe->epsBearer[ebi].hPgwDownlink,
                  ebi,
                  NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr),
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey);
  }
  else
  {
    NW_ASSERT(0);
  }

  NW_ASSERT( NW_OK == rc );

  return rc;
}

NwRcT
nwSaeGwUlpModifyDownlinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwRcT                 rc;
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;
  NwU32T ipv4Addr;

  /*
   * Create downlink user plane flow
   */
  if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SAE)
  {
    /*
     * Create PGW-IPv4 to eNodeB-GTPU downlink user plane flow
     */
    memcpy(&ipv4Addr, pUe->paa.ipv4Addr, sizeof(NwU32T));
    rc = nwSaeGwDpeModifyIpv4GtpuFlow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
        pUe->epsBearer[ebi].hSgwDownlink);
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Modified S/P-GW Downlink Bearer for EBI %u ingress IPue "NW_IPV4_ADDR
                  " to IP "NW_IPV4_ADDR" TEID 0x%08x",
                  ebi,
                  NW_IPV4_ADDR_FORMAT(ipv4Addr),
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                  pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey);
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_SGW)
  {
    /*
     * Create SGW-GTPU to eNodeB-GTPU downlink user plane flow
     */
    rc = nwSaeGwDpeModifyGtpuGtpuFlow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
        pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr,
        pUe->epsBearer[ebi].hSgwDownlink);
    NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                  "Modified S-GW Downlink Bearer for EBI %u ingress IP "NW_IPV4_ADDR
                  " TEID 0x%08x to egress IP "NW_IPV4_ADDR" TEID 0x%08x for IPue "NW_IPV4_ADDR,
                  ebi,
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr)),
                  pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.ipv4Addr)),
                  pUe->epsBearer[ebi].s1uTunnel.fteidEnodeB.teidOrGreKey,
                  NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
  }
  else if(pUe->sessionType == NW_SAE_GW_UE_SESSION_TYPE_PGW)
  {
    /* TODO Instead of removing and creating a flow, the existing flow should be modified as above
     * This case afects Handover with S-GW relocation.
     */
    if(pUe->epsBearer[ebi].hPgwDownlink)
    {
      rc = nwSaeGwDpeDestroyFlow(thiz->pDpe,
                                 (NwSdpUlpSessionHandleT)pUe,
                                 pUe->epsBearer[ebi].hPgwDownlink);
    }
    /*
     * Create PGW-IPv4 to SGW-GTPU downlink user plane flow
     */
    memcpy(&ipv4Addr, pUe->paa.ipv4Addr, sizeof(NwU32T));
    rc = nwSaeGwDpeCreateIpv4GtpuFlow(thiz->pDpe,
        (NwSdpUlpSessionHandleT)pUe,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
        pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr,
        ipv4Addr,
        &pUe->epsBearer[ebi].hPgwDownlink);

    NW_SAE_GW_LOG(NW_LOG_LEVEL_DEBG,
                  "Creating PGW Downlink Bearer for EBI %u ingress IP "NW_IPV4_ADDR
                  " to egress TEID 0x%08x IP " NW_IPV4_ADDR,
                  ebi,
                  NW_IPV4_ADDR_FORMAT(ipv4Addr),
                  pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.teidOrGreKey,
                  NW_IPV4_ADDR_FORMAT(ntohl(pUe->epsBearer[ebi].s5s8uTunnel.fteidSgw.ipv4Addr)));
  }
  else
  {
    NW_ASSERT(0);
  }

  NW_ASSERT( NW_OK == rc );

  return rc;
}


NwRcT
nwSaeGwUlpReleaseDownlinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi){
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;

  return nwSaeGwDpeReleaseEndpointFlow(thiz->pDpe,
                                       (NwSdpUlpSessionHandleT)pUe,
                                       (NwSdpSessionHandleT)pUe->epsBearer[ebi].hSgwDownlink);
}

NwRcT
nwSaeGwUlpRemoveDownlinkEpsBearer(NwHandleT hSaeGw, NwSaeGwUeT *pUe, NwU8T ebi)
{
  NwRcT                 rc = NW_OK;
  NwSaeGwUlpT* thiz = (NwSaeGwUlpT*) hSaeGw;

  NW_ASSERT(thiz);

  /*
   * Remove user plane flow
   */

  if(pUe->epsBearer[ebi].isValid)
  {
    if(pUe->epsBearer[ebi].hSgwDownlink)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Destroying S-GW Downlink Bearer (%p) EBI %u for IPue "NW_IPV4_ADDR,
                    (void*)pUe->epsBearer[ebi].hSgwDownlink,
                    ebi, NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
      rc = nwSaeGwDpeDestroyFlow(thiz->pDpe,
                                 (NwSdpUlpSessionHandleT)pUe,
                                 pUe->epsBearer[ebi].hSgwDownlink);
      pUe->epsBearer[ebi].hSgwDownlink = (NwDpeBearerHandleT)NULL;
    }

    if(pUe->epsBearer[ebi].hPgwDownlink)
    {
      NW_SAE_GW_LOG(NW_LOG_LEVEL_INFO,
                    "Destroying P-GW Downlink Bearer (%p) EBI %u for IPue "NW_IPV4_ADDR,
                    (void*)pUe->epsBearer[ebi].hPgwDownlink,
                    ebi, NW_IPV4_ADDR_FORMATP(pUe->paa.ipv4Addr));
      rc = nwSaeGwDpeDestroyFlow(thiz->pDpe,
                                 (NwSdpUlpSessionHandleT)pUe,
                                 pUe->epsBearer[ebi].hPgwDownlink);
      pUe->epsBearer[ebi].hPgwDownlink = (NwDpeBearerHandleT)NULL;
    }
  }


  return rc;
}

/* Management of Protocol Configuration options
   TODO: Organize this a bit better.
   TODO: Add IMS parameters
 */

typedef struct{
    NwU16T id;
    NwU8T  length; /* without header */
    NwU8T  data[256];
} __attribute__((packed)) PCO_header_t;

typedef struct{
    NwU8T  code;
    NwU8T  identifier;
    NwU16T length; /* with header */
    NwU8T  data[256];
} __attribute__((packed)) PPP_header_t;

typedef struct{
    NwU8T  option;
    NwU8T  length; /* without header */
    NwU8T  data[256];
} __attribute__((packed)) IPCP_option_t;

NwRcT
nwSaeGwUlpPgwSetPCO(NwHandleT hPgw, NwSaeGwUeT *pUe)
{
  NwSaeGwUlpT *thiz = (NwSaeGwUlpT*) hPgw;
  NwU8T          query[NW_SAE_GW_MAX_PCO_LEN];
  NwU16T         qlength, mtu=0;

  NwU8T          *pco_rsp_p, *pco_p;
  PCO_header_t   *pco_h, *pco_h_rsp;

  NwU8T          *ppp_p, *ppp_rsp_p;
  PPP_header_t   *ppp_h, *ppp_h_rsp;

  NwU8T          *ipcp_p, *ipcp_rsp_p;
  IPCP_option_t  *ipcp_h, *ipcp_h_rsp;

  if(!pUe->pco.isPresent)
  {
    return NW_OK;
  }

  qlength = pUe->pco.length;
  bzero(query, NW_SAE_GW_MAX_PCO_LEN);
  memcpy(query, pUe->pco.value, qlength);
  bzero(pUe->pco.value, NW_SAE_GW_MAX_PCO_LEN);

  pco_rsp_p = pUe->pco.value;
  *pco_rsp_p = 0x80; /* First byte */
  pco_rsp_p++;
  pUe->pco.length=1;

  /*
     pco_p = query+1              : Initial value of the buffer
     pco_p-(query+1) < qlength-1  : read < total
     pco_p += pco_h->length + 3   : next PCO element
   */
  for(pco_p = query+1;
      pco_p-(query+1) < qlength-1;
      pco_p += pco_h->length + 3)
  {
    pco_h = (PCO_header_t *)pco_p;
    pco_h_rsp = (PCO_header_t *) pco_rsp_p;

    switch(NW_NTOHS(pco_h->id))
    {
      case 0xC021: /* LCP */
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN,"LCP PCO received, ignoring");
        continue;
      case 0xC023: /* PAP */
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN,"PAP PCO received, ignoring");
        continue;
      case 0xC223: /* CHAP */
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN,"CHAP PCO received, ignoring");
        continue;
      case 0x8021: /* IP Control Protocol (IPCP) */
        pco_h_rsp->id = NW_HTONS(0x8021);
        ppp_rsp_p = pco_h_rsp->data;
        for (ppp_p = pco_h->data;
             ppp_p - pco_h->data < pco_h->length ;
             ppp_p += ppp_h->length)
        {
          ppp_h = (PPP_header_t *) ppp_p;
          ppp_h_rsp = (PPP_header_t*) ppp_rsp_p;

          ppp_h_rsp->identifier = ppp_h->identifier;
          ipcp_rsp_p = ppp_h_rsp->data;
          for (ipcp_p = ppp_h->data;
               ipcp_p - ppp_h->data < NW_NTOHS(ppp_h->length)-4;
               ipcp_p += ipcp_h->length)
          {
            ipcp_h = (IPCP_option_t *)ipcp_p;
            ipcp_h_rsp = (IPCP_option_t *) ipcp_rsp_p;
            ppp_h_rsp->length = NW_HTONS(4);
            switch(ipcp_h->option)
            {
              case 3: /* IP-Address*/
                ipcp_h_rsp->option = 3;
                ipcp_h_rsp->length = 6;
                memcpy(ipcp_h_rsp->data, pUe->paa.ipv4Addr, 4);
                ppp_h_rsp->code = 3; /* Configure-Nak. */
                break;
              case 129: /* Primary DNS Server Address. */
                ipcp_h_rsp->option = 129;
                ipcp_h_rsp->length = 6;
                inet_pton(AF_INET, thiz->ue_dns1, ipcp_h_rsp->data);
                ppp_h_rsp->code = 3; /* Configure-Nak. */
                break;
              case 131: /* Secondary DNS Server Address.*/
                ipcp_h_rsp->option = 131;
                ipcp_h_rsp->length = 6;
                inet_pton(AF_INET, thiz->ue_dns2, ipcp_h_rsp->data);
                ppp_h_rsp->code = 3; /* Configure-Nak. */
                break;
              default:
                ipcp_h_rsp->option = ipcp_h->option;
                ipcp_h_rsp->length = ipcp_h->length;
                ppp_h_rsp->code = 7; /* Code-Reject. */
                NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN,
                              "IPCP option not recognized (%u), rejecting",
                              ipcp_h->option);
            }

            ipcp_rsp_p += ipcp_h_rsp->length;
            ppp_h_rsp->length = NW_HTONS(NW_NTOHS(ppp_h_rsp->length) + ipcp_h_rsp->length);
          }

          ppp_rsp_p += ppp_h_rsp->length;
          pco_h_rsp->length += NW_NTOHS(ppp_h_rsp->length);
        }
        break;
      case 0x0005: /* MS Support of Network Requested Bearer Control indicator*/
        pco_h_rsp->id      = NW_HTONS(0x0005); /* Selected Bearer Control Mode */
        pco_h_rsp->length  = 1;
        pco_h_rsp->data[0] = 0x2; /* MS/NW */
        break;
      case 0x0010: /* IPv4 Link MTU Request */
        pco_h_rsp->id     = NW_HTONS(0x0010); /* IPv4 Link MTU */
        pco_h_rsp->length = 2;
        mtu = htons(thiz->mtu);
        memcpy(pco_h_rsp->data, &mtu, 2);
        break;
      case 0x000A: /* IP address allocation via NAS signalling */
        /* All right, this is the type implemented*/
        continue; /* Ignore */
      case 0x000B: /* IPv4 address allocation via DHCPv4 */
        {
          NwU64T     imsi;
          memcpy(&imsi, pUe->imsi, sizeof(NwU64T));
          NW_SAE_GW_LOG(NW_LOG_LEVEL_ERRO,
                        "UE (%llx) requested IPv4 address allocation via DHCPv4, "
                        "but it is not supported", NW_NTOHLL(imsi));
        }
        continue;
      case 0x000D: /* DNS Server IPv4 Address Request */
        pco_h_rsp->id = NW_HTONS(0x000d); /* DNS Server IPv4 Address */
        pco_h_rsp->length = 4;
        inet_pton(AF_INET, thiz->ue_dns1, pco_h_rsp->data);
        break;
      default: /* Not Recognized Ignoring*/
        NW_SAE_GW_LOG(NW_LOG_LEVEL_WARN,
                      "PCO not recognized (%x), rejecting",
                      NW_NTOHS(pco_h->id));
        continue; /* Ignore */
    }
    pco_rsp_p +=  pco_h_rsp->length + 3;
    pUe->pco.length += pco_h_rsp->length + 3;
  }
  return NW_OK;
}

#ifdef __cplusplus
}
#endif
