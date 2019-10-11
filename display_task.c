/*
 * display_task.c
 *
 *  Created on: Jan 15, 2019
 *      Author: samsonm
 */

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
#include "displayDriver.h"
#include "displayFonts.h"
#include "watchAnimations.h"
#include "ad_spi.h"
#include "miniDB.h"
#include "imageOffsets.h"
#include "cts.h"
#include "display_task.h"
#include "time_client.h"
#include "ad_gpadc.h"
#include "sys_socf.h"
#include "sys_power_mgr.h"


#define SCREEN_NONE                     -1
#define SCREEN_BOOTSCREEN               1
#define SCREEN_WATCHFACE                2
#define SCREEN_UPDATE_WATCHFACE         3
#define SCREEN_NOTIFICATION             4
#define SCREEN_CHARGE                   5
#define SCREEN_TEST                     6

PRIVILEGED_DATA bool app_task_is_initialized;


#define RCV_USB_NONE 1
#define RCV_USB_ATTACH 2
#define RCV_USB_DETACH 3
#define RCV_USB_CHARGING 4
#define RCV_USB_PRECHARGING 5
#define RCV_USB_CHARGED 6
#define RCV_USB_FULL 7

int app_usb_indication = 0;

PRIVILEGED_DATA static OS_TIMER wake_timer;
static int defaultSeconds = 10;
static int timeoutSeconds = -1;

int currentScreen = SCREEN_NONE;

int queueItem = -1;
int displayQueue[20] = {-1};
int touched = 0;

uint8_t device_state = 0;
//NOTE: IN hw_spi.c
//REG_SET_FIELD(CRG_PER, CLK_PER_REG, SPI_CLK_SEL, clk_per_reg_local, 0); // select SPI clock
//Needs to be changed to
//REG_SET_FIELD(CRG_PER, CLK_PER_REG, SPI_CLK_SEL, clk_per_reg_local, 1);// select SPI clock - DIV1 - system clock
//In order to use the PLL as the source clock for the SPI bus

void on_timer_cb()
{
       if (timeoutSeconds <= 0) {
               printf("timer not valid\r\n");
               return;
       }

       timeoutSeconds --;

       if (timeoutSeconds <= 0) {
               printf("timer finished\n\r");
               clearQueue();
               stopWakeupTimeout();
               displayPower(0);
       }
       printf("on timeout %i\n\r",timeoutSeconds );
}

static void wake_timer_cb(OS_TIMER timer)
{
       timeoutSeconds --;

       if (timeoutSeconds <= 0) {
               printf("timer finished\n\r");
               clearQueue();
               stopWakeupTimeout();
               displayPower(0);
       }
       printf("on timeout %i\n\r",timeoutSeconds );
}

static void startWakeupTimeout() {
        if (timeoutSeconds > 0) {//OS_TIMER_IS_ACTIVE(wake_timer)) {
                printf("restarting timeout timer\n\r");
                timeoutSeconds = defaultSeconds;
        } else {
                timeoutSeconds = defaultSeconds;
                displayPowerBrightness(1, 195);
        }
        device_state = 1;
        /*OS_TIMER wake = OS_TIMER_GET_TIMER_ID(wake_timer);

        if (wake) {
              OS_TIMER_START(wake_timer, OS_TIMER_FOREVER);
              printf("reusing timeout timer\n\r");
        } else {
              wake_timer = OS_TIMER_CREATE("waketimer", portCONVERT_MS_2_TICKS(1000), OS_TIMER_SUCCESS, (void *) getDisplayTaskHandle(), wake_timer_cb);
              OS_ASSERT(wake_timer);
              OS_TIMER_START(wake_timer, OS_TIMER_FOREVER);
              printf("starting timeout timer\n\r");
        }*/
     //  timeoutSeconds = defaultSeconds;



}

static void stopWakeupTimeout() {
      // OS_TIMER_STOP(wake_timer,5);
       printf("stopping timeout timer\n\r");
       device_state = 0;
}

void addToQueue(int displayItem) {
        if (queueItem == 20) {
                //queue is full
                return;
        }
        queueItem ++;
        displayQueue[queueItem] = displayItem;

}

void finishedDisplay() {
        //we've finished displaying the screen, move to next in queue
        //if ( queueItem >= 0) { display(); }
}

bool canDisplay = true;

void clearQueue() {
        for(int i=0;i<20-1;i++) {
                displayQueue[i] = -1;
        }
        queueItem = -1;
}

