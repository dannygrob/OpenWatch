#ifndef LIS3DHDEF_H_
#define LIS3DHDEF_H_

uint8_t REG_WHO_AM_I                    = 0x0f;
uint8_t REG_STATUS_AUX                  = 0x07;
uint8_t REG_OUT_ADC1_L                  = 0x08;
uint8_t REG_OUT_ADC1_H                  = 0x09;
uint8_t REG_OUT_ADC2_L                  = 0x0a;
uint8_t REG_OUT_ADC2_H                  = 0x0b;
uint8_t REG_OUT_ADC3_L                  = 0x0c;
uint8_t REG_OUT_ADC3_H                  = 0x0d;
uint8_t REG_INT_COUNTER_REG             = 0x0e;
uint8_t REG_TEMP_CFG_REG                = 0x1f;
uint8_t REG_CTRL_REG1                   = 0x20;
uint8_t REG_CTRL_REG2                   = 0x21;
uint8_t REG_CTRL_REG3                   = 0x22;
uint8_t REG_CTRL_REG4                   = 0x23;
uint8_t REG_CTRL_REG5                   = 0x24;
uint8_t REG_CTRL_REG6                   = 0x25;
uint8_t REG_REFERENCE                   = 0x26;
uint8_t REG_STATUS_REG                  = 0x27;
uint8_t REG_OUT_X_L                     = 0x28;
uint8_t REG_OUT_X_H                     = 0x29;
uint8_t REG_OUT_Y_L                     = 0x2a;
uint8_t REG_OUT_Y_H                     = 0x2b;
uint8_t REG_OUT_Z_L                     = 0x2c;
uint8_t REG_OUT_Z_H                     = 0x2d;
uint8_t REG_OUT_X_L_INCR             = 0xA8;
uint8_t REG_FIFO_CTRL_REG               = 0x2e;
uint8_t REG_FIFO_SRC_REG                = 0x2f;
uint8_t REG_INT1_CFG                    = 0x30;
uint8_t REG_INT1_SRC                    = 0x31;
uint8_t REG_INT1_THS                    = 0x32;
uint8_t REG_INT1_DURATION               = 0x33;
uint8_t REG_CLICK_CFG                   = 0x38;
uint8_t REG_CLICK_SRC                   = 0x39;
uint8_t REG_CLICK_THS                   = 0x3a;
uint8_t REG_TIME_LIMIT                  = 0x3b;
uint8_t REG_TIME_LATENCY                = 0x3c;
uint8_t REG_TIME_WINDOW                 = 0x3d;

uint8_t STATUS_ZYXOR                    = 0x80;
uint8_t STATUS_ZOR                      = 0x40;
uint8_t STATUS_YOR                      = 0x20;
uint8_t STATUS_XOR                      = 0x10;
uint8_t STATUS_ZYXDA                    = 0x08;
uint8_t STATUS_ZDA                      = 0x04;
uint8_t STATUS_YDA                      = 0x02;
uint8_t STATUS_XDA                      = 0x01;

uint8_t STATUS_AUX_321OR                = 0x80;
uint8_t STATUS_AUX_3OR                  = 0x40;
uint8_t STATUS_AUX_2OR                  = 0x20;
uint8_t STATUS_AUX_1OR                  = 0x10;
uint8_t STATUS_AUX_321DA                = 0x08;
uint8_t STATUS_AUX_3DA                  = 0x04;
uint8_t STATUS_AUX_2DA                  = 0x02;
uint8_t STATUS_AUX_1DA                  = 0x01;

uint8_t CTRL_REG1_ODR3                  = 0x80;
uint8_t CTRL_REG1_ODR2                  = 0x40;
uint8_t CTRL_REG1_ODR1                  = 0x20;
uint8_t CTRL_REG1_ODR0                  = 0x10;
uint8_t CTRL_REG1_LPEN                  = 0x08;
uint8_t CTRL_REG1_ZEN                   = 0x04;
uint8_t CTRL_REG1_YEN                   = 0x02;
uint8_t CTRL_REG1_XEN                   = 0x01;

uint8_t RATE_1_HZ                       = 0x10;
uint8_t RATE_10_HZ                      = 0x20;
uint8_t RATE_25_HZ                      = 0x30;
uint8_t RATE_50_HZ                      = 0x40;
uint8_t RATE_100_HZ                     = 0x50;
uint8_t RATE_200_HZ                     = 0x60;
uint8_t RATE_400_HZ                     = 0x70;

uint8_t CTRL_REG2_HPM1                  = 0x80;
uint8_t CTRL_REG2_HPM0                  = 0x40;
uint8_t CTRL_REG2_HPCF2                 = 0x20;
uint8_t CTRL_REG2_HPCF1                 = 0x10;
uint8_t CTRL_REG2_FDS                   = 0x08;
uint8_t CTRL_REG2_HPCLICK               = 0x04;
uint8_t CTRL_REG2_HPIS2                 = 0x02;
uint8_t CTRL_REG2_HPIS1                 = 0x01;

// High Pass Filter values
uint8_t LIS3DH_HPF_DISABLED               = 0x00;
uint8_t LIS3DH_HPF_AOI_INT1               = 0x01;
uint8_t LIS3DH_HPF_AOI_INT2               = 0x02;
uint8_t LIS3DH_HPF_CLICK                  = 0x04;
uint8_t LIS3DH_HPF_FDS                    = 0x08;

