/*
 * lis3dh_task.c
 *
 *  Created on: 19 Sep 2019
 *      Author: Nitwit
 */
#include "osal.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "hw_wkup.h"
#include "sys_watchdog.h"
#include "platform_devices.h"
#include "sys_power_mgr.h"
#include "ad_nvms.h"
#include "miniDB.h"
#include "lis3dh.h"

void wake_interrupt_cb(void)
{
        hw_wkup_reset_interrupt();
        OS_TASK_NOTIFY(getDisplayTaskHandle(), UPDATE_ACCEL_MASK, OS_NOTIFY_SET_BITS);
}

void lis3dh_task(void *params)
{
        setLis3DHTaskHandle(OS_GET_CURRENT_TASK());

        OS_TICK_TIME xNextWakeTime;
        static uint32_t test_counter=0;
        uint32_t ulNotifiedValue;

        /* Initialize and configure the Wake-up Timer */
        hw_wkup_init(NULL);
        hw_wkup_configure_pin(HW_GPIO_PORT_3, HW_GPIO_PIN_1, 1, HW_WKUP_PIN_STATE_HIGH);

        hw_wkup_register_interrupt(wake_interrupt_cb, 1);

        //config lis3dh
        i2c_init();

        for( ;; ) {
                /* Wait for the external interruption notification */
                OS_TASK_NOTIFY_WAIT(0x0, OS_TASK_NOTIFY_ALL_BITS, &ulNotifiedValue,OS_TASK_NOTIFY_FOREVER);

                /* Check the notification is the expected value */
                 if(ulNotifiedValue & 0x1){

                }
        }
}
