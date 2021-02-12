//
// Created by hristo on 1/5/21.
//

#ifndef STM32F469_DISC_ESP32_HTTP_CLIENT_TASK_H
#define STM32F469_DISC_ESP32_HTTP_CLIENT_TASK_H

#if __cplusplus
extern "C" {
#endif

void HTTPSClientExternal_Task(void *argument);
void HTTPSClientInternal_Task(void *argument);

#if __cplusplus
}
#endif

#endif //STM32F469_DISC_ESP32_HTTP_CLIENT_TASK_H
