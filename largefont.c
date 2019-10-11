/*
 * largefont.c
 *
 *  Created on: 28 Sep 2019
 *      Author: Nitwit
 */
#include "displayDriver.h"
#include "largefont.h"
#include "imageOffsets.h"

int displayGetWidthCharacter(char CHARACTER, int bold) {
        int letterWidth = 0;
        switch(CHARACTER)
        {
            case '1':

                    letterWidth = bold == 0 ? 14 : 17;
                    break;

            case '2':

                    letterWidth = bold == 0 ? 29 : 26;
                    break;
            case '3':

                    letterWidth = bold == 0 ? 30 : 27;
                    break;

            case '4':

                    letterWidth = bold == 0 ? 30 : 28;
                    break;
            case '5':

                    letterWidth = bold == 0 ? 30 : 26;
                    break;

            case '6':

                    letterWidth = bold == 0 ? 30 : 26;
                    break;
            case '7':

                    letterWidth = bold == 0 ? 27 : 26;
                    break;

            case '8':

                    letterWidth = bold == 0 ? 31 : 27;
                    break;
            case '9':

                    letterWidth = bold == 0 ? 30 : 27;
                    break;

            case '0':

                    letterWidth = bold == 0 ? 33 : 26;
                    break;
            case ':':

                    letterWidth = bold == 0 ? 1 : 8;
                    break;

        }

        return letterWidth;

}
int displayDrawCharacterNumber(int X_START, int Y_START, char CHARACTER, int bold) {

        int letterWidth = displayGetWidthCharacter(CHARACTER, bold);
        int row = 0;
        int column = 0;

        switch(CHARACTER)
        {
            case '1':
                    column = 2;
                    row = 1;
                    break;

            case '2':
                    column = 3;
                    row = 1;
                    break;
            case '3':
                    column = 4;
                    row = 1;
                    break;

            case '4':
                    column = 5;
                    row = 1;
                    break;
            case '5':
                    column = 1;
                    row = 2;
                    break;

            case '6':
                    column = 2;
                    row = 2;
                    break;
            case '7':
                    column = 3;
                    row = 2;
                    break;

            case '8':
                    column = 4;
                    row = 2;
                    break;
            case '9':
                    column = 5;
                    row = 2;
                    break;

            case '0':
                    column = 1;
                    row = 1;
                    break;
            case ':':
                    column = 1;
                    row = 3;
                    break;

        }
        if (bold == 1) {
                displayPartialImageFromMemory(X_START,Y_START,(column - 1) * NUMBER_FONT_CHARACTER_WIDTH , (row - 1) * NUMBER_FONT_CHARACTER_HEIGHT,NUMBER_FONT_CHARACTER_WIDTH,NUMBER_FONT_CHARACTER_HEIGHT,MEDIUMFONT_OFFSET);
        } else {
                displayPartialImageFromMemory(X_START,Y_START,(column - 1) * NUMBER_FONT_CHARACTER_WIDTH , (row - 1) * NUMBER_FONT_CHARACTER_HEIGHT,NUMBER_FONT_CHARACTER_WIDTH,NUMBER_FONT_CHARACTER_HEIGHT,LIGHTFONT_OFFSET);
        }
        return letterWidth;

}
