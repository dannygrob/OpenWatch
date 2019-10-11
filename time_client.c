#include "osal.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "svc_types.h"
#include "sys_watchdog.h"
#include "miniDB.h"
#include "time_client.h"
#include "displayDriver.h"
#include "sys_power_mgr.h"

PRIVILEGED_DATA static OS_TIMER cts_timer;

INITIALISED_PRIVILEGED_DATA static cts_current_time_t cts_time = {
        .date_time = {
                .year    = 1970,
                .month   = 1,
                .day     = 1,
                .hours   = 0,
                .minutes = 0,
                .seconds = 0,
        },
        .day_of_week = 4,
        .fractions_256 = 0,
        .adjust_reason = CTS_AR_NO_CHANGE,
};

static void on_timer_cb(OS_TIMER timer)
{
        cts_time.date_time.seconds++;

        if (cts_time.date_time.seconds > 59) {
                cts_time.date_time.seconds -= 60;
                cts_time.date_time.minutes += 1;
               // OS_TASK_NOTIFY(getDisplayTaskHandle(), SET_TIME_NOTIF, eSetBits);
        }
        if (cts_time.date_time.minutes > 59) {
                cts_time.date_time.minutes -= 60;
                cts_time.date_time.hours += 1;
             //   OS_TASK_NOTIFY(getDisplayTaskHandle(), SET_TIME_NOTIF, eSetBits);
        }
        if (cts_time.date_time.hours > 23) {
                cts_time.date_time.hours -= 24;
        }

        if (pm_get_sleep_mode() != pm_mode_extended_sleep) {
                OS_TASK_NOTIFY(getDisplayTaskHandle(), SET_TIME_NOTIF, eSetBits);
        }

}

void initTimeClient() {
        if (cts_timer) {
                OS_TIMER_START(cts_timer, OS_TIMER_FOREVER);
        } else {
                cts_timer = OS_TIMER_CREATE("timeclient", portCONVERT_MS_2_TICKS(1000), OS_TIMER_SUCCESS,
                                                        (void *) OS_GET_CURRENT_TASK(), on_timer_cb);
        }
        OS_ASSERT(cts_timer);
        OS_TIMER_START(cts_timer, OS_TIMER_FOREVER);
}

void stopTimeClient() {
        OS_TIMER_STOP(cts_timer, 1);
}

void setHour(int hr) {
        cts_time.date_time.hours = hr;
}
void setMinute(int min) {
        cts_time.date_time.minutes = min;
}
void setSecond(int sec) {
        cts_time.date_time.seconds = sec;
}
void setDate(char *date) {
        //20190924T000907
        char hrstr[2];
        char mnstr[2];
        char scstr[2];
        strncpy(hrstr, date + 9, 2);
        strncpy(mnstr, date + 11, 2);
        strncpy(scstr, date + 13, 2);

        setHour(atoi(hrstr));
        setMinute(atoi(mnstr));
        setSecond(atoi(scstr));
}
cts_current_time_t getTime() {
        return cts_time;
}
int getHour() {
        return cts_time.date_time.hours;
}
int getMinute() {
        return cts_time.date_time.minutes;
}
int getSecond() {
        return cts_time.date_time.seconds;
}
