/**
 ****************************************************************************************
 *
 * @file demo_i2c.c
 *
 * @brief I2C demo (hw_i2c driver)
 *
 * Copyright (C) 2015-2016 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <osal.h>
#include <platform_devices.h>
#include "lis3dh.h"
#include "lis3dhRegisters.h"
#include <hw_i2c.h>
#include <resmgmt.h>
//#include "user_i2c.h"

int _range = 0;
int _mode = 0;
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
 * Signaling event used in operation started from temperature task.
 */
static i2c_event temp_signal;

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

static uint8_t read_multi_reg(uint8_t reg, int numBits) {
        // Read entire block with auto-increment
        uint8_t result;
        read_reg_event(dev, reg, &result, numBits, &i2c_signal);
        if (result == 0x00) {
           //read error
                return 0x00;
        }
        return result;
}

void lis3dh_write_reg_bit(const uint8_t reg, uint8_t bit, int state) {

        uint8_t val;

        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);
        //read the previous value
        read_reg_event(dev, reg, &val, sizeof(val), &i2c_signal);

        if (state == 0) {
            val = val & ~(0x01 << bit);
        } else {
            val = val | (0x01 << bit);
        }

        write_reg_event(dev, reg, &val, sizeof(val), &i2c_signal);

        ad_i2c_device_release(dev);
        ad_i2c_close(dev); /* Close selected device */
}

void i2c_init(void)
{
        uint8_t response = 0x00;

        init_event(&i2c_signal);

        dev = ad_i2c_open(LIS3DH); /* Open selected device */

        ad_i2c_device_acquire(dev);

        read_reg_event(dev, REG_WHO_AM_I, &response, sizeof(response), &i2c_signal);

        ad_i2c_device_release(dev);

        ad_i2c_close(dev);

        if (response == I_AM_LIS3DH) {
              //  enable(true);
                //testConfig();
             setTiltInterrupt();
             enable(true);
             setDataRate(10);
             //configureInertialInterrupt(true);
             //getAccel();
             //getInterruptTable();*/
        }
}

void setTiltInterrupt() {

        dev = ad_i2c_open(LIS3DH); /* Open selected device */

        reset_event(&i2c_signal);

        ad_i2c_device_acquire(dev);

        static const uint8_t val1[] = {0x3F};
        static const uint8_t val2[] = {0x09};
        static const uint8_t val3[] = {0x40};
        static const uint8_t val4[] = {0x00};
        static const uint8_t val5[] = {0x00}; //08 = latching - 00 = not latching
        //static const uint8_t val6[] = {0x10};
        static const uint8_t val6[] = {0x20};
        static const uint8_t val7[] = {0x01};
        //static const uint8_t val8[] = {0x2A};

        uint8_t hex = strtol("1100001", NULL, 2); //11010000 for 6D position on X low

        uint8_t val8[] = {0x00};
        val8[0] = hex;
        static const uint8_t cli_ths = {0x80};
        static const uint8_t cli_cfg = {0x15};


        write_reg_event(dev, REG_CTRL_REG1, val1, sizeof(val1), &i2c_signal);  // Write 0x5F - enable LP, x, y, z axis with ODR of 100 Hz
        write_reg_event(dev, REG_CTRL_REG2, val2, sizeof(val2), &i2c_signal);
        write_reg_event(dev, REG_CTRL_REG3, val3, sizeof(val3), &i2c_signal);
        write_reg_event(dev, REG_CTRL_REG4, val4, sizeof(val4), &i2c_signal);
        write_reg_event(dev, REG_CTRL_REG5, val5, sizeof(val5), &i2c_signal);

        write_reg_event(dev, REG_INT1_THS,      val6, sizeof(val6), &i2c_signal);
        write_reg_event(dev, REG_INT1_DURATION, val7, sizeof(val7), &i2c_signal);
        write_reg_event(dev, REG_INT1_CFG,      val8, sizeof(val8), &i2c_signal);

        ad_i2c_device_release(dev);
        ad_i2c_close(dev); /* Close selected device */


        /*
        AOI And/Or combination of Interrupt events. Default value: 0 Refer to Table 55: Interrupt mode
        6D 6 direction detection function enabled. Default value: 0 Refer to Table 55: Interrupt mode
        ZHIE Enable interrupt generation on Z high event or on Direction recognition. Default value: 0 (0: disable interrupt request;1: enable interrupt request)
        ZLIE Enable interrupt generation on Z low event or on Direction recognition. Default value: 0 (0: disable interrupt request;1: enable interrupt request)
        YHIE Enable interrupt generation on Y high event or on Direction recognition. Default value:  (0: disable interrupt request; 1: enable interrupt request.)
        YLIE Enable interrupt generation on Y low event or on Direction recognition. Default value: 0 (0: disable interrupt request; 1: enable interrupt request.)
        XHIE Enable interrupt generation on X high event or on Direction recognition. Default value: 0 (0: disable interrupt request; 1: enable interrupt request.)
        XLIE Enable interrupt generation on X low event or on Direction recognition. Default value: 0 (0: disable interrupt request; 1: enable interrupt request.)
         */

        /*
         * Write 3Fh into CTRL_REG1; // Set LIS3DH to low power mode with ODR = 25Hz.
        Write 40h into CTRL_REG3; // AOI1 interrupt generation is routed to INT1 pin.
        Write 80h into CTRL_REG4; // FS = ±2g low power mode with BDU bit enabled.
        Write 0Ch into CTRL_REG5; // Interrupt signal on INT1 pin is latched with D4D_INT1 bit enabled.
        If there is an interrupt from AOI1, INT1 pin will go high from low and stay high. Reading the  INT1_SRC(31h) register will clear the interrupt signal on INT1 pin.
        Write 20h into INT1_THS; // Threshold = 32LSBs * 15.625mg/LSB = 500mg. This corresponds to 30 degrees of tilt (=asin(0.5)) cone zone around the vertical gravity vector.
        Write 0Ah into INT1_DURATION; // Duration = 10LSBs * (1/25Hz) = 0.4s. 1LSB = 1/ODR = 40mS. If the X or Y axis enters the cone zone1 or cone zone2 for longer than 0.4s
                                         duration, then the interrupt will be generated. Duration = 0 means that the interrupt will be generated immediately.
        Write 4Fh into INT1_CFG; // 6D movement detection with Z axis disabled and YUPE, YDOWNE, XUPE and XDOWNE bits enabled.
         *
         */
}



