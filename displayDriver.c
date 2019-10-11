/*
 * displayDriver.c
 *
 *  Created on: Jan 15, 2019
 *      Author: samsonm
 */

#include <stdint.h>
#include "math.h"
#include "displayDriver.h"
#include "platform_devices.h"
#include "ad_spi.h"
#include "ad_nvms.h"
#include "miniDB.h"
#include "hw_led.h"
#include "hw_timer2.h"
#include "sys_power_mgr.h"
#include <hw_breath.h>

//#define STORAGE_PART NVMS_GENERIC_PART
#define STORAGE_PART  NVMS_FLASH_STORAGE

PRIVILEGED_DATA static OS_TIMER buzz_timer;

int buzz_milliseconds = 0;

spi_device displaySpi;

uint8_t commonWriteBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
bool devIsOpen = false;

int absoluteValue(int NUMBER)
{
    if(NUMBER<0)
    {
        return -NUMBER;
    }
    else
    {
        return NUMBER;
    }
}

int deviceIsOpen() {
        return devIsOpen;
}
void openDevice() {
#ifdef CONFIG_SPI_DISPLAY
        if (devIsOpen == true) {
                return;
        }
        displaySpi = ad_spi_open(DISPLAY_SPI);
        devIsOpen=true;
#endif
}
void closeDevice() {
#ifdef CONFIG_SPI_DISPLAY
        if (devIsOpen == false) {
                return;
        }
        ad_spi_close(displaySpi);
        devIsOpen=false;
#endif
}

int bitIsSetTo = -1;

void displayWriteCommand(int COMMAND)
{
#ifdef CONFIG_SPI_DISPLAY
        if (devIsOpen == false) {
                displaySpi = ad_spi_open(DISPLAY_SPI);
        }

        hw_spi_set_9th_bit(ad_spi_get_hw_spi_id(displaySpi),0);

        ad_spi_write(displaySpi,(uint8_t *)&COMMAND,1);
        if (devIsOpen == false) {
                ad_spi_close(displaySpi);
        }
#endif
}
void displayWriteData(int DATA)
{
#ifdef CONFIG_SPI_DISPLAY

        if (devIsOpen == false) {
                displaySpi = ad_spi_open(DISPLAY_SPI);
        }
        hw_spi_set_9th_bit(ad_spi_get_hw_spi_id(displaySpi),1);

        ad_spi_write(displaySpi,(uint8_t *)&DATA,1);
        if (devIsOpen == false) {
                ad_spi_close(displaySpi);
        }
#endif
}

