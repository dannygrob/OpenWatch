/*
 * display_task.h
 *
 *  Created on: 24 Sep 2019
 *      Author: Nitwit
 */

#ifndef ANCS_TASK_H_
#define ANCS_TASK_H_

#include "ancs_client.h"
#include "ble_att.h"
#include "ble_common.h"
#include "ble_gap.h"
#include "ble_gatts.h"
#include "bas.h"
#include "cts.h"
#include "dis.h"
#include "scps.h"

typedef struct {
        void *next;

        uint32_t uid;

        ancs_notification_data_t data;

        char *app_id;
        char *date;
        char *title;
        char *message;
} notification_t;

#endif