void write(uint8_t reg, uint8_t val) {
        ad_i2c_write(dev, &reg, sizeof(reg));
        ad_i2c_write(dev, &val, sizeof(val));
}


void getAccel() {
        uint8_t result;

        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);
        // Read entire block with auto-increment
        uint8_t reading = read_multi_reg(REG_OUT_X_L_INCR, 6);
        // Read and sign extend
        /*result[0] <- ((reading[0] | (reading[1] << 8)) << 16) >> 16;
        result[1] <- ((reading[2] | (reading[3] << 8)) << 16) >> 16;
        result[2] <- ((reading[4] | (reading[5] << 8)) << 16) >> 16;

        // multiply by full-scale range to return in G
        result[0] = (result[0] / 32000.0) * _range;
        result[1] = (result[1] / 32000.0) * _range;
        result[2] = (result[2] / 32000.0) * _range;*/

        ad_i2c_device_release(dev);
        ad_i2c_close(dev); /* Close selected device */

        printf(result);
}


int getRange() {
        uint8_t range_bits;

        dev = ad_i2c_open(LIS3DH);
        ad_i2c_transact(dev, &REG_CTRL_REG4, sizeof(REG_CTRL_REG4),&range_bits, sizeof(range_bits));
        ad_i2c_close(dev);

        if (range_bits == 0x00) {
            return 2;
        } else if (range_bits == 0x01) {
            return 4;
        } else if (range_bits == 0x02) {
            return 8;
        } else {
            return 16;
        }
        return 0;
}

// set the full-scale range of the accelerometer (default +/- 2G)
 int setRange(int rangeA) {
        uint8_t val;

        dev = ad_i2c_open(LIS3DH);
        ad_i2c_transact(dev, &REG_CTRL_REG4, sizeof(REG_CTRL_REG4),&val, sizeof(val));
        ad_i2c_close(dev);

        val = val & 0xCF;
        uint8_t rangeBits = 0;
        int _range = 0;
        if (rangeA <= 2) {
            rangeBits = 0x00;
            _range = 2;
        } else if (rangeA <= 4) {
            rangeBits = 0x01;
            _range = 4;
        } else if (rangeA <= 8) {
            rangeBits = 0x02;
            _range = 8;
        } else {
            rangeBits = 0x03;
            _range = 16;
        }
        uint8_t toWrite = val | (rangeBits << 4);

        dev = ad_i2c_open(LIS3DH);
        write_async(REG_CTRL_REG4, toWrite);
      //  write(REG_CTRL_REG4, toWrite);
        ad_i2c_close(dev);

        uint8_t val2 = 0x01;
        OS_DELAY_MS(200);
        dev = ad_i2c_open(LIS3DH);
        ad_i2c_transact(dev, &REG_CTRL_REG4, sizeof(REG_CTRL_REG4),&val2, sizeof(val2));
        ad_i2c_close(dev);

        return _range;
}