void displayWriteDataBuf(uint8_t DATA[], int DATA_SIZE)
{
#ifdef CONFIG_SPI_DISPLAY
        if (devIsOpen == false) {
                displaySpi = ad_spi_open(DISPLAY_SPI);
        }
        hw_spi_set_9th_bit(ad_spi_get_hw_spi_id(displaySpi),1);

        ad_spi_write(displaySpi,DATA,DATA_SIZE);
        if (devIsOpen == false) {
                ad_spi_close(displaySpi);
        }
#endif
}
void displayInit2(void) {
#ifdef CONFIG_SPI_DISPLAY
        //Initial_ST7789V
        //Software Reset
        hw_gpio_set_active(HW_GPIO_PORT_4,HW_GPIO_PIN_7);
        OS_DELAY_MS(120);
        hw_gpio_set_inactive(HW_GPIO_PORT_4, HW_GPIO_PIN_7);
        OS_DELAY_MS(120);
        hw_gpio_set_active(HW_GPIO_PORT_4,HW_GPIO_PIN_7);
        OS_DELAY_MS(120);

        displayWriteCommand(0x01);
        OS_DELAY_MS(10);
        displayWriteCommand(0x11);

        OS_DELAY_MS(10);

        displayWriteCommand(0x36);
        displayWriteData(0x00);

        displayWriteCommand(ST7789_COLMOD);
        displayWriteData(0x55);
        //displayWriteCommand(0x3A);
        //displayWriteData(0x06);

        displayWriteCommand(0xB2);
        displayWriteData(0x0C);
        displayWriteData(0x0C);
        displayWriteData(0x00);
        displayWriteData(0x33);
        displayWriteData(0x33);

        displayWriteCommand(0xB7);
        displayWriteData(0x75);

        displayWriteCommand(0xBB);
        displayWriteData(0x3D);   //Vcom=1.625V

        displayWriteCommand(0xC2);
        displayWriteData(0x01);

        displayWriteCommand(0xC3);
        displayWriteData(0x19);   //GVDD=4.8V

        displayWriteCommand( 0xC4);
        displayWriteData(0x20);   //VDV, 0x20:0v

        displayWriteCommand(0xC6);
        //displayWriteData(0x0F);   //0x0F:60Hz
        displayWriteData(0x01);   //0x01:111Hz

        displayWriteCommand(0xD0);
        displayWriteData(0xA4);
        displayWriteData(0xA1);

        displayWriteCommand(0xD6);
        displayWriteData(0xA1);//sleep ingate

        displayWriteCommand(0xE0);
        displayWriteData(0x70);
        displayWriteData(0x04);
        displayWriteData(0x08);
        displayWriteData(0x09);
        displayWriteData(0x09);
        displayWriteData(0x05);
        displayWriteData(0x2A);
        displayWriteData(0x33);
        displayWriteData(0x41);
        displayWriteData(0x07);
        displayWriteData(0x13);
        displayWriteData(0x13);
        displayWriteData(0x29);
        displayWriteData(0x2F);
        displayWriteCommand(0xE1);
        displayWriteData(0x70);
        displayWriteData(0x03);
        displayWriteData(0x09);
        displayWriteData(0x0A);
        displayWriteData(0x09);
        displayWriteData(0x06);
        displayWriteData(0x2B);
        displayWriteData(0x34);
        displayWriteData(0x41);
        displayWriteData(0x07);
        displayWriteData(0x12);
        displayWriteData(0x14);
        displayWriteData(0x28);
        displayWriteData(0x2E);

        displayWriteCommand(0x21);

        displayWriteCommand(0x29);



        displayWriteCommand(0x2A);     //Column Address Set
        displayWriteData(0x00);
        displayWriteData(0x00);   //0
        displayWriteData(0x00);
        displayWriteData(0xEF);   //239

        displayWriteCommand(0x2B);     //Row Address Set
        displayWriteData(0x00);
        displayWriteData(0x00);   //0
        displayWriteData(0x00);
        displayWriteData(0xEF);   //239

        displayWriteCommand(0x2C);
#endif
}
void displayInit(void)
{
#ifdef CONFIG_SPI_DISPLAY

        //Software Reset
        hw_gpio_set_active(HW_GPIO_PORT_4,HW_GPIO_PIN_7);
        OS_DELAY_MS(10);
        hw_gpio_set_inactive(HW_GPIO_PORT_4, HW_GPIO_PIN_7);
        OS_DELAY_MS(10);
        hw_gpio_set_active(HW_GPIO_PORT_4,HW_GPIO_PIN_7);
        OS_DELAY_MS(10);
        printf("init display_in\r\n");
        //Out of sleep mode
        displayWriteCommand(ST7789_SLPOUT);
        OS_DELAY_MS(120);
        //Set the color mode to 16-bit
        displayWriteCommand(ST7789_COLMOD);
        displayWriteData(0x55);
        OS_DELAY_MS(10);
        //Set the memory access control so that
        //display data comes as row address then
        //column address
        //Also sets refresh from bottom to top
        displayWriteCommand(ST7789_MADCTL);
        displayWriteData(0x00);
        //Column address start and end
        displaySetColumn(ST7789_XSTART,ST7789_WIDTH);
        //Row address start and end
        displaySetRow(ST7789_YSTART,ST7789_HEIGHT);
        //Enable extended command table
        displayWriteCommand(ST7789_CMD2EN);
        displayWriteData(0x5A);
        displayWriteData(0x69);
        displayWriteData(0x02);
        displayWriteData(0x01);
        //Set frame rate to 111 Hz (max)
        displayWriteCommand(ST7789_FRCTRL2);
        displayWriteData(0x0f);//01);
        //Set inversion on
        displayWriteCommand(ST7789_INVON);
        //Set normal display on
        displayWriteCommand(ST7789_NORON);
        OS_DELAY_MS(10);
        //Turn display on
        displayWriteCommand(ST7789_DISPON);
        OS_DELAY_MS(10);

        OS_DELAY_MS(1000); //TODO: CAN THIS BE REDUCED?

        printf("init display_inout\r\n");

        ad_nvms_init();
        printf("init display_invms\r\n");

#endif
}
void displaySetRotation(int ORIENTATION)
{
    displayWriteCommand(ST7789_MADCTL);
    switch(ORIENTATION)
    {
        case 0:
            displayWriteData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
            break;
        case 1:
            displayWriteData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
        case 2:
            displayWriteData(ST7789_MADCTL_RGB);
            break;
        case 3:
            displayWriteData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
            break;
        default:
            displayWriteData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
    }
}
void displaySetWindow(int XSTART, int XEND, int YSTART, int YEND)
{
    //Set column
    displaySetColumn(XSTART,XEND);
    //Set row
    displaySetRow(YSTART,YEND);
    //Begin writing frame to RAM
    displayWriteCommand(ST7789_RAMWR);
}
void displaySetWindow2(int XSTART, int XEND, int YSTART, int YEND)
{
    //Set column
    displaySetColumn(XSTART,XEND);
    //Set row
    displaySetRow(YSTART,YEND);
    //Begin writing frame to RAM
    displayWriteCommand(ST7789_RAMWR);
}
void displaySetColumn(int XSTART, int XEND)
{
    uint8_t xStartHigh = XSTART >> 8;
    uint8_t xStartLow = XSTART & 0xFF;
    uint8_t xEndHigh = (XEND) >> 8;
    uint8_t xEndLow = (XEND) & 0xFF;

    //Set window X dimensions
    displayWriteCommand(ST7789_CASET);
    displayWriteData(xStartHigh);
    displayWriteData(xStartLow);
    displayWriteData(xEndHigh);
    displayWriteData(xEndLow);
}
void displaySetRow(int YSTART, int YEND)
{
    //Add y-offset for display
    YSTART += ST7789_HEIGHT_OFFSET;
    YEND += ST7789_HEIGHT_OFFSET;

    uint8_t yStartHigh = (YSTART) >> 8;
    uint8_t yStartLow = (YSTART) & 0xFF;
    uint8_t yEndHigh = (YEND) >> 8;
    uint8_t yEndLow = (YEND) & 0xFF;

    //Set window Y dimensions
    displayWriteCommand(ST7789_RASET);
    displayWriteData(yStartHigh);
    displayWriteData(yStartLow);
    displayWriteData(yEndHigh);
    displayWriteData(yEndLow);
}
int display24to16Color(int COLOR)
{
    uint8_t redComponent = ((COLOR & 0x00FF0000)>>19);
    uint8_t greenComponent = ((COLOR & 0x0000FF00)>>10);
    uint8_t blueComponent = ((COLOR & 0x000000FF)>>3);
    uint16_t newColor = ((redComponent<<11)|(greenComponent<<5))|(blueComponent);
    return newColor;
}
void displayClear(void)
{
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2;i++)
    {
        displayWriteData(0x00);
    }
}
void displayClearBuf(void)
{
    uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(((ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2)/SPI_WRITE_BUFFER_SIZE);i++)
    {
            for(int j=0;j<SPI_WRITE_BUFFER_SIZE;j++)
            {
                    writeBuffer[j]=0x00;
            }
            displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
    }
}
void displayFillScreen(int COLOR)
{
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART);i++)
    {
        displayWriteData(colorHigh);
        displayWriteData(colorLow);
    }
}
void displayFillScreenBuf(int COLOR)
{
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(((ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2)/SPI_WRITE_BUFFER_SIZE);i++)
    {
            for(int j=0;j<SPI_WRITE_BUFFER_SIZE-1;j+=2)
            {
                    writeBuffer[j]=colorHigh;
                    writeBuffer[j+1]=colorLow;
            }
            displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
    }
}
void startCommonDraw() {
        //commonWriteBuffer = {0};
}
void displayCommonDraw() {
        int COLOR = display24to16Color(0xFF0000);
        uint8_t colorHigh = COLOR >> 8;
        uint8_t colorLow = COLOR & 0xFF;
        //commonWriteBuffer = {0};
        //openDevice();
        displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
        uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
        openDevice();


        for(int i = 0; i<(((ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2)/SPI_WRITE_BUFFER_SIZE);i++)
        {
                 for(int j=0;j<SPI_WRITE_BUFFER_SIZE-1;j+=2)
                {

                        memcpy(writeBuffer, commonWriteBuffer, ST7789_WIDTH*BYTES_PER_PIXEL);

                        /*if (commonWriteBuffer[j] == 0x00) {
                                writeBuffer[j]=colorHigh;
                                writeBuffer[j+1]=colorLow;
                        }*/

                }
                displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
        }
        closeDevice();
}
void displayDrawPixel(int X_LOCATION, int Y_LOCATION, int COLOR)
{
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    displaySetWindow(X_LOCATION,ST7789_WIDTH-1,Y_LOCATION,ST7789_HEIGHT-1);
    displayWriteData(colorHigh);
    displayWriteData(colorLow);
}
void displayDrawPixels(uint8_t DATA[], int COLOR) {
        displayWriteDataBuf(DATA, sizeof(DATA));
}
void displayDrawPixelThickness(int X_LOCATION, int Y_LOCATION, int COLOR, int THICKNESS)
{
    int pixelXStart = X_LOCATION - THICKNESS/2;
    int pixelXEnd = X_LOCATION + THICKNESS/2;
    int pixelYStart = Y_LOCATION - THICKNESS/2;
    int pixelYEnd = Y_LOCATION + THICKNESS/2;
    displayDrawRectangle(pixelXStart, pixelXEnd, pixelYStart, pixelYEnd, COLOR);
}
void displayDrawLine(int START_X, int END_X, int START_Y, int END_Y, int COLOR)
{
    int deltaX = absoluteValue(END_X-START_X);
    int xIncrement = START_X<END_X ? 1 : -1;
    int deltaY = absoluteValue(END_Y-START_Y);
    int yIncrement = START_Y<END_Y ? 1 : -1;
    int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
    int oldLineError;

        for(;;)
        {
            displayDrawPixel(START_X,START_Y,COLOR);
            if (START_X==END_X && START_Y==END_Y) break;
            oldLineError = lineError;
            if (oldLineError >-deltaX)
            {
                lineError -= deltaY;
                START_X += xIncrement;
            }
            if (oldLineError < deltaY)
            {
                lineError += deltaX;
                START_Y += yIncrement;
            }
        }
}
void displayDrawLineInBuf(uint8_t *buf, int START_X, int END_X, int START_Y, int END_Y, int COLOR)
{
    int deltaX = absoluteValue(END_X-START_X);
    int xIncrement = START_X<END_X ? 1 : -1;
    int deltaY = absoluteValue(END_Y-START_Y);
    int yIncrement = START_Y<END_Y ? 1 : -1;
    int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
    int oldLineError;

        int counter = 0;
        for(;;)
        {
            buf[counter] = COLOR;
            //displayDrawPixel(START_X,START_Y,COLOR);
            if (START_X==END_X && START_Y==END_Y) break;
            oldLineError = lineError;
            if (oldLineError >-deltaX)
            {
                lineError -= deltaY;
                START_X += xIncrement;
            }
            if (oldLineError < deltaY)
            {
                lineError += deltaX;
                START_Y += yIncrement;
            }
            counter++;
        }
}
void displayDrawLinePolarShort(int START_X, int START_Y, int RADIUS, int ANGLE, int length, int COLOR)
{
    float newAngle = 3.1416*ANGLE/180;

    int startX = START_X+(RADIUS - length)*cos(newAngle);
    int startY = START_Y+(RADIUS - length)*sin(newAngle);

    int endX = START_X+RADIUS*cos(newAngle);
    int endY = START_Y+RADIUS*sin(newAngle);
    displayDrawLine(startX,endX,startY,endY,COLOR);
}
void displayDrawLinePolar(int START_X, int START_Y, int RADIUS, int ANGLE, int COLOR)
{
    float newAngle = 3.1416*ANGLE/180;
    int endX = START_X+RADIUS*cos(newAngle);
    int endY = START_Y+RADIUS*sin(newAngle);
    displayDrawLine(START_X,endX,START_Y,endY,COLOR);
}
void drawLine565(int x1, int y1, int x2, int y2,
                 uint32_t color,
                 char *fbuffer,
                 const int bytesperline)
{
    int XMOVEPOW = 1;
    int xmove, ymove;
    int dx, dy;
    int accum;
    int cnt;
    uint8_t * addr = fbuffer;

    /* (x1, y1) must be the topmost point */
    if ((dy = y2 - y1) < 0) {
        dy = -dy;
        y1 = y2;
        dx = x2; x2 = x1; x1 = dx;
    }
    addr += y1*bytesperline + (x1 << XMOVEPOW);
    if ((dx = x2 - x1) < 0) {
        dx = -dx;
        xmove = -(1 << XMOVEPOW);
    } else {
        xmove = 1 << XMOVEPOW;
    }
    /* here: dx >= 0, dy >= 0, addr, xmove */
    while (1) {
        if (dy == 0) {
            /* horizontal lines */
            break;
        }
        if (dx == 0) {
            /* vertical lines */
            xmove = bytesperline;
            dx = dy;
            break;
        }
        if (dx == dy) {
            /* 45 deg lines */
            xmove += bytesperline;
            break;
        }
        /* the rest...*/
        ymove = bytesperline;
        dx <<= 1;
        dy <<= 1;
        if (dx > dy) {
            /* left-left-down and right-right-down */
            cnt = xmove; xmove = ymove; ymove = cnt;
            cnt = dx; dx = dy; dy = cnt;
        }
        /* left-down-down and right-rown-down */
        accum = dy >> 1;
        cnt = dy >> 5;
        switch (accum & 0x0F) {
            do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += ymove; \
                      accum -= dx; \
                      if (accum < 0) { \
                          accum += dy; \
                          addr += xmove; \
                      }
                PUTPIX(15)
                PUTPIX(14)
                PUTPIX(13)
                PUTPIX(12)
                PUTPIX(11)
                PUTPIX(10)
                PUTPIX(9)
                PUTPIX(8)
                PUTPIX(7)
                PUTPIX(6)
                PUTPIX(5)
                PUTPIX(4)
                PUTPIX(3)
                PUTPIX(2)
                PUTPIX(1)
                PUTPIX(0)
                if (--cnt < 0) return;
            } while (1);
        }
    }
    /* special care for horizontal/vertical/45 deg lines
       as they are statistically significant objects */
    cnt = dx >> 4;
    switch (dx & 0x0F) {
        do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += xmove;
            PUTPIX(15)
            PUTPIX(14)
            PUTPIX(13)
            PUTPIX(12)
            PUTPIX(11)
            PUTPIX(10)
            PUTPIX(9)
            PUTPIX(8)
            PUTPIX(7)
            PUTPIX(6)
            PUTPIX(5)
            PUTPIX(4)
            PUTPIX(3)
            PUTPIX(2)
            PUTPIX(1)
            PUTPIX(0)
            if (--cnt < 0) return;
        } while (1);
        return;
    }
    return;
}


