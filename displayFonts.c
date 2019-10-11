/*
 * displayFonts.c

 *
 *  Created on: Jan 25, 2019
 *      Author: samsonm
 */

#include "displayFonts.h"
#include "displayDriver.h"
#include "imageOffsets.h"
#include "math.h"
#include "smallfont.h"
#include "largefont.h"


int setCircularMargin(int CURRENT_Y_POSITION)
{
    int minimumCircularMargin = 80;
    int xMargin = 0;

    if(CURRENT_Y_POSITION<=20||CURRENT_Y_POSITION>=220)
    {
        xMargin = minimumCircularMargin*9/10;
    }
    else if(CURRENT_Y_POSITION<=40||CURRENT_Y_POSITION>=200)
    {
        xMargin = minimumCircularMargin*6/10;
    }
    else if(CURRENT_Y_POSITION<=60||CURRENT_Y_POSITION>=180)
    {
        xMargin = minimumCircularMargin*4/10;
    }
    else if(CURRENT_Y_POSITION<=80||CURRENT_Y_POSITION>=160)
    {
        xMargin = minimumCircularMargin*3/10;
    }
    else if(CURRENT_Y_POSITION<=100||CURRENT_Y_POSITION>=140)
    {
        xMargin = minimumCircularMargin*2/10;
    }
    else
    {
        xMargin = minimumCircularMargin*1/10;
    }
    return xMargin;
}

