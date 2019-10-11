/*
 * display_task.h
 *
 *  Created on: 24 Sep 2019
 *      Author: Nitwit
 */

#ifndef DISPLAY_TASK_H_
#define DISPLAY_TASK_H_

static void stopWakeupTimeout();
static void startWakeupTimeout();
void display_watchface(bool updateOnly);
void display_bootscreen();
void display_notification();
void display_charging();
void display_menu();

#endif /* DISPLAY_TASK_H_ */