uint8_t LIS3DH_HPF_CUTOFF1                = 0x00;
uint8_t LIS3DH_HPF_CUTOFF2                = 0x10;
uint8_t LIS3DH_HPF_CUTOFF3                = 0x20;
uint8_t LIS3DH_HPF_CUTOFF4                = 0x30;

uint8_t LIS3DH_HPF_DEFAULT_MODE           = 0x00;
uint8_t LIS3DH_HPF_REFERENCE_SIGNAL       = 0x40;
uint8_t LIS3DH_HPF_NORMAL_MODE            = 0x80;
uint8_t LIS3DH_HPF_AUTORESET_ON_INTERRUPT = 0xC0;

uint8_t LIS3DH_FIFO_BYPASS_MODE           = 0x00;
uint8_t LIS3DH_FIFO_FIFO_MODE             = 0x40;
uint8_t LIS3DH_FIFO_STREAM_MODE           = 0x80;
uint8_t LIS3DH_FIFO_STREAM_TO_FIFO_MODE   = 0xC0;


// Bitfield values
uint8_t LIS3DH_X_LOW         = 0x01;
uint8_t LIS3DH_X_HIGH        = 0x02;
uint8_t LIS3DH_Y_LOW         = 0x04;
uint8_t LIS3DH_Y_HIGH        = 0x08;
uint8_t LIS3DH_Z_LOW         = 0x10;
uint8_t LIS3DH_Z_HIGH        = 0x20;
uint8_t LIS3DH_SIX_D         = 0x40;
uint8_t LIS3DH_AOI           = 0x80;

// Click Detection values
uint8_t LIS3DH_SINGLE_CLICK  = 0x15;
uint8_t LIS3DH_DOUBLE_CLICK  = 0x2A;

uint8_t LIS3DH_MODE_NORMAL = 0x00;
uint8_t LIS3DH_MODE_LOW_POWER = 0x01;
uint8_t LIS3DH_MODE_HIGH_RESOLUTION = 0x02;

uint8_t LIS3DH_ADC1 = 0x01;
uint8_t LIS3DH_ADC2 = 0x02;
uint8_t LIS3DH_ADC3 = 0x03;

uint8_t CTRL_REG3_I1_CLICK = 0x80;
uint8_t CTRL_REG3_I1_INT1 = 0x40;
uint8_t CTRL_REG3_I1_DRDY = 0x10;
uint8_t CTRL_REG3_I1_WTM = 0x04;
uint8_t CTRL_REG3_I1_OVERRUN = 0x02;

uint8_t CTRL_REG4_BDU = 0x80;
uint8_t CTRL_REG4_BLE = 0x40;
uint8_t CTRL_REG4_FS1 = 0x20;
uint8_t CTRL_REG4_FS0 = 0x10;
uint8_t CTRL_REG4_HR = 0x08;
uint8_t CTRL_REG4_ST1 = 0x04;
uint8_t CTRL_REG4_ST0 = 0x02;
uint8_t CTRL_REG4_SIM = 0x01;

uint8_t CTRL_REG5_BOOT = 0x80;
uint8_t CTRL_REG5_FIFO_EN = 0x40;
uint8_t CTRL_REG5_LIR_INT1 = 0x08;
uint8_t CTRL_REG5_D4D_INT1 = 0x04;

uint8_t CTRL_REG6_I2_CLICK = 0x80;
uint8_t CTRL_REG6_I2_INT2 = 0x40;
uint8_t CTRL_REG6_BOOT_I2 = 0x10;
uint8_t CTRL_REG6_H_LACTIVE = 0x02;

uint8_t INT1_CFG_AOI = 0x80;
uint8_t INT1_CFG_6D = 0x40;
uint8_t INT1_CFG_ZHIE_ZUPE = 0x20;
uint8_t INT1_CFG_ZLIE_ZDOWNE = 0x10;
uint8_t INT1_CFG_YHIE_YUPE = 0x08;
uint8_t INT1_CFG_YLIE_YDOWNE = 0x04;
uint8_t INT1_CFG_XHIE_XUPE = 0x02;
uint8_t INT1_CFG_XLIE_XDOWNE = 0x01;

uint8_t INT1_SRC_IA = 0x40;
uint8_t INT1_SRC_ZH = 0x20;
uint8_t INT1_SRC_ZL = 0x10;
uint8_t INT1_SRC_YH = 0x08;
uint8_t INT1_SRC_YL = 0x04;
uint8_t INT1_SRC_XH = 0x02;
uint8_t INT1_SRC_XL = 0x01;

uint8_t TEMP_CFG_ADC_PD = 0x80;
uint8_t TEMP_CFG_TEMP_EN = 0x40;

uint8_t FIFO_CTRL_BYPASS = 0x00;
uint8_t FIFO_CTRL_FIFO = 0x40;
uint8_t FIFO_CTRL_STREAM = 0x80;
uint8_t FIFO_CTRL_STREAM_TO_FIFO = 0xc0;

uint8_t FIFO_SRC_WTM = 0x80;
uint8_t FIFO_SRC_OVRN = 0x40;
uint8_t FIFO_SRC_EMPTY = 0x20;
uint8_t FIFO_SRC_FSS_MASK = 0x1f;

#endif
