/**
  ******************************************************************************
  * @file    usbd_mtp.h
  * @author  Alexander Bassett
  * @brief   header file for the usbd_mtp.c file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MTP_H
#define __USB_MTP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"


#define MTP_IN_EP                                   0x81U  /* EP1 for data IN */
#define MTP_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define MTP_EVENT_EP                                0x82U  /* EP2 for events to host*/

#ifndef MTP_HS_BINTERVAL
#define MTP_HS_BINTERVAL                          0x10U
#endif /* MTP_HS_BINTERVAL */

#ifndef MTP_FS_BINTERVAL
#define MTP_FS_BINTERVAL                          0x10U
#endif /* MTP_FS_BINTERVAL */

/* Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_HS_MAX_PACKET_SIZE                 512U  /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */

#define USB_MTP_CONFIG_DESC_SIZ                     39U
#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE



typedef struct _USBD_MTP_Itf
{
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length);
  int8_t (* Receive)(uint8_t *Buf, uint32_t *Len);

} USBD_MTP_ItfTypeDef;


/*shoved into regular handle->pclassdata ?*/
typedef struct
{
  uint32_t data[CDC_DATA_HS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
  uint16_t  CmdOpCode;
  uint8_t  CmdLength;
  uint8_t  *RxBuffer; //lets avoid reading random memory and properly initialise this !! 
  uint8_t  *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;
  /* Session management variables,
  Object information,
  Transaction management,
  Different state management variables specific to MTP operations */

  __IO uint32_t TxState;
  __IO uint32_t RxState;
}
USBD_MTP_HandleTypeDef;



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_MTP;
#define USBD_MTP_CLASS    &USBD_MTP
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t  USBD_MTP_RegisterInterface(USBD_HandleTypeDef   *pdev,
                                    USBD_MTP_ItfTypeDef *fops);

uint8_t  USBD_MTP_SetTxBuffer(USBD_HandleTypeDef   *pdev,
                              uint8_t  *pbuff,
                              uint16_t length);

uint8_t  USBD_MTP_SetRxBuffer(USBD_HandleTypeDef   *pdev,
                              uint8_t  *pbuff);

uint8_t  USBD_MTP_ReceivePacket(USBD_HandleTypeDef *pdev);

uint8_t  USBD_MTP_TransmitPacket(USBD_HandleTypeDef *pdev);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_MTP_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