void displayDrawLineThickness(int START_X, int END_X, int START_Y, int END_Y, int COLOR, int THICKNESS)
{
        int width = END_X - START_X;
        int height = THICKNESS;//END_Y - START_Y;

        int widthOfImage = width;
        if(widthOfImage%2)
        {
                widthOfImage++;
        }
      //  int partialImageAdressDataOffset = (widthOfImage*START_Y*BYTES_PER_PIXEL)+(START_X*BYTES_PER_PIXEL);
       // int bufferCounter = 0;
        //int memoryReadSpot = 0;
       // uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
        uint8_t partialImageWidthBuffer[(ST7789_WIDTH*BYTES_PER_PIXEL)] = {0};
        displaySetWindow(START_X,(START_X+width-1),START_Y,(START_Y+height));

//        int counter = 0;
//        for (int x = START_X - (THICKNESS / 2); x< END_X - (THICKNESS / 2); x++) {
//                for (int y = START_Y - (THICKNESS / 2); y< END_Y + THICKNESS / 2; y++) {
//                        partialImageWidthBuffer[counter] = COLOR;
//                        counter ++;
//                }
//        }

        int counter = 0;
        for(int j = THICKNESS/2; j>=0; j--)
            {
                for(int i = THICKNESS/2; i>0; i--)
                {
                        int sx = START_X-i;
                        int ex = END_X-i;
                        int sy = START_Y-j;
                        int ey = END_Y-j;
                        int deltaX = absoluteValue(ex-sx);
                        int xIncrement = sx<ex ? 1 : -1;
                        int deltaY = absoluteValue(ey-sy);
                        int yIncrement = sy<ey ? 1 : -1;
                        int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
                        int oldLineError;

                            //int counter = 0;
                            for(;;)
                            {
                                partialImageWidthBuffer[counter] = COLOR;
                                //displayDrawPixel(START_X,START_Y,COLOR);
                                if (sx==END_X && sy==ey) break;
                                oldLineError = lineError;
                                if (oldLineError >-deltaX)
                                {
                                    lineError -= deltaY;
                                    sx += xIncrement;
                                }
                                if (oldLineError < deltaY)
                                {
                                    lineError += deltaX;
                                    sy += yIncrement;
                                }
                                counter++;
                            }
                }
                for(int i = 0; i<THICKNESS/2; i++)
                {
                        int sx = START_X+i;
                        int ex = END_X+i;
                        int sy = START_Y-j;
                        int ey = END_Y-j;
                        int deltaX = absoluteValue(ex-sx);
                        int xIncrement = sx<ex ? 1 : -1;
                        int deltaY = absoluteValue(ey-sy);
                        int yIncrement = sy<ey ? 1 : -1;
                        int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
                        int oldLineError;

                            //int counter = 0;
                            for(;;)
                            {
                                partialImageWidthBuffer[counter] = COLOR;
                                //displayDrawPixel(START_X,START_Y,COLOR);
                                if (sx==END_X && sy==ey) break;
                                oldLineError = lineError;
                                if (oldLineError >-deltaX)
                                {
                                    lineError -= deltaY;
                                    sx += xIncrement;
                                }
                                if (oldLineError < deltaY)
                                {
                                    lineError += deltaX;
                                    sy += yIncrement;
                                }
                                counter++;
                            }
                }
            }
            for(int j = 0; j<THICKNESS/2; j++)
            {
                for(int i = THICKNESS/2; i>0; i--)
                {

                   int sx = START_X-i;
                   int ex = END_X-i;
                   int sy = START_Y+j;
                   int ey = END_Y+j;
                   int deltaX = absoluteValue(ex-sx);
                   int xIncrement = sx<ex ? 1 : -1;
                   int deltaY = absoluteValue(ey-sy);
                   int yIncrement = sy<ey ? 1 : -1;
                   int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
                   int oldLineError;

                       //int counter = 0;
                       for(;;)
                       {
                           partialImageWidthBuffer[counter] = COLOR;
                           //displayDrawPixel(START_X,START_Y,COLOR);
                           if (sx==END_X && sy==ey) break;
                           oldLineError = lineError;
                           if (oldLineError >-deltaX)
                           {
                               lineError -= deltaY;
                               sx += xIncrement;
                           }
                           if (oldLineError < deltaY)
                           {
                               lineError += deltaX;
                               sy += yIncrement;
                           }
                           counter++;
                       }
                }
                for(int i = 0; i<THICKNESS/2; i++)
                {
                        int sx = START_X+i;
                        int ex = END_X+i;
                        int sy = START_Y+j;
                        int ey = END_Y+j;
                        int deltaX = absoluteValue(ex-sx);
                        int xIncrement = sx<ex ? 1 : -1;
                        int deltaY = absoluteValue(ey-sy);
                        int yIncrement = sy<ey ? 1 : -1;
                        int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
                        int oldLineError;

                          // int counter = 0;
                           for(;;)
                           {
                               partialImageWidthBuffer[counter] = COLOR;
                               //displayDrawPixel(START_X,START_Y,COLOR);
                               if (sx==END_X && sy==ey) break;
                               oldLineError = lineError;
                               if (oldLineError >-deltaX)
                               {
                                   lineError -= deltaY;
                                   sx += xIncrement;
                               }
                               if (oldLineError < deltaY)
                               {
                                   lineError += deltaX;
                                   sy += yIncrement;
                               }
                               counter++;
                           }
                }
            }



        displayWriteDataBuf(partialImageWidthBuffer, counter);
/*
    for(int j = THICKNESS/2; j>=0; j--)
    {
        for(int i = THICKNESS/2; i>0; i--)
        {
            displayDrawLine(START_X-i, END_X-i, START_Y-j, END_Y-j, COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLine(START_X+i, END_X+i, START_Y-j, END_Y-j, COLOR);
        }
    }
    for(int j = 0; j<THICKNESS/2; j++)
    {
        for(int i = THICKNESS/2; i>0; i--)
        {
            displayDrawLine(START_X-i, END_X-i, START_Y+j, END_Y+j, COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLine(START_X+i, END_X+i, START_Y+j, END_Y+j, COLOR);
        }
    }*/
}

