#ifndef LOGG_H
#define LOGG_H

#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>

#include "logg_opt.h"

/** Debug level: ALL messages*/
#define LOG_DBG_LEVEL_ALL     0x00U
/** Debug level: Debug. tracing program execution, watching variables, ... */
#define LOG_DBG_LEVEL_DEBUG   0x00U
/** Debug level: Info. recording important events in normal execution flow, ... */
#define LOG_DBG_LEVEL_INFO    0x01U
/** Debug level: Warnings. bad checksums, dropped packets, ... */
#define LOG_DBG_LEVEL_WARNING 0x02U
/** Debug level: Serious. memory allocation failures, ... */
#define LOG_DBG_LEVEL_SERIOUS 0x03U
/** Debug level: Severe */
#define LOG_DBG_LEVEL_SEVERE  0x04U
/**
 * @}
 */

#define LOG_DBG_MASK_LEVEL    0x07U
/* compatibility define only */
#define LOG_DBG_LEVEL_OFF     LOG_DBG_LEVEL_ALL

/** @name Enable/disable debug messages completely (LOG_DBG_TYPES_ON)
 * @{
 */
/** flag for BA_DEBUGF to enable that debug message */
#define LOG_DBG_ON            0x80U
/** flag for BA_DEBUGF to disable that debug message */
#define LOG_DBG_OFF           0x00U
/**
 * @}
 */

/** @name Debug message types (LOG_DBG_TYPES_ON)
 * @{
 */
/** flag for BA_DEBUGF indicating a tracing message (to follow program flow) */
#define LOG_DBG_TRACE         0x40U
/** flag for BA_DEBUGF indicating a state debug message (to follow module states) */
#define LOG_DBG_STATE         0x20U
/** flag for BA_DEBUGF indicating newly added code, not thoroughly tested yet */
#define LOG_DBG_FRESH         0x10U
/** flag for BA_DEBUGF to halt after printing this debug message */
#define LOG_DBG_HALT          0x08U
/**
 * @}
 */

#define CL_NORMAL   "\x1B[0m"
#define CL_RED      "\x1B[31m"
#define CL_BR_RED   "\x1B[31;1m"
#define CL_GREEN    "\x1B[32;1m"
//#define CL_GREEN    "\x1B[32m"
#define CL_YELLOW   "\x1B[33m"

#define LOG_LVL_C   CL_BR_RED "C: " /* CRITICAL */
#define LOG_LVL_E   CL_RED "E: "
#define LOG_LVL_W   CL_YELLOW "W: "
#define LOG_LVL_I   CL_GREEN "I: "
#define LOG_LVL_D   CL_NORMAL "D: "

#ifndef LOG_DBG_MIN_LEVEL
#   define LOG_DBG_MIN_LEVEL LOG_DBG_LEVEL_INFO
#endif

#ifndef LOG_DBG_TYPES_ON
#   define LOG_DBG_TYPES_ON  (LOG_DBG_ON|LOG_DBG_TRACE|LOG_DBG_STATE|LOG_DBG_FRESH|LOG_DBG_HALT)
#endif

#ifndef LOG_PLATFORM_PRINTF
#   define LOG_PLATFORM_PRINTF printf
#endif

#ifndef LOG_PLATFORM_ABORT
#   include <stdlib.h>
#   define LOG_PLATFORM_ABORT abort
#endif

#define LOG_INTERNAL(ctrl, lvl, tag, fmt, ...)  do { \
                                if ( \
                                   ((ctrl) & LOG_DBG_ON) && \
                                   ((ctrl) & LOG_DBG_TYPES_ON) && \
                                   ((int16_t)((ctrl) & LOG_DBG_MASK_LEVEL) >= LOG_DBG_MIN_LEVEL)) { \
                                    struct timeval tv {}; \
                                    gettimeofday(&tv, NULL); \
                                    LOG_PLATFORM_PRINTF("%ld.%04ld|" tag "|" lvl fmt CL_NORMAL "\r\n", (long)tv.tv_sec, tv.tv_usec/100, ##__VA_ARGS__); \
                                }\
                            } while(0)

#define LOG_D(ctrl, tag, fmt, ...) LOG_INTERNAL(((ctrl)|LOG_DBG_LEVEL_DEBUG), LOG_LVL_D, tag, fmt, ##__VA_ARGS__)
#define LOG_I(ctrl, tag, fmt, ...) LOG_INTERNAL(((ctrl)|LOG_DBG_LEVEL_INFO), LOG_LVL_I, tag, fmt, ##__VA_ARGS__)
#define LOG_W(ctrl, tag, fmt, ...) LOG_INTERNAL(((ctrl)|LOG_DBG_LEVEL_WARNING), LOG_LVL_W, tag, fmt, ##__VA_ARGS__)
#define LOG_E(ctrl, tag, fmt, ...) LOG_INTERNAL(((ctrl)|LOG_DBG_LEVEL_SERIOUS), LOG_LVL_E, tag, fmt, ##__VA_ARGS__)
#define LOG_C(ctrl, tag, fmt, ...) LOG_INTERNAL(((ctrl)|LOG_DBG_LEVEL_SEVERE), LOG_LVL_C, tag, fmt, ##__VA_ARGS__)

#define DBG_ASSERT(condition, fmt, ...) do {if(!(condition)) {\
                                            LOG_PLATFORM_PRINTF("%s:%d| " CL_BR_RED "Assertion failed: " fmt CL_NORMAL "\r\n",\
                                            __FILE__, __LINE__, ##__VA_ARGS__); \
                                            LOG_PLATFORM_ABORT(); \
                                            }}while(0)
#endif /* LOGG_H */