void display() {
        canDisplay = false;
        printf("a\r\n");
        int nextDisplayItem = displayQueue[0];

        if (nextDisplayItem == SCREEN_NONE || queueItem < 0) {
                //TODO: Start display timeout?
                clearQueue();
                canDisplay = true;
                return;
        } else if (nextDisplayItem != SCREEN_UPDATE_WATCHFACE) {
                //reset the display timeout if we're not just updating
               // startWakeupTimeout();
        }

        printf("b\r\n");

        switch (nextDisplayItem) {
                case SCREEN_BOOTSCREEN:
                        printf("display boot\r\n");
                        display_bootscreen();
                        break;
                case SCREEN_WATCHFACE:
                        printf("display watchface\r\n");
                        display_watchface(false);
                        break;
                case SCREEN_NOTIFICATION:
                        printf("display notification\r\n");
                        display_notification();
                        break;
                case SCREEN_CHARGE:
                        printf("display charge\r\n");
                        display_charging();
                        break;
                case SCREEN_UPDATE_WATCHFACE:
                        printf("update watchface\r\n");
                        display_watchface(true);
                        break;
                case SCREEN_TEST:
                        test_draw();
                        printf("display test\r\n");
                        break;
                default:

                break;

        }
        printf("c\r\n");

        //shift items to the left
        for(int i=0;i<20-1;i++)
        {
                displayQueue[i]=displayQueue[i+1];
        }
        displayQueue[20-1]=-1;

        queueItem --;

        if ( queueItem >= 0) { display(); }

        canDisplay = true;
}

