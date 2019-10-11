/*
 * storeInFlash_task.c
 *
 *  Created on: Jan 23, 2019
 *      Author: samsonm
 */

#if LOAD_NEW_IMAGES

#include "osal.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/list.h"
#include "util/queue.h"
#include "svc_types.h"
#include "hw_wkup.h"
#include "sys_watchdog.h"
#include "platform_devices.h"
#include "sys_power_mgr.h"
#include "ad_nvms.h"
#include "miniDB.h"

#define UART_RECEIVE_BUFFER_LENGTH 1
#define MEMORY_BUFFER_SIZE 128

char buf[UART_RECEIVE_BUFFER_LENGTH];


void storeInFlash_task(void *params)
{
        setImageLoaderComplete(false);

        uint8_t memoryBuffer[MEMORY_BUFFER_SIZE]={0xF0, 0xF0};

        uart_device uartDev;
        ad_uart_init();
        uartDev = ad_uart_open(SERIAL2);

        ad_uart_write(uartDev,"\r\nHello\n\r",9);

        ad_nvms_init();
        nvms_t flashMemory = ad_nvms_open(NVMS_FLASH_STORAGE);
        ad_uart_write(uartDev,"\r\nINITE\n\r",9);
        //read byte 0
        uint8_t sizeOfImageBuffer[2]={0};
       // ad_flash_unlock();
        ad_nvms_read(flashMemory, 0, (uint8 *) sizeOfImageBuffer, sizeof(sizeOfImageBuffer));
        ad_uart_write(uartDev,"\r\nDid read ",11);
        ad_uart_write(uartDev, (uint8 *) sizeOfImageBuffer, sizeof(sizeOfImageBuffer));
        ad_uart_write(uartDev,"--",2);
        ad_nvms_write(flashMemory, 0, (uint8_t *) memoryBuffer, sizeof(memoryBuffer));
        //ad_flash_lock();
        ad_uart_write(uartDev,"\r\nReady for new data\r\n",22);
}
void storeInFlash_taska(void *params)
{

        setImageLoaderComplete(false);

        uint8_t memoryBuffer[MEMORY_BUFFER_SIZE]={0};
        uint8_t test = 0xF0;
        uart_device uartDev;
        ad_uart_init();
        uartDev = ad_uart_open(SERIAL2);

        OS_TICK_TIME xNextWakeTime;

        xNextWakeTime = OS_GET_TICK_COUNT();

        OS_DELAY_MS(1000);

        /* Initialize NVMS */
        ad_nvms_init();
        nvms_t flashMemory = ad_nvms_open(NVMS_GENERIC_PART);
        int characterCounter = 0;
        int currentAddress = 0;

        ad_uart_write(uartDev,"\r\nReady for new data\r\n",22);
        for(;;)
        {
                if(!getImageLoaderComplete())
                {
                        if(ad_uart_read(uartDev,&buf,UART_RECEIVE_BUFFER_LENGTH,10000))
                        {
                               // ad_uart_write(uartDev,buf,UART_RECEIVE_BUFFER_LENGTH);
                                if(characterCounter>=MEMORY_BUFFER_SIZE)
                                {
                                        ad_nvms_write(flashMemory, currentAddress, (uint8_t *) memoryBuffer, MEMORY_BUFFER_SIZE);
                                        characterCounter = 0;
                                        currentAddress+=MEMORY_BUFFER_SIZE;
                                        ad_uart_write(uartDev,"INMEM",5);
                                }
                                memoryBuffer[characterCounter] = buf[0];
                                characterCounter++;
                        }
                        else
                        {
                                if(characterCounter>0)
                                {
                                        ad_nvms_write(flashMemory, currentAddress, (uint8_t *) memoryBuffer, MEMORY_BUFFER_SIZE);
                                }
                                ad_uart_write(uartDev,"\r\nTransmission Finished\r\n",25);
                                setImageLoaderComplete(true);
                        }
                }
        }
}



#endif
