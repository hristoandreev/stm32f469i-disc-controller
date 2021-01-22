//
// Created by hristo on 1/20/21.
//

#ifndef STM32F469I_DISC_ESP32_MYTIME_H
#define STM32F469I_DISC_ESP32_MYTIME_H

#include "time.h"

#if __cplusplus
extern "C" {
#endif

int getTime(struct timeval *ptimeval);
int setTime(struct timeval *ptimeval);

#if __cplusplus
}
#endif

#endif //STM32F469I_DISC_ESP32_MYTIME_H