// Set the mode of the accelerometer by passing a constant (LIS3DH_MODE_NORMAL,
// LIS3DH_MODE_LOW_POWER, LIS3DH_MODE_HIGH_RESOLUTION)
void setMode(int mode) {
        uint8_t moderes = 0x00;

        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        read_reg_event(dev, REG_CTRL_REG1, &moderes, sizeof(moderes), &i2c_signal);

        lis3dh_write_reg_bit(REG_CTRL_REG1, 3, moderes & 0x01);
        lis3dh_write_reg_bit(REG_CTRL_REG4, 3, moderes & 0x02);

        ad_i2c_close(dev);

        _mode = (int)mode;
}

// Set Accelerometer Data Rate in Hz
int setDataRate(int rate) {
        uint8_t val;

        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);
        read_reg_event(dev, REG_CTRL_REG1, &val, sizeof(val), &i2c_signal);
        val = val & 0x0F;
        int normalMode = (val < 8);
        if (rate == 0) {
           // 0b0000 -> power-down mode
           // we've already ANDed-out the top 4 bits; just write back
           rate = 0;
        } else if (rate <= 1) {
           val = val | 0x10;
           rate = 1;
        } else if (rate <= 10) {
           val = val | 0x20;
           rate = 10;
        } else if (rate <= 25) {
           val = val | 0x30;
           rate = 25;
        } else if (rate <= 50) {
           val = val | 0x40;
           rate = 50;
        } else if (rate <= 100) {
           val = val | 0x50;
           rate = 100;
        } else if (rate <= 200) {
           val = val | 0x60;
           rate = 200;
        } else if (rate <= 400) {
           val = val | 0x70;
           rate = 400;
        } else if (normalMode) {
           val = val | 0x90;
           rate = 1250;
        } else if (rate <= 1600) {
           val = val | 0x80;
           rate = 1600;
        } else {
           val = val | 0x90;
           rate = 5000;
        }
        write_reg_event(dev, REG_CTRL_REG1, &val, sizeof(val), &i2c_signal);

        ad_i2c_device_release(dev);
        ad_i2c_close(dev);

        return rate;
}

void configureHighPassFilter(uint8_t filters, uint8_t cutoff, uint8_t mode) {
        cutoff = LIS3DH_HPF_CUTOFF1;
        mode = LIS3DH_HPF_DEFAULT_MODE;
        // clear and set filters
        filters = LIS3DH_HPF_DISABLED | filters;

        // set register
        uint8_t toWrite = filters | cutoff | mode;
        write_reg_event(dev, REG_CTRL_REG2, &toWrite, sizeof(toWrite), &i2c_signal);
}

// Set the state of the accelerometer axes
void enable(bool state) {
        // LIS3DH_CTRL_REG1 enables/disables accelerometer axes
        // bit 0 = X axis
        // bit 1 = Y axis
        // bit 2 = Z axis
        uint8 val;
        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);

        read_reg_event(dev, REG_CTRL_REG1, &val, sizeof(val), &i2c_signal);

        if (state) { val = val | 0x07; }
        else { val = val & 0xF8; }
        dev = ad_i2c_open(LIS3DH);
        write_reg_event(dev, REG_CTRL_REG1, &val, sizeof(val), &i2c_signal);

        ad_i2c_device_release(dev);
        ad_i2c_close(dev);
}

