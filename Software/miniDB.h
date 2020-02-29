/*
 * miniDB.h
 *
 *  Created on: Jan 22, 2019
 *      Author: samsonm
 */

#ifndef MINIDB_H_
#define MINIDB_H_

#include "osal.h"
#include "ble_common.h"
#include "ble_gatts.h"
#include "ancs_task.h"

void            setDisplayTaskHandle(TaskHandle_t TASK_HANDLE);
TaskHandle_t    getDisplayTaskHandle();
void            setANCSTaskHandle(TaskHandle_t TASK_HANDLE);
TaskHandle_t    getANCSTaskHandle();
void            setLis3DHTaskHandle(TaskHandle_t TASK_HANDLE);
TaskHandle_t    getLis3DHTaskHandle();
void            setTouchTaskHandle(TaskHandle_t TASK_HANDLE);
TaskHandle_t    getTouchTaskHandle();

void            setANCSTitle(char TITLE[]);
char *             getANCSTitle(void);

void            setANCSMessage(char MESSAGE[]);
char *             getANCSMessage(void);

void            setImageLoaderComplete(bool IS_SET);
bool            getImageLoaderComplete(void);

#define SHOW_NOTIFICATION_MASK (1 << 5)
#define SET_TIME_NOTIF (1 << 3)
#define UPDATE_ACCEL_MASK (1 << 4)
#define CHARGE_STATE_MASK (1 << 6)
#define UPDATE_TOUCH_MASK (1 << 7)
#define UPDATE_WAKE_MASK (1 << 8)

#define DISPLAY_ON_MASK 22
#define DISPLAY_OFF_MASK 23

void setLastNotification(notification_t *notif);
notification_t *getLastNotification();
void clearLastNotification();

void setHour(int hr);
void setMinute(int min);
void setSecond(int sec);
void setDate(char *date);
int getHour();
int getMinute();
int getSecond();
void initLogging();
void setTouch(uint8_t x, uint8_t y);
uint8_t *getTouch();
#endif /* MINIDB_H_ */
