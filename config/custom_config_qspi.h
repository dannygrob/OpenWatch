/**
\addtogroup BSP
\{
\addtogroup CONFIG
\{
\addtogroup CUSTOM
\{
*/

/**
 ****************************************************************************************
 *
 * @file custom_config_qspi.h
 *
 * @brief Board Support Package. User Configuration file for cached QSPI mode.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef CUSTOM_CONFIG_QSPI_H_
#define CUSTOM_CONFIG_QSPI_H_

#include "bsp_definitions.h"

#define CONFIG_USE_BLE
#undef CONFIG_USE_FTDF

#define CONFIG_BLE_STORAGE (1)

#define DEVKIT_USB                              (1)
#define CONFIG_RETARGET                         (1)
#define CONFIG_RETARGET_UART    HW_UART2

#define dg_configTESTMODE_MEASURE_SLEEP_CURRENT (0)

#define dg_configUSE_LP_CLK                     LP_CLK_32768
#define dg_configEXEC_MODE                      MODE_IS_CACHED
#define dg_configCODE_LOCATION                  NON_VOLATILE_IS_FLASH
#define dg_configEXT_CRYSTAL_FREQ               EXT_CRYSTAL_IS_16M

#define dg_configIMAGE_SETUP                    DEVELOPMENT_MODE
#define dg_configEMULATE_OTP_COPY               (0)

#define dg_configUSER_CAN_USE_TIMER1            (0)
#define dg_configUSE_HW_TIMER2                  (1)

#define CONFIG_LIS3DH                           (1)
#define CONFIG_IT7257                           (1)

#define dg_configUSE_WDOG                       (1)
#define dg_configI2C_ADAPTER                    (1)
#define dg_configUSE_HW_I2C                     (1)

#define dg_configFLASH_CONNECTED_TO             (FLASH_CONNECTED_TO_1V8)
#define dg_configFLASH_POWER_DOWN               (0)

#define dg_configPOWER_1V8_ACTIVE               (1)
#define dg_configPOWER_1V8_SLEEP                (1)
//
#define dg_configBATTERY_TYPE                   (BATTERY_TYPE_LIMN2O4)
#define dg_configBATTERY_CHARGE_CURRENT         3       // 30mA
#define dg_configBATTERY_PRECHARGE_CURRENT      2      // 2.1mA
#define dg_configBATTERY_CHARGE_NTC             1       // disabled


//#define dg_configBATTERY_TYPE                   (BATTERY_TYPE_CUSTOM)
#define dg_configBATTERY_CHARGE_VOLTAGE         0x0A // 4.2V
#define dg_configBATTERY_TYPE_CUSTOM_ADC_VOLTAGE (3440)
#define dg_configBATTERY_LOW_LEVEL              (2496) // 3V
#define dg_configPRECHARGING_THRESHOLD          (2462) // 3.006V
#define dg_configCHARGING_THRESHOLD             (2498) // 3.05V
#define dg_configBATTERY_CHARGE_CURRENT         3 // 45mA (see lib/sdk/bsp/system/sys_man/sys_charger.c:115)
//#define dg_configBATTERY_PRECHARGE_CURRENT      1 // 15mA
#define dg_configBATTERY_CHARGE_NTC             1 // disabled

#define dg_configPRECHARGING_TIMEOUT            (60 * 60 * 100) // N x 10msec
#define dg_configUSE_USB                        0
#define dg_configUSE_USB_CHARGER                1//MUST SET TO 1
#define dg_configUSE_USB_ENUMERATION            0
#define dg_configALLOW_CHARGING_NOT_ENUM        1
//
#define dg_configCHARGING_CC_TIMEOUT (360 * 60 * 100)
#define dg_configCHARGING_CV_TIMEOUT (720 * 60 * 100)
#define dg_configUSB_CHARGER_POLLING_INTERVAL (60 * 100) // 1 min (Unit: 0.01s)
#define dg_configBATTERY_REPLENISH_GAP (8) // 0.01V

#define dg_configUSE_ProDK                      (1)

#define dg_configUSE_SW_CURSOR                  0//(1)

#define dg_configCACHEABLE_QSPI_AREA_LEN        0x80000//(NVMS_PARAM_PART_start - MEMORY_QSPIF_BASE)
#define dg_configQSPI_CODE_SIZE                 (256 * 1024)

/*************************************************************************************************\
 * Custom flash specific config
 */
#define dg_configFLASH_HEADER_FILE              "qspi_w25q128.h"
#define dg_configFLASH_DEVICE_TYPE              W25Q128
#define dg_configFLASH_DENSITY                  W25Q_128Mb_SIZE
#define USE_PARTITION_TABLE_16MB


/*************************************************************************************************\
 * Memory layout specific config
 */
#define dg_configQSPI_CACHED_RAM_SIZE           ( 56 * 1024)
#define dg_configQSPI_CACHED_RETRAM_0_SIZE      ( 72 * 1024)//72*1024

/*************************************************************************************************\
 * FreeRTOS specific config
 */
#define OS_FREERTOS                              /* Define this to use FreeRTOS */
#define configTOTAL_HEAP_SIZE                    (19 * 1024)//19*1024//19456   /* This is the FreeRTOS Total Heap Size */

/*************************************************************************************************\
 * Peripheral specific config
 */
#define dg_configFLASH_ADAPTER                  1
#define dg_configNVMS_ADAPTER                   1
#define dg_configNVMS_VES                       1
#define dg_configDISABLE_BACKGROUND_FLASH_OPS   1
#define dg_configGPADC_ADAPTER                  1
#define dg_configBATTERY_ADAPTER                (1)

#define dg_configUSE_SOC                        (1)
/*************************************************************************************************\
 * BLE device config
 */
#define dg_configBLE_CENTRAL                    (0)
#define dg_configBLE_GATT_SERVER                (1)
#define dg_configBLE_OBSERVER                   (0)
#define dg_configBLE_BROADCASTER                (0)
#define dg_configBLE_L2CAP_COC                  (0)

/**************************************************************************************************\
* Macros for enabling SPI operations using Adapters
*/
#define dg_configUSE_HW_SPI                     1
#define dg_configSPI_ADAPTER                    1
#define CONFIG_SPI_ONE_DEVICE_ON_BUS            1
#define CONFIG_SPI_EXCLUSIVE_OPEN               1

#define CONFIG_SPI_DISPLAY                      1

#define LOAD_NEW_IMAGES                         0
#define dg_configUART_ADAPTER                   1



/* Include bsp default values */
#include "bsp_defaults.h"
/* Include memory layout */
#include "bsp_memory_layout.h"

#endif /* CUSTOM_CONFIG_QSPI_H_ */

/**
\}
\}
\}
*/