void refreshCurrentScreen() {
        if (canDisplay == false) {
                return;
        }
        switch (currentScreen) {
                case SCREEN_WATCHFACE:
                        if (queueItem == -1 && timeoutSeconds > 1) {
                                addToQueue(SCREEN_UPDATE_WATCHFACE);
                                display();
                        }
                        break;
                default:
                        break;
        }
}
int currentMin = -1;
int currentHour = -1;
int tickColor = 0;
int tickStep = 0;
int tickCounter = 0;
void display_watchface(bool updateOnly) {
        if (updateOnly == false) {
                displayClearBuf();
                tickStep = 0;
                tickColor = display24to16Color(0x8eaaad);

              //  openDevice();
                displayImageFromMemory(0,0,WATCHFACE_OFFSET);

                //test_draw();
               /* int max = (getSecond() + 1) * 6;

                for (int i=0;i<max;i+=6) {
                        int angle = i - 90;
                        displayDrawLinePolarThicknessShort(WATCH_CENTER,WATCH_CENTER, WATCH_CENTER, angle, 11, tickColor, 1);
                }
                closeDevice();*/
                currentMin = -1;
                currentHour = -1;
        } else {
                if (currentScreen != SCREEN_WATCHFACE) {
                        //don't update the watchface if we are not showing it
                     finishedDisplay();
                     return;
                }
        }

        int padding = 5;
        int yoffset = 65;
        int blockWidth = 40;
        int startX = 110-40 + 8;

        int hr = getHour();
        int min = getMinute();
        int sec = getSecond();

        openDevice();
        if (hr != currentHour) {
                displayDrawRectangleBuf(startX, startX+80, yoffset, yoffset+50, display24to16Color(0x000000));

                char h[2];
                sprintf(h, "%02d", hr);
                int width1 = displayGetWidthCharacter(h[0], true);
                int width2 = displayGetWidthCharacter(h[1], true);
                displayDrawCharacterNumber(startX + ((blockWidth - width1) / 2), yoffset, h[0], true);
                displayDrawCharacterNumber((startX + blockWidth) + ((blockWidth - width2) / 2), yoffset, h[1], true);
                currentHour = hr;
        }

        if (min != currentMin) {
                displayDrawRectangleBuf(startX, startX+80, yoffset + 50, yoffset+100, display24to16Color(0x000000));

                char m[2];
                sprintf(m, "%02d", min);
                int width3 = displayGetWidthCharacter(m[0], true);
                int width4 = displayGetWidthCharacter(m[1], true);
                displayDrawCharacterNumber(startX + ((blockWidth - width3) / 2), yoffset + 50 + padding, m[0], true);
                displayDrawCharacterNumber((startX + blockWidth) + ((blockWidth - width4) / 2), yoffset + 50 + padding, m[1], true);
                currentMin = min;
        }

        //display battery percentage
       char str[5];

#ifndef DEVKIT_USB
       uint8_t level = (socf_get_soc() + 5) / 10;
      // sprintf(str, "%d", level);
       int width = 0.16 * level;

#else
       int width = 5;
#endif

       int battery_y = 180;
       int battery_x = 110;

       if (updateOnly == false) {
                displayDrawStringCentered(battery_y + 15 ,2,0, str);

                displayDrawRectangleBuff(battery_x, battery_x + 20, battery_y, battery_y + 10, display24to16Color(0x003636));
                displayDrawLine(battery_x + 10,battery_x + 20,battery_y + 9,battery_y + 9,display24to16Color(0x003636));
                //tip
                displayDrawRectangleBuff(130, battery_x + 23, battery_y + 2, battery_y + 9, display24to16Color(0x003636));
                displayDrawLine(battery_x + 20,battery_x + 23,battery_y + 6,battery_y + 6,display24to16Color(0x003636));
                //fill


                displayDrawRectangleBuff(battery_x + 3 + width -1, battery_x + 18, battery_y + 2, battery_y + 8, display24to16Color(0x000000));
                displayDrawLine(battery_x + 3 + width-1,battery_x + 18,battery_y + 7,battery_y + 7,display24to16Color(0x000000));


        }
       // char str2[2];
        //sprintf(str2, "%d", touched);
        //displayDrawStringCentered(50 ,2,0, str2);

        if (app_usb_indication != RCV_USB_CHARGING) {
                displayDrawRectangleBuff(battery_x + 2, battery_x + 2 + width, battery_y + 2, battery_y + 8, display24to16Color(0x8eaaad));
                displayDrawLine(battery_x + 2,battery_x + 3 + width-1,battery_y + 7,battery_y + 7,display24to16Color(0x8eaaad));
        } else {
               displayDrawRectangleBuff(battery_x + 2, battery_x + 2 + width, battery_y + 2, battery_y + 8, display24to16Color(0xFF0000));
               displayDrawLine(battery_x + 2,battery_x + 3 + width-1,battery_y + 7, battery_y + 7,display24to16Color(0xFF0000));
        }

        if (sec == 0) {
                if (tickStep == 1) {
                        tickColor = display24to16Color(0x8eaaad);
                        tickStep = 0;
                } else {
                        tickColor = display24to16Color(0x003636);
                        tickStep = 1;
                }
        }
        if (tickColor == 0) {
                tickColor = display24to16Color(0x8eaaad);
        }
        int angle = getSecond() * 6;
        angle -= 90;
        displayDrawLinePolarThicknessShort(WATCH_CENTER,WATCH_CENTER, WATCH_CENTER, angle, 11, tickColor, 2);
        closeDevice();
        currentScreen = SCREEN_WATCHFACE;
        finishedDisplay();
}

void getBatt() {
        int level = read_battery_level();
}

void test_draw() {
        openDevice();
        displayClearBuf();
        int tickColor;

        if (tickStep == 0) {
                tickColor = display24to16Color(0x8eaaad);
        } else {
                tickColor = display24to16Color(0x003636);
        }
        int max = (getSecond() + 1) * 6;
        for (int i=0;i<max;i+=6) {
                int angle = i - 90;
                displayDrawLinePolarThicknessShort(WATCH_CENTER,WATCH_CENTER, WATCH_CENTER, angle, 11, tickColor, 2);
        }
        setANCSTitle("Test voor rare dingen.");
        setANCSMessage("ABCDEFGHIJ KLMNOPQRSTUV WXYZ abcdefghij klmnopqrstuvwx yz1234567890 \"");
        displayDrawStringCentered(20,2,0, getANCSTitle());//Ends at 35+10+5+10+5 = 65
        displayDrawStringCentered(80,2,0, getANCSMessage());
        closeDevice();

        OS_DELAY_MS(3000);

        finishedDisplay();
}

void display_notification(){
        currentScreen = SCREEN_NOTIFICATION;
        displayClearBuf();
        buzz(200);
        displayDrawStringCentered(20,2,0, getANCSTitle());//Ends at 35+10+5+10+5 = 65
        displayDrawStringCentered(80,2,0, getANCSMessage());
        OS_DELAY_MS(5000);
        finishedDisplay();
}