void displayDrawLineThickness2(int START_X, int END_X, int START_Y, int END_Y, int COLOR, int THICKNESS)
{
    float yLength = END_Y - START_Y;
    float currentYPosition = START_Y;
    if(END_X>START_X)
    {
        float xLength = END_X - START_X;
        float lineSlope = yLength/xLength;
        for(int i = START_X; i<=END_X; i+=THICKNESS)
        {
            displayDrawPixelThickness(i,currentYPosition,COLOR,THICKNESS);
            currentYPosition += lineSlope;
        }
    }
    else
    {
        float xLength = START_X - END_X;
        float lineSlope = yLength/xLength;
        for(int i = START_X; i>=END_X; i-=THICKNESS)
        {
            displayDrawPixelThickness(i,currentYPosition,COLOR,THICKNESS);
            currentYPosition += lineSlope;
        }
    }
}
void displayDrawLinePolarThickness(int START_X, int START_Y, int RADIUS, int ANGLE, int COLOR, int THICKNESS)
{
    for(int j = THICKNESS/2; j>=0; j--)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolar(START_X-i,START_Y-j,RADIUS,ANGLE,COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolar(START_X+i,START_Y-j,RADIUS,ANGLE,COLOR);
        }
    }
    for(int j = 0; j<THICKNESS/2; j++)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolar(START_X-i,START_Y+j,RADIUS,ANGLE,COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolar(START_X+i,START_Y+j,RADIUS,ANGLE,COLOR);
        }
    }
}
void displayDrawLinePolarThicknessShort(int START_X, int START_Y, int RADIUS, int ANGLE, int length, int COLOR, int THICKNESS)
{
    for(int j = THICKNESS/2; j>=0; j--)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolarShort(START_X-i,START_Y-j,RADIUS,ANGLE, length, COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolarShort(START_X+i,START_Y-j,RADIUS,ANGLE, length, COLOR);
        }
    }
    for(int j = 0; j<THICKNESS/2; j++)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolarShort(START_X-i,START_Y+j,RADIUS,ANGLE, length, COLOR);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolarShort(START_X+i,START_Y+j,RADIUS,ANGLE, length, COLOR);
        }
    }
}
void displayDrawRectangle(int XSTART, int XEND, int YSTART, int YEND, int COLOR)
{
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    displaySetWindow(XSTART,XEND,YSTART,YEND);
    for(int i = 0; i<YEND*XEND*2;i++)
    {
        displayWriteData(colorHigh);
        displayWriteData(colorLow);
    }
}
void displayDrawRectangleBuff(int XSTART, int XEND, int YSTART, int YEND, int COLOR)
{
        openDevice();
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    uint8_t rectangleWidth = (XEND>XSTART?(XEND-XSTART):(XSTART-XEND));
    uint8_t rectangleHeight = (YEND>YSTART?(YEND-YSTART):(YSTART-YEND));
    //uint16_t leftOverData = (rectangleWidth*rectangleHeight*2)%SPI_WRITE_BUFFER_SIZE;
    uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};

    displaySetWindow(XSTART,XEND,YSTART,YEND);

    int rectSize = rectangleWidth*rectangleHeight*2;
    int currentPointer = 0;
    int bufferPointer = 0;
    //for(int i = 0; i<rectSize;i++)
   // {
    while (currentPointer < rectSize)
    {
            writeBuffer[bufferPointer]=colorHigh;
            writeBuffer[bufferPointer+1]=colorLow;

            if ((bufferPointer + 2 >= SPI_WRITE_BUFFER_SIZE) || (currentPointer + 2 == rectSize)) {
                    displayWriteDataBuf(writeBuffer,bufferPointer);
                    bufferPointer = 0;
            }

            currentPointer += 2;
            bufferPointer += 2;
    }
    closeDevice();

   // }
   // if(leftOverData>0)
   // {
    //        for(int k=0;k<leftOverData-1;k+=2)
   //         {
     //               writeBuffer[k]=colorHigh;
    //                writeBuffer[k+1]=colorLow;
     //       }
    //        displayWriteDataBuf(writeBuffer,leftOverData);
   // }
}
void displayDrawRectangleBuf(int XSTART, int XEND, int YSTART, int YEND, int COLOR)
{
    uint8_t colorHigh = COLOR >> 8;
    uint8_t colorLow = COLOR & 0xFF;
    uint8_t rectangleWidth = (XEND>XSTART?(XEND-XSTART):(XSTART-XEND));
    uint8_t rectangleHeight = (YEND>YSTART?(YEND-YSTART):(YSTART-YEND));
    uint16_t leftOverData = (rectangleWidth*rectangleHeight*2)%SPI_WRITE_BUFFER_SIZE;
    uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};

    displaySetWindow(XSTART,XEND,YSTART,YEND);
    for(int i = 0; i<((rectangleWidth*rectangleHeight*2)/SPI_WRITE_BUFFER_SIZE);i++)
    {
            for(int j=0;j<SPI_WRITE_BUFFER_SIZE-1;j+=2)
            {
                    writeBuffer[j]=colorHigh;
                    writeBuffer[j+1]=colorLow;
            }
            displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
    }
    if(leftOverData>0)
    {
            for(int k=0;k<leftOverData - 1;k+=2)
            {
                    writeBuffer[k]=colorHigh;
                    writeBuffer[k+1]=colorLow;
            }
            displayWriteDataBuf(writeBuffer,leftOverData + 6);
    }
}
void displayArrayBuf(int XSTART, int WIDTH, int YSTART, int HEIGHT, int (*ARRAY)[], int SIZE_OF_ARRAY)
{
//    uint16_t sizeOfArray = sizeof((*ARRAY))/sizeof((*ARRAY)[0]);
    uint16_t sizeOfArray = SIZE_OF_ARRAY;
    uint16_t leftOverData = (WIDTH*HEIGHT)%SPI_WRITE_BUFFER_SIZE;
    uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
    displaySetWindow(XSTART,(XSTART+WIDTH),YSTART,(YSTART+HEIGHT));

    for(int i = 0; i<((WIDTH*HEIGHT*2)/SPI_WRITE_BUFFER_SIZE);i++)
    {
            for(int j=0;j<SPI_WRITE_BUFFER_SIZE-1;j++)
            {
                    writeBuffer[(2*j)]=(*ARRAY)[((i*SPI_WRITE_BUFFER_SIZE)+j)] >> 8;
                    writeBuffer[(2*j)+1]=(*ARRAY)[((i*SPI_WRITE_BUFFER_SIZE)+j)] & 0xFF;
            }
            displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
    }
    if(leftOverData>0)
    {
            for(int k=0;k<leftOverData-1;k++)
            {
                    writeBuffer[(2*k)]=(*ARRAY)[((sizeOfArray-leftOverData)+k)] >> 8;
                    writeBuffer[(2*k)+1]=(*ARRAY)[((sizeOfArray-leftOverData)+k)] & 0xFF;
            }
            displayWriteDataBuf(writeBuffer,(2*leftOverData));
    }
}
void displayDrawCircle(int CENTER_X, int CENTER_Y, int RADIUS, int COLOR)
{
    int x = RADIUS-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx-(RADIUS<<1);

    while(x>=y)
    {
        displayDrawPixel(CENTER_X-y, CENTER_Y+x, COLOR);
        displayDrawPixel(CENTER_X+y, CENTER_Y+x, COLOR);
        displayDrawPixel(CENTER_X-x, CENTER_Y+y, COLOR);
        displayDrawPixel(CENTER_X+x, CENTER_Y+y, COLOR);
        displayDrawPixel(CENTER_X-x, CENTER_Y-y, COLOR);
        displayDrawPixel(CENTER_X+x, CENTER_Y-y, COLOR);
        displayDrawPixel(CENTER_X-y, CENTER_Y-x, COLOR);
        displayDrawPixel(CENTER_X+y, CENTER_Y-x, COLOR);

        if(err <=0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        else
        {
            x--;
            dx += 2;
            err += dx -(RADIUS<<1);
        }
    }
}
void displayTestPattern(void)
{
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2;i++)
    {
        uint8_t colorHigh = i;
        uint8_t colorLow = i;
        displayWriteData(colorHigh);
        displayWriteData(colorLow);
    }
}
void displayTestPattern2(void)
{
    displaySetWindow(ST7789_XSTART,ST7789_WIDTH,ST7789_YSTART,ST7789_HEIGHT);
    for(int i = 0; i<(ST7789_WIDTH-ST7789_XSTART)*(ST7789_HEIGHT-ST7789_YSTART)*2;i++)
    {
        if(i%2)
        {
            displayWriteData(0xF);
            displayWriteData(0xF);
        }
        else
        {
            displayWriteData(0x0);
            displayWriteData(0x0);
        }
    }
}

