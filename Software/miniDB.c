/*
 * miniDB.c
 *
 *  Created on: Jan 22, 2019
 *      Author: samsonm
 */

#include "miniDB.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "platform_devices.h"
TaskHandle_t DisplayTaskHandle;
TaskHandle_t ANCSTaskHandle;
TaskHandle_t LIS3DHTaskHandle;
TaskHandle_t TouchTaskHandle;

char titleBuffer[50];
char messageBuffer[250];
bool imageLoaderIsDone;

uint8_t touchPoints[2] = {0x00,0x00};

notification_t *lastNotif;

void setDisplayTaskHandle(TaskHandle_t TASK_HANDLE)
{
        DisplayTaskHandle = TASK_HANDLE;
}
TaskHandle_t getDisplayTaskHandle()
{
        return DisplayTaskHandle;
}
void setLis3DHTaskHandle(TaskHandle_t TASK_HANDLE)
{
        LIS3DHTaskHandle = TASK_HANDLE;
}
TaskHandle_t getLis3DHTaskHandle()
{
        return LIS3DHTaskHandle;
}
void setANCSTaskHandle(TaskHandle_t TASK_HANDLE)
{
        ANCSTaskHandle = TASK_HANDLE;
}
TaskHandle_t getANCSTaskHandle()
{
        return ANCSTaskHandle;
}
void setANCSTitle(char TITLE[])
{
        strncpy(titleBuffer,TITLE,50);
}
char * getANCSTitle(void)
{
        return titleBuffer;
}
void setANCSMessage(char MESSAGE[])
{
        strncpy(messageBuffer,MESSAGE,255);
}
char * getANCSMessage(void)
{
        return messageBuffer;
}
void setTouchTaskHandle(TaskHandle_t TASK_HANDLE)
{
        TouchTaskHandle = TASK_HANDLE;
}
TaskHandle_t getTouchTaskHandle()
{
        return TouchTaskHandle;
}
void setImageLoaderComplete(bool IS_SET)
{
        imageLoaderIsDone = IS_SET;
}
bool getImageLoaderComplete(void)
{
        return imageLoaderIsDone;
}
void setLastNotification(notification_t *notif) {
        lastNotif = notif;
}
notification_t *getLastNotification() {
        return lastNotif;
}
void clearLastNotification() {
        lastNotif = NULL;
}
void setTouch(uint8_t x, uint8_t y) {
        touchPoints[0] = x;
        touchPoints[1] = y;
}
uint8_t *getTouch() {
        return touchPoints;
}
void initLogging() {
        uart_device uartDev;
        ad_uart_init();
        uartDev = ad_uart_open(SERIAL1);
}
