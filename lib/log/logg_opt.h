#ifndef LOGG_OPT_H
#define LOGG_OPT_H

#define LOG_DBG_MIN_LEVEL LOG_DBG_LEVEL_INFO
#define LOG_DBG_TYPES_ON  LOG_DBG_OFF
#include "dbg.h"
#if USE_SEMIHOSTING
    #define LOG_PLATFORM_PRINTF printf
    #undef LOG_DBG_TYPES_ON
#else
    #define LOG_PLATFORM_PRINTF dbgPrint
    #undef LOG_DBG_TYPES_ON
#endif

#define LOG_PLATFORM_ABORT dbg_abort

#endif /* LOGG_OPT_H */