void displayImageFromMemory(int XSTART, int YSTART, int ADDRESS_IN_MEMORY)
{
        OS_TICK_TIME xNextWakeTime;
        xNextWakeTime = OS_GET_TICK_COUNT();
        uint8_t sizeOfImageBuffer[2]={0};
        nvms_t flashMemory = ad_nvms_open(STORAGE_PART);
        ad_nvms_read(flashMemory, ADDRESS_IN_MEMORY, (uint8 *) sizeOfImageBuffer, sizeof(sizeOfImageBuffer));
        int imageAdressDataOffset = ADDRESS_IN_MEMORY+2;
        int widthOfImage = sizeOfImageBuffer[0];
        int heightOfImage = sizeOfImageBuffer[1];
        int sizeOfImageInBytes = widthOfImage*heightOfImage*2;
        int leftOverData = sizeOfImageInBytes%SPI_WRITE_BUFFER_SIZE;
        int currentPositionInData = 0;

        uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
        displaySetWindow(XSTART,(XSTART+widthOfImage),YSTART,YSTART+heightOfImage);
        for(int i = 0; i<(sizeOfImageInBytes/SPI_WRITE_BUFFER_SIZE);i++)
        {
                ad_nvms_read(flashMemory, ((i*SPI_WRITE_BUFFER_SIZE)+imageAdressDataOffset), (uint8 *) writeBuffer, sizeof(writeBuffer));
                displayWriteDataBuf(writeBuffer,SPI_WRITE_BUFFER_SIZE);
                currentPositionInData = i*SPI_WRITE_BUFFER_SIZE;
        }
        if(leftOverData>0)
        {
                ad_nvms_read(flashMemory, (currentPositionInData+SPI_WRITE_BUFFER_SIZE+imageAdressDataOffset), (uint8 *) writeBuffer, sizeof(writeBuffer));
                displayWriteDataBuf(writeBuffer,leftOverData);
        }
}

