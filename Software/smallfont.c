/*
 * smallfont.c
 *
 *  Created on: 28 Sep 2019
 *      Author: Nitwit
 */
#include "displayDriver.h"
#include "smallfont.h"
#include "imageOffsets.h"

int getWidthSmallCharacter(char CHARACTER) {
    int letterWidth = 0;

    switch(CHARACTER)
    {
        case ' ':
                letterWidth = 3;
                break;
        case 'A':
                letterWidth = 10;
                break;
        case 'a':
               letterWidth = 8;
                break;
        case 'B':
               letterWidth = 9;
                break;
        case 'b':
               letterWidth = 8;
                break;
        case 'C':
                letterWidth = 10;
                break;
        case 'c':

                letterWidth = 7;
                break;
        case 'D':

                letterWidth = 9;
                break;
        case 'd':

                letterWidth = 8;
                break;
        case 'E':

                letterWidth = 7;
                break;
        case 'e':

                letterWidth = 8;
                break;
        case 'F':

                letterWidth = 6;
                break;
        case 'f':

                letterWidth = 5;
                break;
        case 'G':

                letterWidth = 10;
                break;
        case 'g':

                letterWidth = 9;
                break;
        case 'H':

                letterWidth = 10;
                break;
        case 'h':

                letterWidth = 7;
                break;
        case 'I':
                letterWidth = 2;
                break;
        case 'i':

                letterWidth = 2;
                break;
        case 'J':
                letterWidth = 5;
                break;
        case 'j':

                letterWidth = 4;
                break;
        case 'K':

                letterWidth = 10;
                break;
        case 'k':

                letterWidth = 9;
                break;
        case 'L':

                letterWidth = 7;
                break;
        case 'l':

                letterWidth = 2;
                break;
        case 'M':

                letterWidth = 12;
                break;
        case 'm':

                letterWidth = 14;
                break;
        case 'N':

                letterWidth = 10;
                break;
        case 'n':

                letterWidth = 8;
                break;
        case 'O':

                letterWidth = 11;
                break;
        case 'o':

                letterWidth = 8;
                break;
        case 'P':

                letterWidth = 9;
                break;
        case 'p':

                letterWidth = 8;
                break;
        case 'Q':

                letterWidth = 11;
                break;
        case 'q':

                letterWidth = 8;
                break;
        case 'R':

                letterWidth = 9;
                break;
        case 'r':

                letterWidth = 5;
                break;
        case 'S':

                letterWidth = 9;
                break;
        case 's':

                letterWidth = 7;
                break;
        case 'T':

                letterWidth = 9;
                break;
        case 't':

                letterWidth = 6;
                break;
        case 'U':

                letterWidth = 9;
                break;
        case 'u':

                letterWidth = 8;
                break;
        case 'V':

                letterWidth = 11;
                break;
        case 'v':

                letterWidth = 8;
                break;
        case 'W':

                letterWidth = 15;
                break;
        case 'w':

                letterWidth = 12;
                break;
        case 'X':

                letterWidth = 10;
                break;
        case 'x':

                letterWidth = 8;
                break;
        case 'Y':

                letterWidth = 11;
                break;
        case 'y':

                letterWidth = 8;
                break;
        case 'Z':

                letterWidth = 8;
                break;
        case 'z':

                letterWidth = 6;
                break;
        case '1':

                letterWidth = 5;
                break;
        case '2':

                letterWidth = 8;
                break;
        case '3':

                letterWidth = 8;
                break;
        case '4':

                letterWidth = 9;
                break;
        case '5':

                letterWidth = 8;
                break;
        case '6':
                letterWidth = 9;
                break;
        case '7':

                letterWidth = 7;
                break;
        case '8':
                letterWidth = 8;
                break;
        case '9':
                letterWidth = 9;
                break;
        case '0':
                letterWidth = 9;
                break;
        case '.':
                letterWidth = 2;
                break;
        case ',':
                letterWidth = 2;
                break;
        case '!':
                letterWidth = 2;
                break;
        case '?':
                letterWidth = 8;
                break;
        case ':':
                letterWidth = 2;
                break;
        case '/':
                letterWidth = 6;
                break;
        case '"':
                letterWidth = 6;
                break;
        case '\'':
                letterWidth = 2;
                break;
        case '(':
                letterWidth = 4;
                break;
        case ')':
                letterWidth = 4;
                break;
        case '#':
                letterWidth = 9;
                break;
        case '$':
                letterWidth = 9;
                break;
        case '@':
                letterWidth = 15;
                break;
        case '*':
               letterWidth = 5;
               break;
        case '-':
               letterWidth = 5;
               break;
        case '+':
               letterWidth = 5;
               break;
        case '&':
               letterWidth = 12;
               break;
        case '€':
               letterWidth = 12;
               break;
        default:
                letterWidth = 5;
        break;


    }
    return letterWidth;
}

