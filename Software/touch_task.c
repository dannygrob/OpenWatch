/*
 * touch_task.c
 *
 *  Created on: 05 oct 2019
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
#include "touchDriver.h"

#define mainCOUNTER_FREQUENCY_MS                OS_MS_2_TICKS(1000)

unsigned int didFirst = 0;


void touch_wake_interrupt_cb(void)
{
        hw_wkup_reset_interrupt();
       // if (didFirst == 1) {
              //  OS_TASK_NOTIFY(getDisplayTaskHandle(), UPDATE_TOUCH_MASK, OS_NOTIFY_SET_BITS);
                OS_TASK_NOTIFY(getTouchTaskHandle(), 0x1, OS_NOTIFY_SET_BITS);
       // }
       // else {
                didFirst = 1;
      //  }
}

void touch_task(void *params)
{
        setTouchTaskHandle(OS_GET_CURRENT_TASK());
        uint32_t ulNotifiedValue;

        initLogging();

        printf("log");

        /* Initialize and configure the Wake-up Timer */
        hw_wkup_init(NULL);
#ifdef DEVKIT_USB
        hw_wkup_configure_pin(HW_GPIO_PORT_3, HW_GPIO_PIN_1, 1, HW_WKUP_PIN_STATE_LOW);
        hw_wkup_configure_pin(HW_GPIO_PORT_1, HW_GPIO_PIN_4, 1, HW_WKUP_PIN_STATE_LOW);
#else
        hw_wkup_configure_pin(HW_GPIO_PORT_3, HW_GPIO_PIN_2, 1, HW_WKUP_PIN_STATE_LOW);
#endif

        printf("start3");
        hw_wkup_register_interrupt(touch_wake_interrupt_cb, 1);

        touch_init();
        printf("start4");
        for( ;; ) {

                printf("start");
                /* Wait for the external interruption notification */
                OS_TASK_NOTIFY_WAIT(0x0, OS_TASK_NOTIFY_ALL_BITS, &ulNotifiedValue,OS_TASK_NOTIFY_FOREVER);

                /* Check the notification is the expected value */
                 if(ulNotifiedValue & 0x1){
                         /* Send the charaacter on the UART */
                        pollTouch();
                }
        }
}
