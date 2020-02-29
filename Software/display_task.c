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
#define SCREEN_MENU                     6

#define SCREEN_TEST                     99

PRIVILEGED_DATA bool app_task_is_initialized;


#define RCV_USB_NONE 1
#define RCV_USB_ATTACH 2
#define RCV_USB_DETACH 3
#define RCV_USB_CHARGING 4
#define RCV_USB_PRECHARGING 5
#define RCV_USB_CHARGED 6
#define RCV_USB_FULL 7

int app_usb_indication = 0;

//PRIVILEGED_DATA static OS_TIMER wake_timer;
static int defaultSeconds = 10;
static int timeoutSeconds = -1;

int currentScreen = SCREEN_NONE;

int queueItem = -1;
int displayQueue[10] = {-1};
int touched = 0;

unsigned int menu_page = 0;

uint8_t device_state = 0;

char *months[12] =
{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
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
       //printf("on time-out %i\n\r",timeoutSeconds );
}

//static void wake_timer_cb(OS_TIMER timer)
//{
//       timeoutSeconds --;
//
//       if (timeoutSeconds <= 0) {
//               printf("timer finished\n\r");
//               clearQueue();
//               stopWakeupTimeout();
//               displayPower(0);
//       }
//       printf("on timeout %i\n\r",timeoutSeconds );
//}

static void startWakeupTimeout() {
        if (timeoutSeconds > 0) {//OS_TIMER_IS_ACTIVE(wake_timer)) {
                printf("restarting time-out timer\n\r");
                timeoutSeconds = defaultSeconds;
        } else {
                printf("starting time-out timer\n\r");
                timeoutSeconds = defaultSeconds;
                displayPowerBrightness(1, 195);
        }
        device_state = 1;
}

static void stopWakeupTimeout() {
      // OS_TIMER_STOP(wake_timer,5);
       printf("stopping time-out timer\n\r");
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

bool canDisplay = true;

void clearQueue() {
        for(int i=0;i<10;i++) {
                displayQueue[i] = -1;
        }
        queueItem = -1;
}

void display() {
        canDisplay = false;

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


        switch (nextDisplayItem) {
                case SCREEN_BOOTSCREEN:
                        printf("display boot\r\n");
                        display_bootscreen();
                        break;
                case SCREEN_WATCHFACE:
                        printf("display watchface\r\n");
                        display_watchface(false);
                        printf("display watchfacez\r\n");
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
                        display_watchface(true);
                        break;
                case SCREEN_MENU:
                        display_menu();
                        printf("display menu\r\n");
                        break;
                default:

                break;

        }
        printf("shift\r\n");
        logQueue();
        //shift items to the left
        for(int i=0;i<9;i++)
        {
                displayQueue[i]=displayQueue[i+1];
        }

        displayQueue[9]=-1;

        queueItem --;

        logQueue();
        if ( displayQueue[0] != SCREEN_NONE) {
                canDisplay = true;
                display();
        }
        canDisplay = true;


}

void logQueue() {
        printf("queue:");
        for(int i=0;i<9;i++)
        {
                printf("%i", displayQueue[i]);
        }
        printf("\r\n");
}
bool touchInRect(int touchx, int touchy, int x, int y, int w, int h) {
        if (touchx > x && touchx < (x+w) && touchy > y && touchy < (y+h)) {
             return true;
        }
        return false;
}
void touchedCurrentScreen(uint8_t x, uint8_t y) {
        bool touchedAction = touchInRect(x, y, 80, 80, 80, 80);
        bool touchedback = touchInRect(x, y, 10, 80, 40, 80);
        bool touchednext = touchInRect(x, y, 200, 80, 40, 80);
        switch (currentScreen) {
                case SCREEN_WATCHFACE:
                        //temp
                        if (touchInRect(x, y, 150, 122, 50, 50)) {
                                addToQueue(SCREEN_MENU);
                                display();
                        }
                        break;
                case SCREEN_MENU:
                       if (menu_page == 0) {//messages
                                if (touchedAction) {
                                        addToQueue(SCREEN_NOTIFICATION);
                                        addToQueue(SCREEN_MENU);
                                        display();
                                }
                        } if (menu_page == 1) {//timer
                                if (touchedAction) {

                                }
                        } if (menu_page == 2) {//back
                                if (touchedAction) {
                                        display_watchface(false);
                                }
                        }
                        //arrows
                        if (touchedback) {
                                printf("back menu");
                                menu_page -= 1;
                                if (menu_page < 0){menu_page = 2;}
                                set_menu_page(menu_page);
                        } else if (touchednext) {
                                printf("next menu");
                                menu_page += 1;
                                if (menu_page > 2){menu_page = 0;}
                                set_menu_page(menu_page);
                        }

                        break;
                default:
                        break;
        }
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
                displayImageFromMemory(0,0,WATCHFACE_OFFSET);
                currentMin = -1;
                currentHour = -1;
        } else {
                if (currentScreen != SCREEN_WATCHFACE) {
                     //don't update the watchface if we are not showing it
                     return;
                }
        }

        int padding = 5;
        int yoffset = 60;
        int blockWidth = 40;
        int startX = 35;//110-40 + 8;

        int hr = getHour();
        int min = getMinute();
        bool bold = false;
        openDevice();
        if (hr != currentHour || min != currentMin) {

                char h[2];
                sprintf(h, "%02d", hr);
                char m[2];
                char semi[1];
                semi[0]= ':';
                sprintf(m, "%02d", min);
                int width1 = displayGetWidthClockCharacter(h[0], bold)+padding;
                int width2 = displayGetWidthClockCharacter(h[1], bold)+padding;

                int width3 = displayGetWidthClockCharacter(semi[0], bold)+padding;

                int width4 = displayGetWidthClockCharacter(m[0], bold)+padding;
                int width5 = displayGetWidthClockCharacter(m[1], bold);

                int totalWidth = width1 + width2 + width3 + width4 + width5;
                int offset = (240 - totalWidth) / 2;

                displayDrawRectangleBuf(offset-20, offset+totalWidth+20, yoffset, yoffset+50, display24to16Color(0x000000));

                displayDrawClockNumber(offset, yoffset, h[0], bold);
                displayDrawClockNumber(offset + width1, yoffset, h[1], bold);
                displayDrawClockNumber(offset + width1 + width2, yoffset, semi[0], bold);
                displayDrawClockNumber(offset + width1 + width2 + width3, yoffset, m[0], bold);
                displayDrawClockNumber(offset + width1 + width2 + width3 + width4, yoffset, m[1], bold);
                currentHour = hr;
                currentMin = min;
        }

       char str[5];

#ifndef DEVKIT_USB //fake the battery level
       uint8_t level = (socf_get_soc() + 5) / 10;
       sprintf(str, "%d", level);
       //int width = 0.16 * level;

#else
       //int width = 5;
       sprintf(str, "%d", 35);
#endif

       int battery_y = 170;

       if (updateOnly == false) {
                displayDrawStringCentered(battery_y ,2,0, str);

                char d[2];
                sprintf(d, "%02d", getDay());

                displayDrawString(57, 129 ,2,0, d);
                displayDrawString(51, 145 ,2,0, months[getMonth()-1]);
                if (app_usb_indication != RCV_USB_CHARGING) {
                        displayDrawCircle(120, 180, 23, display24to16Color(0x1ba569));
                        displayDrawCircle(120, 180, 22, display24to16Color(0x1ba569));
                } else {
                        displayDrawCircle(120, 180, 23, display24to16Color(0x1b80a5));
                        displayDrawCircle(120, 180, 22, display24to16Color(0x1b80a5));
                }
        }

        closeDevice();
        currentScreen = SCREEN_WATCHFACE;
}

