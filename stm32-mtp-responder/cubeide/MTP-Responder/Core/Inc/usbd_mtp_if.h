
/*
  ******************************************************************************
  * @file           : usbd_cdc_if.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbd_cdc_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************/

//Defined to prevent recursive inclusion -------------------------------------

#ifndef __USBD_MTP_IF_H__
#define __USBD_MTP_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

//Includes ------------------------------------------------------------------
#include "usbd_mtp.h"

//Define size for the receive and transmit buffer over MTP
#define APP_RX_DATA_SIZE  1024
#define APP_TX_DATA_SIZE  1024

//MTP Interface callback.
extern USBD_MTP_ItfTypeDef USBD_Interface_fops_FS;


//uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);


#ifdef __cplusplus
}
#endif

#endif  /*__USBD_MTP_IF_H__*/
