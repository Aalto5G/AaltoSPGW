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
 * @file NwIpv4If.c
 * @brief This files defines IP interface entity.
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/sockios.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include "NwEvt.h"
#include "NwUtils.h"
#include "NwLog.h"
#include "NwIpv4If.h"
#include "NwIpv4IfLog.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 *                          I P     E N T I T Y
 *--------------------------------------------------------------------------*/

/*
 The following function was taken from :
 https://github.com/a34729t/exp/blob/master/tunclient.c
*/
static int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";

   /* open the clone device */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }

   /* preparation of the struct ifr, of type "struct ifreq" */
   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
  strcpy(dev, ifr.ifr_name);

  /* this is the special file descriptor that the caller will use to talk
   * with the virtual interface */
  return fd;
}

NwRcT nwIpv4IfInitialize(NwIpv4IfT* thiz, NwU8T* device, NwSdpHandleT hSdp)
{
  int sd;

  sd = tun_alloc(device, IFF_TUN | IFF_NO_PI);  /* tun interface */
  if(sd < 0){
    NW_IP_LOG(NW_LOG_LEVEL_ERRO, "Failed to open tun interface (%s)", device);
    return NW_FAILURE;
  }
  thiz->hSocket = sd;
  thiz->hSdp    = hSdp;

  return NW_OK;
}

NwRcT nwIpv4IfGetSelectionObjectIpv4(NwIpv4IfT* thiz, NwU32T *pSelObj)
{
  *pSelObj = thiz->hSocket;
  return NW_OK;
}

void NW_EVT_CALLBACK(nwIpv4IfDataIndicationCallback)
{
  NwRcT         rc;
  NwU8T         ipDataBuf[MAX_IP_PAYLOAD_LEN];
  NwS32T        bytesRead;
  NwIpv4IfT* thiz = (NwIpv4IfT*) arg;

  bytesRead = read(thiz->hSocket, ipDataBuf, MAX_IP_PAYLOAD_LEN);
  if(bytesRead > 0)
  {
    NW_IP_LOG(NW_LOG_LEVEL_DEBG, "Received IP message of length %u", bytesRead);
    nwLogHexDump((NwU8T*)ipDataBuf, bytesRead);
    rc = nwSdpProcessIpv4DataInd(thiz->hSdp, 0, (ipDataBuf), (bytesRead));
  }
  else
  {
    NW_IP_LOG(NW_LOG_LEVEL_ERRO, "%s", strerror(errno));
  }
}

NwRcT nwIpv4IfIpv4DataReq(NwSdpHandleT hThiz,
    NwU8T* dataBuf,
    NwU32T dataSize)
{
  NwS32T bytesSent;
  NwIpv4IfT* thiz = (NwIpv4IfT*) hThiz;

  nwLogHexDump((NwU8T*)dataBuf, dataSize);

  bytesSent = write(thiz->hSocket, dataBuf, dataSize);

  if(bytesSent < 0)
  {
    NW_IP_LOG(NW_LOG_LEVEL_ERRO, "IP PDU send error - %s", strerror(errno));
  }
  return NW_OK;
}

#ifdef __cplusplus
}
#endif
