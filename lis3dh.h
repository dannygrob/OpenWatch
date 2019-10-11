/*
 * lis3dh.h
 *
 *  Created on: 18 Sep 2019
 *      Author: Nitwit
 */

#ifndef LIS3DH_H_
#define LIS3DH_H_

void i2c_init(void);
void reset();
void lis3dh_read(const uint8_t reg, uint8_t *val);
void lis3dh_read2(uint8_t reg, uint8_t *val, size_t len);
void lis3dh_write(const uint8_t reg, uint8_t *val, uint8_t len);
void lis3dh_write_reg_bit(const uint8_t reg, uint8_t bit, int state);
void write(uint8_t reg, uint8_t val);
void write_async(uint8_t reg, uint8_t val);

//static int read_reg_event(i2c_device dev, uint8_t reg, uint8_t *val, uint8_t len, i2c_event *event);
//static inline void read_reg_async(i2c_device dev, uint8_t reg, uint8_t *val, uint8_t len, ad_i2c_user_cb cb, void *user_data);
//static int write_reg_event(i2c_device dev, uint8_t reg, const uint8_t *val, uint8_t len, i2c_event *event);
//

void configureClickInterrupt(bool enable);
void configureInertialInterrupt(bool enable);
void setMode(int mode);
int setDataRate(int rate);
int getRange();
int setRange(int rangeA);
void enable(bool state);
void setTiltInterrupt();

#define LIS3DH_G_CHIP_ADDR  0x30 //(0x18 << 1)    // SA0(=SDO pin) = Ground
#define LIS3DH_V_CHIP_ADDR  0x19 //(0x19 << 1)    // SA0(=SDO pin) = Vdd

#define I_AM_LIS3DH 0x33// 0b00110011;

#endif /* LIS3DH_H_ */