void displayDrawStringCentered(int Y_START, int KERNING_SIZE, int MARGIN, int POINTER_TO_STRING) {
        char stringToWrite[255]={NULL};
        strncpy(stringToWrite,POINTER_TO_STRING,255);
        int numberOfCharacters = 0;
        int stringLength = strlen(stringToWrite);
        int currentXLocation = 0;
        int currentYLocation = Y_START;
        int nextSpaceCount = 0;
        int xMargin = 0;
        int letterSize = FONT_CHARACTER_WIDTH;

        int minimumCircularMargin = 20;

        //steps:
        //1 - get the with of the total line
        //2 - start drawing the line with the right start offset
        //3 - move to next line
        //4 - etc

        if(Y_START<MARGIN+minimumCircularMargin)
        {
                currentYLocation = MARGIN+minimumCircularMargin;
        }

        int rowLettersWidth = 0;
        int stringOffset = 0;
        int lastSpacePosition = -1;
        int widthUntilLastSpace = -1;

        while(numberOfCharacters!=stringLength)
        {
                //check until we reach the end
                rowLettersWidth += getWidthSmallCharacter(stringToWrite[numberOfCharacters]) + KERNING_SIZE;

                if (stringToWrite[numberOfCharacters] == ' ') {
                        lastSpacePosition = numberOfCharacters;
                        widthUntilLastSpace = rowLettersWidth;
                }

                numberOfCharacters++;

                if (rowLettersWidth >= (ST7789_WIDTH - xMargin) || numberOfCharacters >= stringLength) {
                        //draw the string
                        int x = currentXLocation;

                        //if we had a space, and we're not at the end of the string yet
                        if (lastSpacePosition != -1 && numberOfCharacters < stringLength) {
                                numberOfCharacters = lastSpacePosition;
                                rowLettersWidth = widthUntilLastSpace;
                                lastSpacePosition = -1;
                        }

                        //calculate x offset for center aligned:
                        int xOffset = (ST7789_WIDTH - rowLettersWidth) / 2;
                        x = xOffset;
                        for (int i=stringOffset; i<numberOfCharacters; i++) {
                                int width = getWidthSmallCharacter(stringToWrite[i]);
                                displayDrawCharacter(x, currentYLocation, stringToWrite[i]);
                                x += width + KERNING_SIZE;
                        }
                        rowLettersWidth = 0;
                        stringOffset = numberOfCharacters;
                        currentYLocation += FONT_CHARACTER_HEIGHT;
                        xMargin = setCircularMargin(currentYLocation)+MARGIN;
                        currentXLocation = xMargin;
                }
        }
}
void displayDrawString(int X_START, int Y_START, int KERNING_SIZE, int MARGIN, int POINTER_TO_STRING)
{
    char stringToWrite[255]={NULL};
    strncpy(stringToWrite,POINTER_TO_STRING,255);
    int numberOfCharacters = 0;
    int stringLength = strlen(stringToWrite);
    int currentXLocation = X_START;
    int currentYLocation = Y_START;
    int nextSpaceCount = 0;
    int xMargin = 0;
    int letterSize = FONT_CHARACTER_WIDTH;

    int minimumCircularMargin = 20;

    if(Y_START<MARGIN+minimumCircularMargin)
    {
        currentYLocation = MARGIN+minimumCircularMargin;
    }

    xMargin = setCircularMargin(currentYLocation)+MARGIN;

    if(X_START<xMargin)
    {
        currentXLocation = xMargin;
    }
    while(numberOfCharacters!=stringLength)
    {
        if(stringToWrite[numberOfCharacters]==' ')
        {
            nextSpaceCount = 0;
            for(int nextSpaceLocation = 1; nextSpaceLocation<(stringLength-numberOfCharacters);nextSpaceLocation++)
            {
                if(stringToWrite[numberOfCharacters+nextSpaceLocation]==' ')
                {
                    break;
                }
                nextSpaceCount++;
            }
            //Check if the next space comes after the text reaches the edge of the writing area
            if(((currentXLocation+(nextSpaceCount*(FONT_CHARACTER_WIDTH+KERNING_SIZE)))>(ST7789_WIDTH-xMargin))||(nextSpaceCount==(stringLength-numberOfCharacters)))
            {
                numberOfCharacters++;
                currentYLocation += FONT_CHARACTER_HEIGHT;
                if((currentYLocation+FONT_CHARACTER_HEIGHT+MARGIN+minimumCircularMargin)>ST7789_HEIGHT)
                {
                    break;
                }
                xMargin = setCircularMargin(currentYLocation)+MARGIN;
                if(X_START>xMargin)
                {
                    currentXLocation = X_START;
                }
                else
                {
                    currentXLocation = xMargin;
                }
            }
        }
        if(((currentXLocation+(FONT_CHARACTER_WIDTH+KERNING_SIZE)+xMargin)>ST7789_WIDTH)&&(stringToWrite[numberOfCharacters]==' '))
        {
            currentYLocation += FONT_CHARACTER_HEIGHT;
            if((currentYLocation+FONT_CHARACTER_HEIGHT+MARGIN+minimumCircularMargin)>ST7789_HEIGHT)
            {
                break;
            }
            xMargin = setCircularMargin(currentYLocation)+MARGIN;
            if(X_START>xMargin)
            {
                currentXLocation = X_START;
            }
            else
            {
                currentXLocation = xMargin;
            }
        }
        else
        {
            letterSize = displayDrawCharacter(currentXLocation, currentYLocation, stringToWrite[numberOfCharacters]);
            currentXLocation += (letterSize+KERNING_SIZE);
        }
        numberOfCharacters++;
    }
}
/*
void displayDrawCharacter(int X_START, int Y_START, int SIZE, int COLOR, char CHARACTER)
{
    int xStartLocation = X_START;
    int xEndLocation = xStartLocation + SIZE;
    int yStartLocation = Y_START;
    int yEndLocation = yStartLocation + SIZE;
    switch(CHARACTER)
    {
        case 'A':
        case 'a':
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yEndLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yStartLocation,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/3,xStartLocation+SIZE*2/3,yStartLocation+SIZE*2/3,yStartLocation+SIZE*2/3,COLOR,SIZE/5);
        break;
        case 'B':
        case 'b':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/3,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/3,xEndLocation-SIZE/3,yStartLocation,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/3,xStartLocation,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation-SIZE/6,yStartLocation+SIZE/2,yStartLocation+SIZE*4/6,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/6,xEndLocation-SIZE/6,yStartLocation+SIZE*4/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/6,xStartLocation,yEndLocation,yEndLocation,COLOR,SIZE/5);
        break;
        case 'C':
        case 'c':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation+SIZE/6,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation-SIZE/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
        break;
        case 'D':
        case 'd':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
        break;
        case 'E':
        case 'e':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yEndLocation,yEndLocation,COLOR,SIZE/5);
        break;
        case 'F':
        case 'f':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
        break;
        case 'G':
        case 'g':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation+SIZE/6,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation-SIZE/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation-SIZE/2,yEndLocation-SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yEndLocation-SIZE/2,yEndLocation,COLOR,SIZE/5);
        break;
        case 'H':
        case 'h':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'I':
        case 'i':
            displayDrawLineThickness(xStartLocation+SIZE/3,xStartLocation+SIZE*2/3,yStartLocation,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xStartLocation+SIZE/2,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/3,xStartLocation+SIZE*2/3,yEndLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'J':
        case 'j':
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation-SIZE/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
        break;
        case 'K':
        case 'k':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/4,xStartLocation+SIZE*2/3,yStartLocation+SIZE/2,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/4,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/4,xEndLocation-SIZE/4,yStartLocation+SIZE/2,yEndLocation,COLOR,SIZE/5);
        break;
        case 'L':
        case 'l':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/4,yEndLocation,yEndLocation,COLOR,SIZE/5);
        break;
        case 'M':
        case 'm':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yStartLocation,yStartLocation+SIZE/2,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yStartLocation+SIZE/2,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'N':
        case 'n':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'O':
        case 'o':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation+SIZE/6,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation-SIZE/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
        break;
        case 'P':
        case 'p':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/6,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/6,xEndLocation-SIZE/6,yStartLocation,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/6,xStartLocation,yStartLocation+SIZE/2,yStartLocation+SIZE/2,COLOR,SIZE/5);
        break;
        case 'Q':
        case 'q':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation+SIZE/6,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yEndLocation-SIZE/6,yEndLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/6,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation+SIZE/6,yEndLocation-SIZE/6,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yStartLocation+SIZE/2,yEndLocation,COLOR, SIZE/5);
        break;
        case 'R':
        case 'r':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/3,yStartLocation,yStartLocation,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/3,xEndLocation-SIZE/3,yStartLocation,yStartLocation+SIZE*2/6,COLOR,SIZE/5);
            displayDrawLineThickness(xEndLocation-SIZE/3,xStartLocation,yStartLocation+SIZE*2/6,yStartLocation+SIZE/2,COLOR,SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yStartLocation+SIZE/2,yEndLocation,COLOR, SIZE/5);
        break;
        case 'S':
        case 's':
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yStartLocation+SIZE/4,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yStartLocation,yStartLocation+SIZE/4,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation+SIZE/4,yEndLocation-SIZE/4,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yEndLocation-SIZE/4,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/4,COLOR, SIZE/5);
        break;
        case 'T':
        case 't':
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xStartLocation+SIZE/2,yStartLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'U':
        case 'u':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation-SIZE/4,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yEndLocation-SIZE/4,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation,yEndLocation-SIZE/4,COLOR, SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation-SIZE/4,COLOR, SIZE/5);
        break;
        case 'V':
        case 'v':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation,yStartLocation,COLOR, SIZE/5);
        break;
        case 'W':
        case 'w':
            displayDrawLineThickness(xStartLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xStartLocation+SIZE/2,yEndLocation,yEndLocation-SIZE/2,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation-SIZE/2,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xEndLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
        break;
        case 'X':
        case 'x':
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yEndLocation,yStartLocation,COLOR, SIZE/5);
        break;
        case 'Y':
        case 'y':
            displayDrawLineThickness(xStartLocation,xEndLocation-SIZE/2,yStartLocation,yEndLocation-SIZE/2,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xEndLocation,yEndLocation-SIZE/2,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation+SIZE/2,xStartLocation+SIZE/2,yEndLocation-SIZE/2,yEndLocation,COLOR, SIZE/5);
        break;
        case 'Z':
        case 'z':
            displayDrawLineThickness(xStartLocation,xEndLocation,yStartLocation,yStartLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xEndLocation,xStartLocation,yStartLocation,yEndLocation,COLOR, SIZE/5);
            displayDrawLineThickness(xStartLocation,xEndLocation,yEndLocation,yEndLocation,COLOR, SIZE/5);
        break;
        default:
        break;
    }
}
*/


