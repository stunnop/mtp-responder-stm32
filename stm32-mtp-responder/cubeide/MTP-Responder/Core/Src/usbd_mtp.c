#include "usbd_mtp.h"
#include "usbd_ctlreq.h"
#include "mtp_constants.h"

/* USB Standard Device Descriptor */
/*CLAUDE : Device Descriptor - This is the primary descriptor that identifies the device. It contains basic information like vendor ID, product ID, 
device class, and USB version supported. It's required for all USB devices.
Device Qualifier Descriptor - This descriptor is specific to USB 2.0 devices that support both full-speed and high-speed operation. 
It describes how the device would operate at the alternate speed (for example, how a high-speed device would behave when connected to a full-speed host).
IDK BRO*/
__ALIGN_BEGIN static uint8_t USBD_MTP_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = //alignement keywords maybe not necessary but they live for now. DMA ?
{
  USB_LEN_DEV_QUALIFIER_DESC,     // bLength: Size of descriptor in bytes (10 bytes)
  USB_DESC_TYPE_DEVICE_QUALIFIER,  // bDescriptorType: Device Qualifier descriptor type (6)
  0x00,                            // bcdUSB: USB specification version number (2.00)
  0x02,                            // (continued from above, LSB first [0x0200])
  0x00,                            // bDeviceClass: Class code (defined at interface level)
  0x00,                            // bDeviceSubClass: Subclass code (defined at interface level)
  0x00,                            // bDeviceProtocol: Protocol code (defined at interface level)
  0x40,                            // bMaxPacketSize0: Maximum packet size for endpoint 0 (64 bytes)
  0x01,                            // bNumConfigurations: Number of possible configurations
  0x00,                            // bReserved: Reserved field, must be set to zero
};

static uint8_t  USBD_MTP_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_MTP_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_MTP_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  USBD_MTP_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_MTP_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_MTP_EP0_RxReady(USBD_HandleTypeDef *pdev);

static uint8_t  *USBD_MTP_GetHSCfgDesc(uint16_t *length);
static uint8_t  *USBD_MTP_GetFSCfgDesc(uint16_t *length);
static uint8_t  *USBD_MTP_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t  *USBD_MTP_GetDeviceQualifierDescriptor(uint16_t *length);

USBD_ClassTypeDef USBD_MTP = {
    USBD_MTP_Init,
    USBD_MTP_DeInit,
    USBD_MTP_Setup,
    NULL,
    USBD_MTP_EP0_RxReady,
    USBD_MTP_DataIn,//includes interrupt and data EPs hence epnum
    USBD_MTP_DataOut,
    NULL,
    NULL,
    NULL,
    USBD_MTP_GetHSCfgDesc,
    USBD_MTP_GetFSCfgDesc,
    USBD_MTP_GetOtherSpeedCfgDesc,
    USBD_MTP_GetDeviceQualifierDescriptor,
};


/**
  * @brief  USBD_MTP_Init
  *         Initialize the MTP interface : open EPs, prep data structures
  * @param  pdev: pointer to device instance (USBD_HandleTypeDef)
  * @param  cfgidx: Configuration index (not used since we only have 1 configuration)
  * @retval status
  */