void displayPartialImageFromMemory(int SCREEN_XSTART, int SCREEN_YSTART, int IMAGE_XSTART, int IMAGE_YSTART, int IMAGE_PARTIAL_WIDTH, int IMAGE_PARTIAL_HEIGHT, int ADDRESS_IN_MEMORY)
{
        OS_TICK_TIME xNextWakeTime;
        xNextWakeTime = OS_GET_TICK_COUNT();
        nvms_t flashMemory = ad_nvms_open(STORAGE_PART);
        uint8_t sizeOfImageBuffer[2]={0};
        ad_nvms_read(flashMemory, ADDRESS_IN_MEMORY, (uint8 *) sizeOfImageBuffer, sizeof(sizeOfImageBuffer));
        int widthOfImage = sizeOfImageBuffer[0];
        if(widthOfImage%2)
        {
                widthOfImage++;
        }
        int partialImageAdressDataOffset = (widthOfImage*IMAGE_YSTART*BYTES_PER_PIXEL)+(IMAGE_XSTART*BYTES_PER_PIXEL)+2+ADDRESS_IN_MEMORY;
        int bufferCounter = 0;
        int memoryReadSpot = 0;
        uint8_t writeBuffer[SPI_WRITE_BUFFER_SIZE] = {0};
        uint8_t partialImageWidthBuffer[(ST7789_WIDTH*BYTES_PER_PIXEL)] = {0};
        displaySetWindow(SCREEN_XSTART,(SCREEN_XSTART+IMAGE_PARTIAL_WIDTH-1),SCREEN_YSTART,(SCREEN_YSTART+IMAGE_PARTIAL_HEIGHT));
        int currentPositionInData = 0;
        for(int currentRow = 0;currentRow<(IMAGE_PARTIAL_HEIGHT);currentRow++)
        {
                memoryReadSpot = (currentRow*(widthOfImage)*BYTES_PER_PIXEL)+partialImageAdressDataOffset;
                ad_nvms_read(flashMemory,memoryReadSpot, (uint8 *)partialImageWidthBuffer, sizeof(partialImageWidthBuffer));




               // if((bufferCounter*(IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL))>SPI_WRITE_BUFFER_SIZE)
                //{
                        memcpy(writeBuffer, partialImageWidthBuffer, IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL);
                        displayWriteDataBuf(writeBuffer,(IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL));
                        currentPositionInData += (IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL);
                        //currentPositionInData = bufferCounter*SPI_WRITE_BUFFER_SIZE;
                        bufferCounter = 0;

                //}
                //else
               // {
               //         memcpy(&writeBuffer[currentPositionInData], partialImageWidthBuffer, IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL);
               //         currentPositionInData += (IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL);
               //         bufferCounter++;
               // }
        }
        if(bufferCounter>0)
        {
               // displayWriteDataBuf(writeBuffer,(bufferCounter*(IMAGE_PARTIAL_WIDTH*BYTES_PER_PIXEL)));
        }
}
void displayPower(int ON_OFF) {
        displayPowerBrightness(ON_OFF, 127);
}
void displayPowerBrightness(int ON_OFF, int brightness)
{
        if(ON_OFF)
        {
                if (pm_get_sleep_mode() != pm_mode_active) {
                       pm_set_sleep_mode(pm_mode_active);
                }
                //Out of sleep mode
                displayWriteCommand(ST7789_SLPOUT);
                OS_DELAY_MS(120);
                displayWriteCommand(ST7789_DISPON);

                setup_breath(239, 240, 1, 255);

                /*static timer2_config cfg = {
                        .frequency = 0,
                        .pwm2_end = 0,
                        .pwm2_start = 0,
                        .pwm3_end = 0,
                        .pwm3_start = 0,
                        .pwm4_end = 0,
                        .pwm4_start = 0,
                };
                hw_timer2_init(&cfg);
                hw_timer2_set_division_factor(HW_TIMER2_DIV_4);
                hw_timer2_set_frequency(100);
                hw_timer2_set_pwm_duty_cycle(HW_TIMER2_PWM_3, brightness);
                hw_timer2_enable();
                hw_led_set_led2_src(HW_LED_SRC2_PWM3);
                hw_led_enable_led2(true);*/



        }
        else
        {
                setup_breath(0, 1, 255, 255);
                OS_DELAY_MS(2000);
                hw_breath_disable();
                //Into sleep mode
                displayWriteCommand(ST7789_SLPIN);
                displayWriteCommand(ST7789_DISPOFF);
                if (pm_get_sleep_mode() != pm_mode_extended_sleep) {
                        pm_set_sleep_mode(pm_mode_extended_sleep);
                }
        }
}