void display_charging() {
        displayClearBuf();
        buzz(100);
        displayImageFromMemory(0,0,CHARGING_OFFSET);
        OS_DELAY_MS(2000);
        finishedDisplay();
}

void display_bootscreen() {
        currentScreen = SCREEN_BOOTSCREEN;
        displayImageFromMemory(0,0,ZBOOTSCREEN_OFFSET);
        OS_DELAY_MS(2000);
        finishedDisplay();
}

void display_menu() {

}
void usb_attach_cb(void)
{
        if (app_task_is_initialized) {
                // Do something
                if (app_usb_indication != RCV_USB_ATTACH) {
                       app_usb_indication = RCV_USB_ATTACH;
                       OS_TASK_NOTIFY(getDisplayTaskHandle(), CHARGE_STATE_MASK, eSetBits);
                }
        } else {
                // Raise a flag for the app to check when started.
                app_usb_indication = RCV_USB_ATTACH;
        }
}

void usb_detach_cb(void)
{
        if (app_task_is_initialized) {
                // Do something
                if (app_usb_indication != RCV_USB_DETACH) {
                       app_usb_indication = RCV_USB_DETACH;
                       OS_TASK_NOTIFY(getDisplayTaskHandle(), CHARGE_STATE_MASK, eSetBits);
                }


        } else {
                // Raise a flag for the app to check when started.
                app_usb_indication = RCV_USB_DETACH;
        }
}

void usb_charging(void) {
        if (app_task_is_initialized) {
                // Do something
                if (app_usb_indication != RCV_USB_CHARGING) {
                       app_usb_indication = RCV_USB_CHARGING;
                       OS_TASK_NOTIFY(getDisplayTaskHandle(), CHARGE_STATE_MASK, eSetBits);
                }

        } else {
                // Raise a flag for the app to check when started.
                app_usb_indication = RCV_USB_CHARGING;
        }
}

void display_task(void *params)
{
        setDisplayTaskHandle(OS_GET_CURRENT_TASK());

        app_usb_indication = RCV_USB_NONE;
        initTimeClient();


#ifdef CONFIG_SPI_DISPLAY
        printf("init display1\r\n");
        ad_spi_init();

        displayInit();

        addToQueue(SCREEN_BOOTSCREEN);
        addToQueue(SCREEN_WATCHFACE);

#endif

        startWakeupTimeout();
        display();
        app_task_is_initialized = true;
        for(;;)
        {
                OS_BASE_TYPE ret;
                uint32_t notif;

                ret = OS_TASK_NOTIFY_WAIT(0, OS_TASK_NOTIFY_ALL_BITS, &notif, OS_TASK_NOTIFY_FOREVER);
                OS_ASSERT(ret == OS_OK);
                printf("got notified on loop %i \r\n", notif);

                /* Notified from BLE manager, can get event */
                if (notif & SHOW_NOTIFICATION_MASK)
                {
                        printf("notification\r\n");
                        startWakeupTimeout();
#ifdef CONFIG_SPI_DISPLAY
                        addToQueue(SCREEN_NOTIFICATION);
                        addToQueue(SCREEN_WATCHFACE);
                        display();
#endif
                }

                if (notif & SET_TIME_NOTIF) {
                        printf("time\r\n");
#ifdef CONFIG_SPI_DISPLAY
                        refreshCurrentScreen();
#endif
                        on_timer_cb();
                }

                if (notif & UPDATE_TOUCH_MASK) {

                        touched = 1;
                        if (device_state == 0 && currentScreen == SCREEN_WATCHFACE) {
                                addToQueue(SCREEN_WATCHFACE);
                                startWakeupTimeout();
                                display();
                        }

                        uint8_t *touchPoints = getTouch();
                        printf("touch %i %i\r\n", touchPoints[0], touchPoints[1]);
                        displayDrawPixel(touchPoints[0], touchPoints[1],  display24to16Color(0x003636));
                }

                if (notif & UPDATE_ACCEL_MASK) {
                        printf("accel\r\n");
                        startWakeupTimeout();
                }

                if (notif & CHARGE_STATE_MASK) {
                        printf("charge state\r\n");
                        startWakeupTimeout();
                        if (app_usb_indication == RCV_USB_CHARGING) {
#ifdef CONFIG_SPI_DISPLAY
                                addToQueue(SCREEN_CHARGE);
                                addToQueue(SCREEN_WATCHFACE);
                                display();
#endif
                        }
                }
        }
}
