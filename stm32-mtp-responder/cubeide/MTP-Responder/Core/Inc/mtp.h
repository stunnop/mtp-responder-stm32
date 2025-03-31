/*
 * mtp.h
 *
 *  Created on: Mar 13, 2025
 *      Author: abassett
 */

#ifndef INC_MTP_H_
#define INC_MTP_H_

/* Includes ------------------------------------------------------------------*/


typedef struct _MTP_PACKET_HEADER
{
	uint32_t length;
	uint16_t operation;
	uint16_t code;
	uint32_t tx_id;
}MTP_PACKET_HEADER;

typedef struct mtp_container {
    uint32_t length;         // Total length in bytes
    uint16_t type;           // Container type (response = 0x0003)
    uint16_t code;           // Response code (MTP_RESP_OK = 0x2001)
    uint32_t transaction_id; // Matching transaction ID from request
    uint16_t status;         // Device status code
}MTP_Container;

/* Exported functions prototypes ---------------------------------------------*/

int mtp_process_in_packet(int packet_t);



#endif /* INC_MTP_H_ */