static uint8_t  USBD_MTP_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx){
  uint8_t ret = 0;

  /* Open EPs*/
  USBD_LL_OpenEP(pdev, MTP_IN_EP, USBD_EP_TYPE_BULK, CDC_DATA_FS_IN_PACKET_SIZE);
  pdev->ep_in[MTP_IN_EP & 0xFU].is_used = 1U; //gets epnum
  USBD_LL_OpenEP(pdev, MTP_OUT_EP, USBD_EP_TYPE_BULK, CDC_DATA_FS_IN_PACKET_SIZE);
  pdev->ep_out[MTP_OUT_EP & 0xFU].is_used = 1U;
  USBD_LL_OpenEP(pdev, MTP_EVENT_EP, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
  pdev->ep_out[MTP_EVENT_EP & 0xFU].is_used = 1U;

  pdev->pClassData = USBD_malloc(sizeof(USBD_MTP_HandleTypeDef));

  if (pdev->pClassData == NULL){
    /* memory allocation failed */
    ret = 1;
  }else{
    USBD_MTP_HandleTypeDef *hMTP = pdev->pClassData; //class handle
    /* Init  physical Interface components */

    //((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Init(); //i have no clue what this actually does

    /* Init Xfer states */
    hMTP->TxState = 0U;
    hMTP->RxState = 0U;

    /* Prepare Out endpoint to receive next packet */
    USBD_LL_PrepareReceive(pdev, MTP_OUT_EP, hMTP->RxBuffer, CDC_DATA_FS_OUT_PACKET_SIZE);
  }

  return ret;
}

/**
  * @brief  USBD_MTP_DeInit
  *         DeInitialize the MTP layer : close EPs, Free memory
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
 static uint8_t  USBD_MTP_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
 {
   uint8_t ret = 0U;
 
   /* Close EPs */
   USBD_LL_CloseEP(pdev, MTP_IN_EP);
   pdev->ep_in[MTP_IN_EP & 0xFU].is_used = 0U;
   USBD_LL_CloseEP(pdev, MTP_OUT_EP);
   pdev->ep_out[MTP_OUT_EP & 0xFU].is_used = 0U;
   USBD_LL_CloseEP(pdev, MTP_EVENT_EP);
   pdev->ep_in[MTP_EVENT_EP & 0xFU].is_used = 0U;
 
   /* DeInit  physical Interface components */
   //TODO IDK!!
   if (pdev->pClassData != NULL)
   {
     //((USBD_CDC_ItfTypeDef *)pdev->pUserData)->DeInit();
     USBD_free(pdev->pClassData);
     pdev->pClassData = NULL;
   }
 
   return ret;
 }

 static uint8_t  USBD_MTP_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req){
  printf("USBD_MTP_Setup->");
  printf("bmRequest:%02x bRequest:%02x\r\n", req->bmRequest, req->bRequest);

  switch (req->bRequest)
  {
  case MTP_REQ_GET_DEVICE_STATUS:
    /* code */
    printf("MTP_REQ_GET_DEVICE_STATUS received\r\n");
    break;
  
  default:
    break;
  }
  USBD_MTP_HandleTypeDef   *hMTP = (USBD_MTP_HandleTypeDef *) pdev->pClassData;

  return 1U;
 }

static uint8_t  USBD_MTP_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum){
  printf("USBD_MTP_DataIn\r\n");

  return 1U;
}

/**
  * @brief  USBD_MTP_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MTP_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum){
  printf("USBD_MTP_DataOut\r\n");
  USBD_MTP_HandleTypeDef *hMTP = (USBD_MTP_HandleTypeDef *) pdev->pClassData;

  //Get the received data length
  hMTP->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);
  printf("data received of length %lu \r\n", hMTP->RxLength);
  printf("data received :");
  for (uint32_t i = 0; i < hMTP->RxLength; i++) {
    printf("%02x ", hMTP->RxBuffer[i]);
  }
  printf("\r\n");

  return 1U;
}

/**
  * @brief  USBD_MTP_EP0_RxReady
  *         Handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
*/
static uint8_t  USBD_MTP_EP0_RxReady(USBD_HandleTypeDef *pdev)
{

	printf("USBD_MTP_EP0_RxReady");
	USBD_MTP_HandleTypeDef *hMTP = (USBD_MTP_HandleTypeDef *) pdev->pClassData;

  if ((pdev->pUserData != NULL) && (hMTP->CmdOpCode != 0xFFU))
  {
    hMTP->CmdOpCode = 0xFFU;

  }
  return USBD_OK;
}


uint8_t  USBD_MTP_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_MTP_ItfTypeDef *fops){
  printf("USBD_MTP_RegisterInterface\r\n");
  uint8_t ret = USBD_FAIL;

  if (fops != NULL){
    pdev->pUserData = fops;
    ret = USBD_OK;
  }

  return ret;
}

/**
  * @brief  USBD_MTP_SetTxBuffer
  * assign transmission buffer in handler to actual buffer address
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
*/
uint8_t  USBD_MTP_SetTxBuffer(USBD_HandleTypeDef   *pdev, uint8_t  *pbuff, uint16_t length){
  printf("USBD_MTP_SetTxBuffer\r\n");
  USBD_MTP_HandleTypeDef   *hMTP = (USBD_MTP_HandleTypeDef *) pdev->pClassData;

  hMTP->TxBuffer = pbuff;
  hMTP->TxLength = length;

  return USBD_OK;
}

