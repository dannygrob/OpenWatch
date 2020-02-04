#ifndef IT7257DEF_H_
#define IT7257DEF_H_

uint8_t CHIP_WORK_MODE_ACTIVE       = 0x0;
uint8_t CHIP_WORK_MODE_MONITOR      = 0x1;
uint8_t CHIP_WORK_MODE_HIBERNATE    = 0x3;

uint8_t INDEX_CMD_WRITE             = 0x20;
uint8_t INDEX_QUERY_READ            = 0x80;
uint8_t INDEX_CMD_READ              = 0xA0;
uint8_t INDEX_POINT_READ            = 0xE0;

uint8_t QUERY_COMMAND_READY         = 0x00;
uint8_t QUERY_COMMAND_BUSY          = 0x01;
uint8_t QUERY_COMMAND_ERROR         = 0x02;
uint8_t COMMAND_RESPONSE_SUCCESS    = 0x0000;
uint8_t SIGNATURE_LENGTH            = 16;


//Configure register
uint8_t REG_VALID_DETECT            = 0x80;
uint8_t REG_ZOOM_DISTANCE           = 0x97;
uint8_t REG_PERIODACTIVE            = 0x88;
uint8_t SCI_ASSERT_TP               = 0x01;
uint8_t REG_GESTURE_ID              = 0x01;
uint8_t REG_NUM_FINGER              = 0x02;
uint8_t REG_1ST_FINGER              = 0x03;
uint8_t REG_2ED_FINGER              = 0x09;
uint8_t REG_3RD_FINGER              = 0x0F;
uint8_t REG_4TH_FINGER              = 0x15;
uint8_t REG_5TH_FINGER              = 0x1B;
uint8_t REG_GT_ZOOM_OUT             = 0x49;
uint8_t REG_GT_ZOOM_IN              = 0x48;
uint8_t REG_FIRMWARE_ID              = 0xa6;
uint8_t REG_CALI_TPC                = 0xfc;

uint8_t TOUCH_VALID_THRESHOLD       = 0x16;
uint8_t TOUCH_VAL_ZOOM_THRESHOLD    = 0x30;
uint8_t TOUCH_PERIODACTIVE          = 0x6;

//touchpanel firmware update
uint8_t TP_ERR_READID               = 0x02;
uint8_t TP_ERR_ECC                  = 0x03;
uint8_t TP_OK                       = 0x04;

#endif
