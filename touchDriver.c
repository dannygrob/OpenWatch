#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <osal.h>
#include <platform_devices.h>
#include <hw_i2c.h>
#include <resmgmt.h>
#include "it7257Registers.h"
#include "touchDriver.h"
#include "miniDB.h"

i2c_device dev;

typedef struct i2c_event
{
        OS_EVENT os_event;
        HW_I2C_ABORT_SOURCE error;
} i2c_event;

/*
 * Signaling event used in operation started from GUI task.
 */
static i2c_event i2c_signal;

/*
 * Initialize event structure used in this demo.
 */
static void init_event(i2c_event *event)
{
        event->error = 0;
        OS_EVENT_CREATE(event->os_event);
}

/*
 * Callback passed to i2c_async_transact. It signals event passed as user data.
 */
static void signal_event(void *user_data, HW_I2C_ABORT_SOURCE error)
{
        i2c_event *event = (i2c_event *) user_data;
        event->error = error;
        OS_EVENT_SIGNAL_FROM_ISR(event->os_event);
}

/*
 * Resets event used for synchronization.
 */
static void reset_event(i2c_event *event)
{
        event->error = 0;
        OS_EVENT_WAIT(event->os_event, OS_EVENT_NO_WAIT);
}

/*
 * Wait for event to be in signaled state.
 */
static void wait_for_event(i2c_event *event)
{
        OS_EVENT_WAIT(event->os_event, OS_EVENT_FOREVER);
}

/*
 * Write sensor register
 */
static inline void write_reg_async(i2c_device dev, uint8_t reg, const uint8_t *val,
                                                uint8_t len, ad_i2c_user_cb cb, void *user_data)
{

        ad_i2c_async_transact(dev, I2C_SND(&reg, 1),
                                I2C_SND_ST(val, len),
                                I2C_CB1(cb, user_data),
                                I2C_END);
}

/*
 * Read sensor register asynchronously
 */
static inline void read_reg_async(i2c_device dev, uint8_t reg, uint8_t *val, uint8_t len, ad_i2c_user_cb cb, void *user_data)
{
        /*
         * Reading sensor register consist of writing register number and reading it's contents.
         * Do this by preparing transaction that will generate write and read.
         * This demo uses callback that will signal event so task can be resumed after read.
         */
        ad_i2c_async_transact(dev, I2C_SND(&reg, 1),
                                I2C_RCV(val, len),
                                I2C_CB1(cb, user_data),
                                I2C_END);
}
/*
 * Write sensor register synchronously with user supplied event
 */
static int write_reg_event(i2c_device dev, uint8_t reg, const uint8_t *val, uint8_t len,
                                                                                i2c_event *event)
{

        reset_event(event);
        write_reg_async(dev, reg, val, len, signal_event, event);
        wait_for_event(event);
        return event->error;
}

static int read_reg_event(i2c_device dev, uint8_t reg, uint8_t *val, uint8_t len, i2c_event *event)
{
        reset_event(event);

        read_reg_async(dev, reg, val, len, signal_event, event);
        wait_for_event(event);

        return event->error;
}

bool IsBitSet(uint8_t num, int bit)
{
    return 1 == ( (num >> bit) & 1);
}



void query_busy() {
        uint8_t b = 0x01;

        dev = ad_i2c_open(IT7257); /* Open selected device */
        ad_i2c_device_acquire(dev);

        do {
          read_reg_event(dev, INDEX_CMD_READ, &b ,sizeof(b), &i2c_signal);
         // printf("busy\r\n");
        } while (b == 0x01);

        ad_i2c_device_release(dev);
        ad_i2c_close(dev);
}

void touch_init(void)
{
        printf("init touch\r\n");

        init_event(&i2c_signal);

        OS_DELAY_MS(10);
        query_busy();

        set_interrupt();

}


void whoami() {
       reset_event(&i2c_signal);

       uint8_t hello = 0x00;

       dev = ad_i2c_open(IT7257); // Open selected device
       ad_i2c_device_acquire(dev);
       write_reg_event(dev, INDEX_CMD_WRITE, &hello, sizeof(hello), &i2c_signal);

       uint8_t response[9] = {0x00};
       read_reg_event(dev, INDEX_CMD_READ, response, sizeof(response), &i2c_signal);
       ad_i2c_device_release(dev);
       ad_i2c_close(dev);

       printf("device: %c%c%c%c%c%c%c\n\r", response[1],response[2],response[3],response[4],response[5],response[6], response[7]);
}

void set_interrupt() {
        printf("set interrupt\r\n");
        uint8_t pucData[4];
        pucData[0] = 0x02;
        pucData[1] = 0x04;

        reset_event(&i2c_signal);
        OS_DELAY_MS(10);
        query_busy();
        OS_DELAY_MS(10);

        dev = ad_i2c_open(IT7257); /* Open selected device */

        ad_i2c_device_acquire(dev);

        write_reg_event(dev, INDEX_CMD_WRITE, pucData, sizeof(pucData), &i2c_signal);

        ad_i2c_device_release(dev);

        ad_i2c_close(dev);
}

void pollTouch() {

        //printf("pollTouch\r\n");

        OS_DELAY_MS(10);
        query_busy();
        uint8_t result = 0x00;
        start_query(&result);

        //if the 8th bit is set to 1 there is point/gesture/touch data available
        if (result & 0x80) {
                get_touch_sample();
        }
}
void start_query(uint8_t *result) {
        uint8_t res = 0x00;

        reset_event(&i2c_signal);

        dev = ad_i2c_open(IT7257); /* Open selected device */
        ad_i2c_device_acquire(dev);

        read_reg_event(dev, INDEX_QUERY_READ, &res ,sizeof(res), &i2c_signal);
       // printf("query %i\r\n",res);
        memcpy(result, &res, sizeof(res));

        ad_i2c_device_release(dev);
        ad_i2c_close(dev);
}

unsigned int hadTouch = 0;

void get_touch_sample() {
      // printf("get sample\r\n");

       uint8_t pointData[14] = {0x00};

       reset_event(&i2c_signal);

       dev = ad_i2c_open(IT7257); /* Open selected device */
       ad_i2c_device_acquire(dev);
       read_reg_event(dev, INDEX_POINT_READ, pointData, sizeof(pointData), &i2c_signal);
       ad_i2c_device_release(dev);
       ad_i2c_close(dev);

       //uint8_t x = pointData[2];
       //uint8_t y = pointData[4];

       //printf("Point data: %i %i %i %i %i %i % i% i\n\r", pointData[0] ,pointData[1],pointData[2],pointData[3],pointData[4],pointData[5],pointData[6], pointData[7]);
       if (pointData[2] != 0) { //touch down
               hadTouch = 1;
             //  printf("x: %i y: %i", pointData[2],pointData[4]);
               uint8_t points = {pointData[2],pointData[4]};
               setTouch(240-pointData[2], 240-pointData[4]);

       } else { //touch up?
               hadTouch = 0;
               OS_TASK_NOTIFY_FROM_ISR(getDisplayTaskHandle(), UPDATE_TOUCH_MASK, OS_NOTIFY_SET_BITS);
       }
       //return points;
}
