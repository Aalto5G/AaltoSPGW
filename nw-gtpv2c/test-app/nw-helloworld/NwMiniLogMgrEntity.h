/*----------------------------------------------------------------------------*
 *                                                                            *
 *         M I N I M A L I S T I C    L O G M G R     E N T I T Y             *
 *                                                                            *
 *                    Copyright (C) 2010 Amit Chawre.                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/

/**
 * @file hello-world.c
 * @brief This file contains example of a minimalistic log manager entity.
*/

#include <stdio.h>
#include <assert.h>
#include "NwEvt.h"
#include "NwLog.h"

#ifndef NW_ASSERT
#define NW_ASSERT assert
#endif

#ifndef __NW_MINI_LOG_MGR_H__
#define __NW_MINI_LOG_MGR_H__

#define NW_LOG( _logLevel, ...)                                         \
  do {                                                                  \
    if((nwMiniLogMgrGetInstance())->logLevel >= _logLevel)              \
    {                                                                   \
      char _logStr[1024];                                               \
      snprintf(_logStr, 1024, __VA_ARGS__);                             \
      printf("NWGTPV2C-APP  %s - %s <%s,%u>\n", gLogLevelStr[_logLevel], _logStr, basename(__FILE__), __LINE__);\
    }                                                                   \
  } while(0)

/**
 * MiniLogMgr Class Definition
 */
typedef struct NwMiniLogMgr
{
  NwU8T  logLevel; /*< Log level */
} NwMiniLogMgrT;


/*---------------------------------------------------------------------------
 * Public functions
 *--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get global singleton MiniLogMgr instance
 */
NwMiniLogMgrT* nwMiniLogMgrGetInstance();

/**
 * Initialize MiniLogMgr
 * @param thiz : Pointer to global singleton MiniLogMgr instance
 * @param logLevel : Log Level
 */
NwGtpv2cRcT nwMiniLogMgrInit(NwMiniLogMgrT* thiz, NwU32T logLevel );

/**
 * Set MiniLogMgr log level
 * @param thiz : Pointer to global singleton MiniLogMgr instance
 * @param logLevel : Log Level
 */
NwGtpv2cRcT nwMiniLogMgrSetLogLevel(NwMiniLogMgrT* thiz, NwU32T logLevel);

/**
 * Process log request from stack
 * @param thiz : Pointer to global singleton MiniLogMgr instance
 * @param logLevel : Log Level
 * @param file : Filename
 * @param line : Line Number
 * @param logStr : Log string
 */
NwGtpv2cRcT nwMiniLogMgrLogRequest (NwGtpv2cLogMgrHandleT logMgrHandle,
    NwU32T logLevel,
    NwCharT* file,
    NwU32T line,
    NwCharT* logStr);

#ifdef __cplusplus
}
#endif

#endif