void display_notification(){
        currentScreen = SCREEN_NOTIFICATION;
        displayClearBuf();
        buzz(200);
        if (strlen(getANCSTitle()) >0){
                displayDrawStringCentered(20,2,0, getANCSTitle());//Ends at 35+10+5+10+5 = 65
                displayDrawStringCentered(80,2,0, getANCSMessage());
        } else {
                char title[12] = "No messages";
                char message[18] = "No messages found";
                displayDrawStringCentered(20,2,0, title);//Ends at 35+10+5+10+5 = 65
                displayDrawStringCentered(80,2,0, message);
        }
        OS_DELAY_MS(5000);
}

void display_charging() {
        displayClearBuf();
        buzz(100);
        displayImageFromMemory(0,0,CHARGING_OFFSET);
        OS_DELAY_MS(2000);
}

void display_bootscreen() {
        currentScreen = SCREEN_BOOTSCREEN;
        displayImageFromMemory(0,0,ZBOOTSCREEN_OFFSET);
        OS_DELAY_MS(2000);
}

void display_menu() {
        currentScreen = SCREEN_MENU;
        displayFillScreenBuf(display24to16Color(0x42d5ad));
        //left
        displayPartialImageFromMemory(10,111,1, 131, 18,18,MENU_ICONS_OFFSET);
        //right
        displayPartialImageFromMemory(212,111,22, 131, 18,18,MENU_ICONS_OFFSET);
        //messages
        set_menu_page(menu_page);
}
void set_menu_page(unsigned int page) {
        displayDrawRectangleBuff(90,160,90,155,display24to16Color(0x42d5ad));

        menu_page = page;

        if (page == 0) {
                //message
                displayPartialImageFromMemory(90,90,130, 0, 65,58,MENU_ICONS_OFFSET);
        } else if (page == 1) {
                //timer
                displayPartialImageFromMemory(90,90,67, 0, 62,63,MENU_ICONS_OFFSET);
        } else if (page == 2) {
                //back
                displayPartialImageFromMemory(90,95,3, 5, 57,51,MENU_ICONS_OFFSET);
        }
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
#ifdef CONFIG_SPI_DISPLAY
                        refreshCurrentScreen();
#endif
                        on_timer_cb();
                }

                if (notif & UPDATE_WAKE_MASK) {
                       if (device_state == 0 && currentScreen == SCREEN_WATCHFACE) {
                               addToQueue(SCREEN_WATCHFACE);
                               startWakeupTimeout();
                               display();
                       } else {
                               //reset the timer
                               startWakeupTimeout();
                       }
                }

                if (notif & UPDATE_TOUCH_MASK) {
                        startWakeupTimeout();

                        uint8_t *touchPoints = getTouch();
                        printf("touch %i %i\r\n", touchPoints[0], touchPoints[1]);
                        touchedCurrentScreen(touchPoints[0], touchPoints[1]);
                }

                if (notif & UPDATE_ACCEL_MASK) {
                        printf("accel\r\n");
                        startWakeupTimeout();
                }

                if (notif & CHARGE_STATE_MASK) {
                        printf("charge state %i\r\n",app_usb_indication);

                        if (app_usb_indication == RCV_USB_CHARGING) {
#ifdef CONFIG_SPI_DISPLAY
                                startWakeupTimeout();
                                addToQueue(SCREEN_CHARGE);
                                addToQueue(SCREEN_WATCHFACE);
                                display();
#endif
                        }
                }
        }
}
