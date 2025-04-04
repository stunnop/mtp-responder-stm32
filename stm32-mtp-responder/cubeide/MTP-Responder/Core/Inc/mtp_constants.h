/*
 * mtp_constants.h
 *
 *  Created on: Mar 11, 2025
 *      Author: abassett
 */

#ifndef INC_MTP_CONSTANTS_H_
#define INC_MTP_CONSTANTS_H_


// MTP Operation Codes
#define MTP_OPERATION_GET_DEVICE_INFO                       0x1001
#define MTP_OPERATION_OPEN_SESSION                          0x1002
#define MTP_OPERATION_CLOSE_SESSION                         0x1003
#define MTP_OPERATION_GET_STORAGE_IDS                       0x1004
#define MTP_OPERATION_GET_STORAGE_INFO                      0x1005
#define MTP_OPERATION_GET_NUM_OBJECTS                       0x1006
#define MTP_OPERATION_GET_OBJECT_HANDLES                    0x1007
#define MTP_OPERATION_GET_OBJECT_INFO                       0x1008
#define MTP_OPERATION_GET_OBJECT                            0x1009
#define MTP_OPERATION_GET_THUMB                             0x100A
#define MTP_OPERATION_DELETE_OBJECT                         0x100B
#define MTP_OPERATION_SEND_OBJECT_INFO                      0x100C
#define MTP_OPERATION_SEND_OBJECT                           0x100D
#define MTP_OPERATION_INITIATE_CAPTURE                      0x100E
#define MTP_OPERATION_FORMAT_STORE                          0x100F
#define MTP_OPERATION_RESET_DEVICE                          0x1010
#define MTP_OPERATION_SELF_TEST                             0x1011
#define MTP_OPERATION_SET_OBJECT_PROTECTION                 0x1012
#define MTP_OPERATION_POWER_DOWN                            0x1013
#define MTP_OPERATION_GET_DEVICE_PROP_DESC                  0x1014
#define MTP_OPERATION_GET_DEVICE_PROP_VALUE                 0x1015
#define MTP_OPERATION_SET_DEVICE_PROP_VALUE                 0x1016
#define MTP_OPERATION_RESET_DEVICE_PROP_VALUE               0x1017
#define MTP_OPERATION_TERMINATE_OPEN_CAPTURE                0x1018
#define MTP_OPERATION_MOVE_OBJECT                           0x1019
#define MTP_OPERATION_COPY_OBJECT                           0x101A
#define MTP_OPERATION_GET_PARTIAL_OBJECT                    0x101B
#define MTP_OPERATION_INITIATE_OPEN_CAPTURE                 0x101C
#define MTP_OPERATION_GET_OBJECT_PROPS_SUPPORTED            0x9801
#define MTP_OPERATION_GET_OBJECT_PROP_DESC                  0x9802
#define MTP_OPERATION_GET_OBJECT_PROP_VALUE                 0x9803
#define MTP_OPERATION_SET_OBJECT_PROP_VALUE                 0x9804
#define MTP_OPERATION_GET_OBJECT_PROP_LIST                  0x9805
#define MTP_OPERATION_SET_OBJECT_PROP_LIST                  0x9806
#define MTP_OPERATION_GET_INTERDEPENDENT_PROP_DESC          0x9807
#define MTP_OPERATION_SEND_OBJECT_PROP_LIST                 0x9808
#define MTP_OPERATION_GET_OBJECT_REFERENCES                 0x9810
#define MTP_OPERATION_SET_OBJECT_REFERENCES                 0x9811
#define MTP_OPERATION_SKIP                                  0x9820


