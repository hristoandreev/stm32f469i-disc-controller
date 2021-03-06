/**
  ******************************************************************************
  * File Name          : LWIP.h
  * Description        : This file provides code for the configuration
  *                      of the LWIP.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  *************************************************************************

  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __mx_pppd_H
#define __mx_pppd_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
//#include "lwip/mem.h"
//#include "lwip/memp.h"
//#include "netif/etharp.h"
//#include "lwip/dhcp.h"
//#include "lwip/netif.h"
//#include "lwip/timeouts.h"

/* Includes for RTOS ---------------------------------------------------------*/
#if WITH_RTOS
#include "lwip/tcpip.h"
#endif /* WITH_RTOS */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Global Variables ----------------------------------------------------------*/
//extern ETH_HandleTypeDef heth;

/* LWIP init function */
void PPPoS_Task(void *argument);

#ifdef __cplusplus
}
#endif
#endif /*__ mx_pppd_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