/**
  * @brief  USBD_MTP_SetRxBuffer
  * assign reception buffer in handler to actual buffer address
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
*/
uint8_t  USBD_MTP_SetRxBuffer(USBD_HandleTypeDef   *pdev, uint8_t  *pbuff){
  printf("USBD_MTP_SetRxBuffer\r\n");
  USBD_MTP_HandleTypeDef   *hMTP = (USBD_MTP_HandleTypeDef *) pdev->pClassData;

  hMTP->RxBuffer = pbuff;

  return USBD_OK;
}



/* USB MTP device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_MTP_CfgHSDesc[USB_MTP_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  USB_MTP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */

  /* 09 */
  /*---------------------------------------------------------------------------*/

  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x03,   /* bNumEndpoints:  three endpoints used */
  0x06,   /* bInterfaceClass: Communication Interface Class */
  0x01,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: Index of string descriptor*/

  /* 18 */
  /*Control Endpoint 0 is implicit*/

  /*Endpoint Descriptor : Interrupt IN (Events)*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  MTP_EVENT_EP,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  MTP_FS_BINTERVAL,                           /* bInterval: Polling interval*/

  /* 25 */
  /*Endpoint Descriptor : Bulk OUT*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_OUT_EP,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_HS_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */

    /* 32 */
  /*Endpoint Descriptor : Bulk IN*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_IN_EP,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_HS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_HS_MAX_PACKET_SIZE),
  0x00                               /* bInterval: ignore for Bulk transfer */
  /* 39*/
} ;


/* USB MTP device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_MTP_CfgFSDesc[USB_MTP_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  USB_MTP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */

  /*---------------------------------------------------------------------------*/

  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x03,   /* bNumEndpoints:  three endpoints used */
  0x06,   /* bInterfaceClass: Communication Interface Class */
  0x01,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: Index of string descriptor*/

  /*Control Endpoint 0 is implicit*/

  /*Endpoint Descriptor : Interrupt IN (Events)*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  MTP_EVENT_EP,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  MTP_FS_BINTERVAL,                           /* bInterval: Polling interval*/

  /*Endpoint Descriptor : Bulk OUT*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_OUT_EP,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */

  /*Endpoint Descriptor : Bulk IN*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_IN_EP,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00                               /* bInterval: ignore for Bulk transfer */
} ;

__ALIGN_BEGIN uint8_t USBD_MTP_OtherSpeedCfgDesc[USB_MTP_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  USB_MTP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */

  /*---------------------------------------------------------------------------*/

  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x03,   /* bNumEndpoints: three endpoints used */
  0x06,   /* bInterfaceClass: Communication Interface Class */
  0x01,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: Index of string descriptor*/

  /*Control Endpoint 0 is implicit*/

  /*Endpoint Descriptor : Interrupt IN (Events)*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  MTP_EVENT_EP,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  MTP_FS_BINTERVAL,                           /* bInterval: Polling interval*/

  /*Endpoint Descriptor : Bulk OUT*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_OUT_EP,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */

  /*Endpoint Descriptor : Bulk IN*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  MTP_IN_EP,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00                               /* bInterval: ignore for Bulk transfer */
};

/**
  * @brief  USBD_MTP_GetFSCfgDesc
  *         provides both the configuration descriptor and its length
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MTP_GetFSCfgDesc(uint16_t *length)
{
  printf("Gave Full Speed CfgDesc\r\n");
  *length = sizeof(USBD_MTP_CfgFSDesc);
  return USBD_MTP_CfgFSDesc;
}

/**
  * @brief  USBD_MTP_GetHSCfgDesc
  *         provides both the configuration descriptor and its length
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MTP_GetHSCfgDesc(uint16_t *length)
{
  printf("Gave Half Speed CfgDesc\r\n");
  *length = sizeof(USBD_MTP_CfgHSDesc);
  return USBD_MTP_CfgHSDesc;
}

/**
  * @brief  USBD_MTP_GetCfgDesc
  *         provides both the configuration descriptor and its length
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MTP_GetOtherSpeedCfgDesc(uint16_t *length)
{
  printf("Gave Other Speed CfgDesc\r\n");
  *length = sizeof(USBD_MTP_OtherSpeedCfgDesc);
  return USBD_MTP_OtherSpeedCfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         provides both the configuration descriptor and its length
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_MTP_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = sizeof(USBD_MTP_DeviceQualifierDesc);
  return USBD_MTP_DeviceQualifierDesc;
}