// MTP Response Codes
#define MTP_RESPONSE_UNDEFINED                                  0x2000
#define MTP_RESPONSE_OK                                         0x2001
#define MTP_RESPONSE_GENERAL_ERROR                              0x2002
#define MTP_RESPONSE_SESSION_NOT_OPEN                           0x2003
#define MTP_RESPONSE_INVALID_TRANSACTION_ID                     0x2004
#define MTP_RESPONSE_OPERATION_NOT_SUPPORTED                    0x2005
#define MTP_RESPONSE_PARAMETER_NOT_SUPPORTED                    0x2006
#define MTP_RESPONSE_INCOMPLETE_TRANSFER                        0x2007
#define MTP_RESPONSE_INVALID_STORAGE_ID                         0x2008
#define MTP_RESPONSE_INVALID_OBJECT_HANDLE                      0x2009
#define MTP_RESPONSE_DEVICE_PROP_NOT_SUPPORTED                  0x200A
#define MTP_RESPONSE_INVALID_OBJECT_FORMAT_CODE                 0x200B
#define MTP_RESPONSE_STORAGE_FULL                               0x200C
#define MTP_RESPONSE_OBJECT_WRITE_PROTECTED                     0x200D
#define MTP_RESPONSE_STORE_READ_ONLY                            0x200E
#define MTP_RESPONSE_ACCESS_DENIED                              0x200F
#define MTP_RESPONSE_NO_THUMBNAIL_PRESENT                       0x2010
#define MTP_RESPONSE_SELF_TEST_FAILED                           0x2011
#define MTP_RESPONSE_PARTIAL_DELETION                           0x2012
#define MTP_RESPONSE_STORE_NOT_AVAILABLE                        0x2013
#define MTP_RESPONSE_SPECIFICATION_BY_FORMAT_UNSUPPORTED        0x2014
#define MTP_RESPONSE_NO_VALID_OBJECT_INFO                       0x2015
#define MTP_RESPONSE_INVALID_CODE_FORMAT                        0x2016
#define MTP_RESPONSE_UNKNOWN_VENDOR_CODE                        0x2017
#define MTP_RESPONSE_CAPTURE_ALREADY_TERMINATED                 0x2018
#define MTP_RESPONSE_DEVICE_BUSY                                0x2019
#define MTP_RESPONSE_INVALID_PARENT_OBJECT                      0x201A
#define MTP_RESPONSE_INVALID_DEVICE_PROP_FORMAT                 0x201B
#define MTP_RESPONSE_INVALID_DEVICE_PROP_VALUE                  0x201C
#define MTP_RESPONSE_INVALID_PARAMETER                          0x201D
#define MTP_RESPONSE_SESSION_ALREADY_OPEN                       0x201E
#define MTP_RESPONSE_TRANSACTION_CANCELLED                      0x201F
#define MTP_RESPONSE_SPECIFICATION_OF_DESTINATION_UNSUPPORTED   0x2020
#define MTP_RESPONSE_INVALID_OBJECT_PROP_CODE                   0xA801
#define MTP_RESPONSE_INVALID_OBJECT_PROP_FORMAT                 0xA802
#define MTP_RESPONSE_INVALID_OBJECT_PROP_VALUE                  0xA803
#define MTP_RESPONSE_INVALID_OBJECT_REFERENCE                   0xA804
#define MTP_RESPONSE_GROUP_NOT_SUPPORTED                        0xA805
#define MTP_RESPONSE_INVALID_DATASET                            0xA806
#define MTP_RESPONSE_SPECIFICATION_BY_GROUP_UNSUPPORTED         0xA807
#define MTP_RESPONSE_SPECIFICATION_BY_DEPTH_UNSUPPORTED         0xA808
#define MTP_RESPONSE_OBJECT_TOO_LARGE                           0xA809
#define MTP_RESPONSE_OBJECT_PROP_NOT_SUPPORTED                  0xA80A
#define MTP_RESPONSE_NO_RESPONSE                                0xFFFF

// MTP class requests
#define MTP_REQ_CANCEL              0x64
#define MTP_REQ_GET_EXT_EVENT_DATA  0x65
#define MTP_REQ_RESET               0x66
#define MTP_REQ_GET_DEVICE_STATUS   0x67


#endif /* INC_MTP_CONSTANTS_H_ */