void setup_breath(uint8_t dc_min, uint8_t dc_max, uint8_t dc_step, uint8_t freq_div)
{
        breath_config config = {
                .dc_min = dc_min,
                .dc_max = dc_max,
                .dc_step = dc_step,
                .freq_div = freq_div,
                .polarity = HW_BREATH_PWM_POL_POS
        };

        /*
         * Setup breath timer configuration, so hardware can drive LED automatically.
         */
        hw_breath_init(&config);

        /*
         * Setup LED1 output to be driven by timer.
         */
        hw_led_set_led2_src(HW_LED_SRC2_BREATH);
        hw_led_enable_led2(true);

        /*
         * Then start breath timer.
         */
        hw_breath_enable();
}

void buzz(int duration) {
        buzz_milliseconds = duration;
        startBuzzTimeout();
}

static void buzz_timer_cb(OS_TIMER timer)
{
        buzz_milliseconds -=100;

       if (buzz_milliseconds <= 0) {

               stopBuzzTimeout();
       }
}

void startBuzzTimeout() {

        if (OS_TIMER_IS_ACTIVE(buzz_timer)) {
                return;
        }

        hw_gpio_set_active(HW_GPIO_PORT_1,HW_GPIO_PIN_0);

        OS_TIMER wake = OS_TIMER_GET_TIMER_ID(buzz_timer);

        if (wake) {
              OS_TIMER_START(buzz_timer, OS_TIMER_FOREVER);
        } else {
              buzz_timer = OS_TIMER_CREATE("buzz", portCONVERT_MS_2_TICKS(100), OS_TIMER_SUCCESS,
                                                              (void *) getDisplayTaskHandle(), buzz_timer_cb);
              OS_ASSERT(buzz_timer);
              OS_TIMER_START(buzz_timer, OS_TIMER_FOREVER);
        }

}

void stopBuzzTimeout() {
       hw_gpio_set_inactive(HW_GPIO_PORT_1,HW_GPIO_PIN_0);
       OS_TIMER_STOP(buzz_timer,0);
}


