/*
 * datasets.h
 *
 *  Created on: Mar 13, 2025
 *      Author: abassett
 */

#ifndef INC_DATASETS_H_
#define INC_DATASETS_H_

typedef struct {
    uint16_t StandardVersion;          // PTP version (e.g., 100 for v1.00)
    uint32_t MTPVendorExtensionID;     // MTP extension ID (0xFFFFFFFF for standard MTP)
    uint16_t MTPVersion;               // MTP version in hundredths
    char* MTPExtensions;               // String describing extensions: "domain.com: 1.0; "
    uint16_t FunctionalMode;           // Current device mode
    uint16_t* OperationsSupported;     // Array of supported operation codes
    uint16_t* EventsSupported;         // Array of supported event codes
    uint16_t* DevicePropertiesSupported; // Array of supported device property codes
    uint16_t* CaptureFormats;          // Array of object formats device can create
    uint16_t* PlaybackFormats;         // Array of object formats device can play/display
    char* Manufacturer;                // Device manufacturer name
    char* Model;                       // Device model name
    char* DeviceVersion;               // Device firmware/software version
    char* SerialNumber;                // Device serial number (32-char hex string)
} DeviceInfo;



#endif /* INC_DATASETS_H_ */
