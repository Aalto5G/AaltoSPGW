/*----------------------------------------------------------------------------*
 *                                                                            *
 *                              n w - g t p v 2 c                             *
 *    G P R S   T u n n e l i n g    P r o t o c o l   v 2 c    S t a c k     *
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

#include <string.h>
#include "NwTypes.h"
#include "NwGtpv2c.h"
#include "NwGtpv2cIe.h"

#ifndef __NW_GTPV2C_MSG_PARSER_H__
#define __NW_GTPV2C_MSG_PARSER_H__

/**
 * @file NwGtpv2cMsgParser.h
 * @brief This file defines APIs to parser gtpv2c messages.
*/

typedef struct
{
  NwU16T                msgType;
  NwU16T                mandatoryIeCount;
  NwGtpv2cStackHandleT  hStack;
  NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType, NwU8T ieLength, NwU8T ieInstance,  NwU8T* ieValue, void* ieReadCallbackArg);
  void* ieReadCallbackArg;

  struct {
    NwU8T iePresence;
    NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType, NwU8T ieLength, NwU8T ieInstance,  NwU8T* ieValue, void* ieReadCallbackArg);
    void* ieReadCallbackArg;
  } ieParseInfo[NW_GTPV2C_IE_TYPE_MAXIMUM][NW_GTPV2C_IE_INSTANCE_MAXIMUM];

  NwU8T *pIe[NW_GTPV2C_IE_TYPE_MAXIMUM][NW_GTPV2C_IE_INSTANCE_MAXIMUM];
} NwGtpv2cMsgParserT;

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * Allocate a gtpv2c message Parser.
  *
  * @param[in] hGtpcStackHandle : gtpv2c stack handle.
  * @param[in] msgType : Message type for this message parser.
  * @param[out] pthiz : Pointer to message parser handle.
  */

NwGtpv2cRcT
nwGtpv2cMsgParserNew( NW_IN NwGtpv2cStackHandleT hGtpcStackHandle,
                      NW_IN NwU8T     msgType,
                      NW_IN NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType,
                                                       NwU8T ieLength,
                                                       NwU8T ieInstance,
                                                       NwU8T* ieValue,
                                                       void* ieReadCallbackArg),
                      NW_IN void* ieReadCallbackArg,
                      NW_IN NwGtpv2cMsgParserT **pthiz);


 /**
  * Free a gtpv2c message parser.
  *
  * @param[in] hGtpcStackHandle : gtpv2c stack handle.
  * @param[in] thiz : Message parser handle.
  */

NwGtpv2cRcT
nwGtpv2cMsgParserDelete( NW_IN NwGtpv2cStackHandleT hGtpcStackHandle,
                          NW_IN NwGtpv2cMsgParserT* thiz);

NwGtpv2cRcT
nwGtpv2cMsgParserUpdateIe( NW_IN NwGtpv2cMsgParserT* thiz,
                        NW_IN NwU8T ieType,
                        NW_IN NwU8T ieInstance,
                        NW_IN NwU8T iePresence,
                        NW_IN NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType,
                                                       NwU8T ieLength,
                                                       NwU8T ieInstance,
                                                       NwU8T* ieValue,
                                                       void* ieReadCallbackArg),
                        NW_IN void* ieReadCallbackArg);


NwGtpv2cRcT
nwGtpv2cMsgParserUpdateIeReadCallback( NW_IN NwGtpv2cMsgParserT* thiz,
                                       NW_IN NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType,
                                                       NwU8T ieLength,
                                                       NwU8T ieInstance,
                                                       NwU8T* ieValue,
                                                       void* ieReadCallbackArg));


NwGtpv2cRcT
nwGtpv2cMsgParserUpdateIeReadCallbackArg( NW_IN NwGtpv2cMsgParserT* thiz,
                                          NW_IN void* ieReadCallbackArg);

NwGtpv2cRcT
nwGtpv2cMsgParserAddIe( NW_IN NwGtpv2cMsgParserT* thiz,
                        NW_IN NwU8T ieType,
                        NW_IN NwU8T ieInstance,
                        NW_IN NwU8T iePresence,
                        NW_IN NwGtpv2cRcT (*ieReadCallback) (NwU8T ieType,
                                                       NwU8T ieLength,
                                                       NwU8T ieInstance,
                                                       NwU8T* ieValue,
                                                       void* ieReadCallbackArg),
                        NW_IN void* ieReadCallbackArg);


NwGtpv2cRcT
nwGtpv2cMsgParserRun( NW_IN NwGtpv2cMsgParserT* thiz,
                      NW_IN NwGtpv2cMsgHandleT hMsg,
                      NW_OUT NwU8T *pOffendingIeType,
                      NW_OUT NwU8T *pOffendingIeInstance);

#ifdef __cplusplus
}
#endif

#endif /* __NW_GTPV2C_MSG_PARSER_H__ */


/*--------------------------------------------------------------------------*
 *                      E N D     O F    F I L E                            *
 *--------------------------------------------------------------------------*/