/*int getSizeOfImage(char *FILENAME, int NAME_SIZE)
{
    int sizeOfFile = 0;
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    filePointer = sdCard_fopen(fileToOpen, "r");
    sdCard_fseek(filePointer, BITMAP_SIZE_OFFSET);
    for(int byteNumberToRead = 0; byteNumberToRead<4;byteNumberToRead++)
    {
        sizeOfFile |= sdCard_fgetc(filePointer)<<(8*byteNumberToRead);
    }
    sdCard_fclose(filePointer);
    return sizeOfFile;
}
int getDataOffsetBMP(char *FILENAME, int NAME_SIZE)
{
    int dataOffsetLocation = 0;
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    filePointer = sdCard_fopen(fileToOpen, "r");
    sdCard_fseek(filePointer, BITMAP_DATA_OFFSET);
    for(int byteNumberToRead = 0; byteNumberToRead<4;byteNumberToRead++)
    {
        dataOffsetLocation |= sdCard_fgetc(filePointer)<<(8*byteNumberToRead);
    }
    sdCard_fclose(filePointer);
    return dataOffsetLocation;
}
int getHeightOfBMP(char *FILENAME, int NAME_SIZE)
{
    int heightOfImage = 0;
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    filePointer = sdCard_fopen(fileToOpen, "r");
    sdCard_fseek(filePointer, BITMAP_HEIGHT_OFFSET);
    for(int byteNumberToRead = 0; byteNumberToRead<4;byteNumberToRead++)
    {
        heightOfImage |= sdCard_fgetc(filePointer)<<(8*byteNumberToRead);
    }
    sdCard_fclose(filePointer);
    return heightOfImage;
}
int getWidthOfBMP(char *FILENAME, int NAME_SIZE)
{
    int widthOfImage = 0;
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    filePointer = sdCard_fopen(fileToOpen, "r");
    sdCard_fseek(filePointer, BITMAP_WIDTH_OFFSET);
    for(int byteNumberToRead = 0; byteNumberToRead<4;byteNumberToRead++)
    {
        widthOfImage |= sdCard_fgetc(filePointer)<<(8*byteNumberToRead);
    }
    sdCard_fclose(filePointer);
    return widthOfImage;
}
void displayDrawBMP(int START_X, int START_Y, char *FILENAME, int NAME_SIZE)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    displaySetWindow(START_X,widthOfImage,START_Y,heightOfImage);
    for(int currentRow = 1; currentRow <= heightOfImage; currentRow++)
    {
        sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
        for(int currentColumn = 0; currentColumn<widthOfImage;currentColumn++)
        {
            colorHigh = sdCard_fbgetc(filePointer);
            colorLow = sdCard_fbgetc(filePointer);
            displayWriteData(colorLow);
            displayWriteData(colorHigh);
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPClearBackground(int START_X, int START_Y, char *FILENAME, int NAME_SIZE, int BACKGROUND_COLOR)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    displaySetWindow(START_X,widthOfImage,START_Y,heightOfImage);
    for(int currentRow = 1; currentRow <= heightOfImage; currentRow++)
    {
        sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
        for(int currentColumn = 0; currentColumn<widthOfImage;currentColumn++)
        {
            colorHigh = sdCard_fbgetc(filePointer);
            colorLow = sdCard_fbgetc(filePointer);
            if(!colorLow)
            {
                colorLow = BACKGROUND_COLOR >> 8;
            }
            if(!colorHigh)
            {
                colorHigh = BACKGROUND_COLOR & 0xFF;
            }
            displayWriteData(colorLow);
            displayWriteData(colorHigh);
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPPartial(int START_X, int START_Y, int END_X, int END_Y, char *FILENAME, int NAME_SIZE)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    displaySetWindow(START_X,widthOfImage,START_Y,heightOfImage);
    for(int currentRow = 1; currentRow <= END_Y; currentRow++)
    {
        sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
        for(int currentColumn = 0; currentColumn<END_X;currentColumn++)
        {
            colorHigh = sdCard_fbgetc(filePointer);
            colorLow = sdCard_fbgetc(filePointer);
            displayWriteData(colorLow);
            displayWriteData(colorHigh);
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPFast(int START_X, int START_Y, char *FILENAME, int NAME_SIZE, int NUMBER_OF_DIVISIONS)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    for(int rowStagger = 0; rowStagger<NUMBER_OF_DIVISIONS;rowStagger++)
    {
        for(int currentRow = 1+rowStagger; currentRow <= heightOfImage; currentRow+=NUMBER_OF_DIVISIONS)
        {
            displaySetWindow(START_X,widthOfImage,currentRow+START_Y,currentRow+1+START_Y);
            sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
            for(int currentColumn = 0; currentColumn<widthOfImage;currentColumn++)
            {
                colorHigh = sdCard_fbgetc(filePointer);
                colorLow = sdCard_fbgetc(filePointer);
                displayWriteData(colorLow);
                displayWriteData(colorHigh);
            }
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPPartialFast(int START_X, int START_Y, int END_X, int END_Y, char *FILENAME, int NAME_SIZE, int NUMBER_OF_DIVISIONS)
{

    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    for(int rowStagger = 0; rowStagger<NUMBER_OF_DIVISIONS;rowStagger++)
    {
        for(int currentRow = 1+rowStagger; currentRow <= END_Y; currentRow+=NUMBER_OF_DIVISIONS)
        {
            displaySetWindow(START_X,widthOfImage,currentRow+START_Y,currentRow+1+START_Y);
            sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
            for(int currentColumn = 0; currentColumn<END_X;currentColumn++)
            {
                colorHigh = sdCard_fbgetc(filePointer);
                colorLow = sdCard_fbgetc(filePointer);
                displayWriteData(colorLow);
                displayWriteData(colorHigh);
            }
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPFastClearBackground(int START_X, int START_Y, char *FILENAME, int NAME_SIZE, int BACKGROUND_COLOR, int NUMBER_OF_DIVISIONS)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int widthOfImage = getWidthOfBMP(fileToOpen,NAME_SIZE);
    int heightOfImage = getHeightOfBMP(fileToOpen,NAME_SIZE);
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    for(int rowStagger = 0; rowStagger<NUMBER_OF_DIVISIONS;rowStagger++)
    {
        for(int currentRow = 1+rowStagger; currentRow <= heightOfImage; currentRow+=NUMBER_OF_DIVISIONS)
        {
            displaySetWindow(START_X,widthOfImage,currentRow+START_Y,currentRow+1+START_Y);
            sdCard_fseek(filePointer, (sizeOfImage-currentRow*(widthOfImage*2)));
            for(int currentColumn = 0; currentColumn<widthOfImage;currentColumn++)
            {
                colorHigh = sdCard_fbgetc(filePointer);
                colorLow = sdCard_fbgetc(filePointer);
                if(!colorLow)
                {
                    colorLow = BACKGROUND_COLOR >> 8;
                }
                if(!colorHigh)
                {
                    colorHigh = BACKGROUND_COLOR & 0xFF;
                }
                displayWriteData(colorLow);
                displayWriteData(colorHigh);
            }
        }
    }
    sdCard_fclose(filePointer);
}
void displayDrawBMPPixel(int X_POSITION_DISPLAY, int Y_POSITION_DISPLAY, int X_POSITION_FILE, int Y_POSITION_FILE, char *FILENAME, int NAME_SIZE)
{
    char filePointer;
    char fileToOpen[NAME_SIZE];
    int fileNameLetterCounter = 0;
    uint offset;
    uint8 colorLow;
    uint8 colorHigh;
    while(FILENAME[fileNameLetterCounter]!=0)
    {
        fileToOpen[fileNameLetterCounter] = FILENAME[fileNameLetterCounter];
        fileNameLetterCounter++;
    }
    fileToOpen[NAME_SIZE-1] = 0;
    int sizeOfImage = getSizeOfImage(fileToOpen,NAME_SIZE);
    int dataOffsetOfImage = getDataOffsetBMP(fileToOpen,NAME_SIZE);

    filePointer = sdCard_fopen(fileToOpen, "r");
    displaySetWindow(X_POSITION_DISPLAY,ST7789_WIDTH-1,Y_POSITION_DISPLAY,ST7789_HEIGHT-1);
    sdCard_fseek(filePointer, (sizeOfImage-Y_POSITION_DISPLAY*(ST7789_WIDTH*2)));
    for(int wastedPixels = 0; wastedPixels<X_POSITION_FILE;wastedPixels++)
    {
        colorHigh = sdCard_fbgetc(filePointer);
        colorLow = sdCard_fbgetc(filePointer);
    }
    colorHigh = sdCard_fbgetc(filePointer);
    colorLow = sdCard_fbgetc(filePointer);
    displayWriteData(colorLow);
    displayWriteData(colorHigh);
    sdCard_fclose(filePointer);
}
void displayDrawLineOfBMP(int START_X, int END_X, int START_Y, int END_Y, char *FILENAME, int NAME_SIZE)
{
    int deltaX = absoluteValue(END_X-START_X);
    int xIncrement = START_X<END_X ? 1 : -1;
    int deltaY = absoluteValue(END_Y-START_Y);
    int yIncrement = START_Y<END_Y ? 1 : -1;
    int lineError = (deltaX>deltaY ? deltaX : -deltaY)/2;
    int oldLineError;

    for(;;)
    {
        displayDrawBMPPixel(START_X, START_Y, START_X, START_Y, FILENAME, NAME_SIZE);

        if (START_X==END_X && START_Y==END_Y) break;
        oldLineError = lineError;
        if (oldLineError >-deltaX)
        {
            lineError -= deltaY;
            START_X += xIncrement;
        }
        if (oldLineError < deltaY)
        {
            lineError += deltaX;
            START_Y += yIncrement;
        }
    }
}
void displayDrawLinePolarOfBMP(int START_X, int START_Y, int RADIUS, int ANGLE, char *FILENAME, int NAME_SIZE)
{
    float newAngle = 3.1416*ANGLE/180;
    int endX = START_X+RADIUS*cos(newAngle);
    int endY = START_Y+RADIUS*sin(newAngle);
    displayDrawLineOfBMP(START_X, endX, START_Y, endY, FILENAME, NAME_SIZE);
}
void displayDrawLinePolarThicknessOfBMP(int START_X, int START_Y, int RADIUS, int ANGLE, int THICKNESS, char *FILENAME, int NAME_SIZE)
{
    for(int j = THICKNESS/2; j>=0; j--)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolarOfBMP(START_X-i,START_Y-j,RADIUS,ANGLE,FILENAME,NAME_SIZE);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolarOfBMP(START_X+i,START_Y-j,RADIUS,ANGLE,FILENAME,NAME_SIZE);
        }
    }
    for(int j = 0; j<THICKNESS/2; j++)
    {
        for(int i = THICKNESS/2; i>=0; i--)
        {
            displayDrawLinePolarOfBMP(START_X-i,START_Y+j,RADIUS,ANGLE,FILENAME,NAME_SIZE);
        }
        for(int i = 0; i<THICKNESS/2; i++)
        {
            displayDrawLinePolarOfBMP(START_X+i,START_Y+j,RADIUS,ANGLE,FILENAME,NAME_SIZE);
        }
    }
}
*/