int displayDrawCharacter(int X_START, int Y_START, char CHARACTER)
{
    int letterWidth = getWidthSmallCharacter(CHARACTER);
    int row = 0;
    int column = 0;

    switch(CHARACTER)
    {
        case ' ':
                column = 1;
                row = 1;
                break;

        case 'A':
                column = 17;
                row = 2;
                break;
        case 'a':
                column = 15;
                row = 4;
                break;
        case 'B':
                column = 1;
                row = 3;
                break;
        case 'b':
                column = 16;
                row = 4;

                break;
        case 'C':
                column = 2;
                row = 3;
                break;
        case 'c':
                column = 17;
                row = 4;
                break;
        case 'D':
                column = 3;
                row = 3;
                break;
        case 'd':
                column = 1;
                row = 5;
                break;
        case 'E':
                column = 4;
                row = 3;
                break;
        case 'e':
                column = 2;
                row = 5;
                break;
        case 'F':
                column = 5;
                row = 3;
                break;
        case 'f':
                column = 3;
                row = 5;
                break;
        case 'G':
                column = 6;
                row = 3;
                break;
        case 'g':
                column = 4;
                row = 5;
                break;
        case 'H':
                column = 7;
                row = 3;
                break;
        case 'h':
                column = 5;
                row = 5;
                break;
        case 'I':
                column = 8;
                row = 3;
                break;
        case 'i':
                column = 6;
                row = 5;
                break;
        case 'J':
                column = 9;
                row = 3;
                break;
        case 'j':
                column = 7;
                row = 5;
                break;
        case 'K':
                column = 10;
                row = 3;
                break;
        case 'k':
                column = 8;
                row = 5;
                break;
        case 'L':
                column = 11;
                row = 3;
                break;
        case 'l':
                column = 9;
                row = 5;
                break;
        case 'M':
                column = 12;
                row = 3;
                break;
        case 'm':
                column = 10;
                row = 5;
                break;
        case 'N':
                column = 13;
                row = 3;
                break;
        case 'n':
                column = 11;
                row = 5;
                break;
        case 'O':
                column = 14;
                row = 3;
                break;
        case 'o':
                column = 12;
                row = 5;
                break;
        case 'P':
                column = 15;
                row = 3;
                break;
        case 'p':
                column = 13;
                row = 5;
                break;
        case 'Q':
                column = 16;
                row = 3;
                break;
        case 'q':
                column = 14;
                row = 5;
                break;
        case 'R':
                column = 17;
                row = 3;
                break;
        case 'r':
                column = 15;
                row = 5;
                break;
        case 'S':
                column = 1;
                row = 4;
                break;
        case 's':
                column = 16;
                row = 5;
                break;
        case 'T':
                column = 2;
                row = 4;
                break;
        case 't':
                column = 17;
                row = 5;
                break;
        case 'U':
                column = 3;
                row = 4;
                break;
        case 'u':
                column = 1;
                row = 6;
                break;
        case 'V':
                column = 4;
                row = 4;
                break;
        case 'v':
                column = 2;
                row = 6;
                break;
        case 'W':
                column = 5;
                row = 4;
                break;
        case 'w':
                column = 3;
                row = 6;
                break;
        case 'X':
                column = 6;
                row = 4;
                break;
        case 'x':
                column = 4;
                row = 6;
                break;
        case 'Y':
                column = 7;
                row = 4;
                break;
        case 'y':
                column = 5;
                row = 6;
                break;
        case 'Z':
                column = 8;
                row = 4;
                break;
        case 'z':
                column = 6;
                row = 6;
                break;
        case '1':
                column = 1;
                row = 2;
                break;
        case '2':
                column = 2;
                row = 2;
                break;
        case '3':
                column = 3;
                row = 2;
                break;
        case '4':
                column = 4;
                row = 2;
                break;
        case '5':
                column = 5;
                row = 2;
                break;
        case '6':
                column = 6;
                row = 2;
                break;
        case '7':
                column = 7;
                row = 2;
                break;
        case '8':
                column = 8;
                row = 2;
                break;
        case '9':
                column = 9;
                row = 2;
                break;
        case '0':
                column = 17;
                row = 1;
                break;
        case '.':
                column = 15;
                row = 1;
                break;
        case ',':
                column = 13;
                row = 1;
                break;
        case '!':
                column = 2;
                row = 1;
                break;
        case '?':
                column = 15;
                row = 2;
                break;
        case ':':
                column = 10;
                row = 2;
                break;
        case '/':
                column = 16;
                row = 1;
                break;
        case '"':
                column = 3;
                row = 1;
                break;
        case '\'':
                column = 8;
                row = 1;
                break;
        case '(':
                column = 9;
                row = 1;
                break;
        case ')':
                column = 10;
                row = 1;
                break;
        case '#':
                column = 4;
                row = 1;
                break;
        case '$':
                column = 5;
                row = 1;
                break;
        case '@':
                column = 16;
                row = 2;
                break;
        case '*':
               column = 11;
               row = 1;
               break;
        case '-':
               column = 14;
               row = 1;
               break;
        case '+':
               column = 11;
               row = 1;
               break;
        case '&':
               column = 7;
               row = 1;
               break;
        case '€':
               column = 12;
               row = 6;
               break;
        default:
                column = 1;
                row = 1;
                letterWidth = 5;
        break;


    }
    displayPartialImageFromMemory(X_START,Y_START,(column - 1) * FONT_CHARACTER_WIDTH , (row - 1) * FONT_CHARACTER_HEIGHT,FONT_CHARACTER_WIDTH,FONT_CHARACTER_HEIGHT,FONT_OFFSET);
    return letterWidth;
}