// Enable/disable and configure inertial interrupts
void configureInertialInterrupt(bool enable) {//, threshold = 2.0, duration = 5, options = LIS3DH_X_HIGH | LIS3DH_Y_HIGH | LIS3DH_Z_HIGH) {
        int threshold = 2;
        uint8_t duration = 0x050;
        uint8_t options = LIS3DH_X_HIGH | LIS3DH_Y_HIGH | LIS3DH_Z_HIGH;

        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);

        // Set the enable flag
        lis3dh_write_reg_bit(REG_CTRL_REG3, 6, enable ? 1 : 0);

        // If we're disabling the interrupt, don't set anything else
        if (!enable) return;

        // Clamp the threshold
        if (threshold < 0) { threshold = threshold * -1.0; }    // Make sure we have a positive value
        if (threshold > _range) { threshold = _range; }          // Make sure it doesn't exceed the _range

        // Set the threshold
        threshold = (((threshold * 1.0) / (_range)) * 127);
        uint8_t thres = (threshold & 0x7f);
        write_reg_event(dev, REG_INT1_THS, &thres, sizeof(thres), &i2c_signal);

        // Set the duration
        uint8_t dur = duration & 0x7f;
        write_reg_event(dev, REG_INT1_DURATION, &dur, sizeof(dur), &i2c_signal);

        // Set the options flags
        write_reg_event(dev, REG_INT1_CFG, &options, sizeof(options), &i2c_signal);

        ad_i2c_device_release(dev);

        ad_i2c_close(dev);

}

// Enable/disable and configure click interrupts
void configureClickInterrupt(bool enable) {//, uint8_t clickType, float threshold, int timeLimit, int latency, int window) {
        /*clickType = LIS3DH_SINGLE_CLICK;
        threshold = 1.1;
        timeLimit = 5;
        latency = 10;
        window = 50;*/

        dev = ad_i2c_open(LIS3DH); /* Open selected device */

        ad_i2c_device_acquire(dev);

        uint8_t ctrl3;
        read_reg_event(dev, REG_CTRL_REG3, &ctrl3, sizeof(ctrl3), &i2c_signal);

        uint8_t newValue = ctrl3 | 0x80;

        lis3dh_write_reg_bit(REG_CTRL_REG3, newValue, sizeof(newValue));

        uint8_t click_cfg = 0x15;  //Turn on all axes & singletap.
        uint8_t threshold = 0x80 | threshold;

        uint8_t time_limit = 0x0a; //10
        uint8_t time_latency = 0x05;
        uint8_t time_window = 0xFF;

        write_reg_event(dev, REG_CLICK_CFG, &click_cfg, sizeof(click_cfg), &i2c_signal);
        write_reg_event(dev, REG_CLICK_THS, &threshold, sizeof(threshold), &i2c_signal);
        write_reg_event(dev, REG_TIME_LIMIT, &time_limit, sizeof(time_limit), &i2c_signal);
        write_reg_event(dev, REG_TIME_LATENCY, &time_latency, sizeof(time_latency), &i2c_signal);
        write_reg_event(dev, REG_TIME_WINDOW,  &time_window, sizeof(time_window), &i2c_signal);

        ad_i2c_device_release(dev);

        ad_i2c_close(dev);

}

   // Enable/Disable Data Ready Interrupt 1 on Interrupt Pin
void configureDataReadyInterrupt(bool enable) {
         lis3dh_write_reg_bit(REG_CTRL_REG3, 4, enable ? 1 : 0);
}

   // Enables/disables interrupt latching
void configureInterruptLatching(bool enable) {
        lis3dh_write_reg_bit(REG_CTRL_REG5, 3, enable ? 1 : 0);
        lis3dh_write_reg_bit(REG_CLICK_THS, 7, enable ? 1 : 0);
}

void getInterruptTable() {
        dev = ad_i2c_open(LIS3DH); /* Open selected device */
        ad_i2c_device_acquire(dev);

        uint8_t int1;
        read_reg_event(dev, REG_INT1_SRC, &int1, sizeof(int1), &i2c_signal);
        uint8_t click;
        read_reg_event(dev, REG_CLICK_SRC, &click, sizeof(click), &i2c_signal);


        bool inta =        (int1 & 0x40) != 0;
        bool xLow =         (int1 & 0x01) != 0;
        bool xHigh =        (int1 & 0x02) != 0;
        bool yLow =         (int1 & 0x04) != 0;
        bool yHigh =        (int1 & 0x08) != 0;
        bool zLow =         (int1 & 0x10) != 0;
        bool zHigh =        (int1 & 0x20) != 0;
        bool clicka =        (click & 0x40) != 0;
        bool singleClick =  (click & 0x10) != 0;
        bool doubleClick =  (click & 0x20) != 0;

        ad_i2c_device_release(dev);
        ad_i2c_close(dev);

    }

