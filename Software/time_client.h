/*
 * time_client.h
 *
 *  Created on: 27 Sep 2019
 *      Author: Nitwit
 */

#ifndef TIME_CLIENT_H_
#define TIME_CLIENT_H_

#include "cts.h"

void setHour(int hr);
void setMinute(int min);
void setSecond(int sec);
int getDay();
int getMonth();
void setDate(char *date);
cts_current_time_t getTime();
void stopTimeClient();
void initTimeClient();
#endif /* TIME_CLIENT_H_ */
