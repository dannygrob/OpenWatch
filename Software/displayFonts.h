/*
 * displayFonts.h
 *
 *  Created on: Jan 25, 2019
 *      Author: samsonm
 */

#ifndef DISPLAYFONTS_H_
#define DISPLAYFONTS_H_

int displayGetWidthCharacter(char CHARACTER, int bold);
int displayDrawCharacter(int X_START, int Y_START, char CHARACTER);
void displayDrawString(int X_START, int Y_START, int KERNING_SIZE, int MARGIN, int POINTER_TO_STRING);
void displayDrawStringCentered(int Y_START, int KERNING_SIZE, int MARGIN, int POINTER_TO_STRING);
int displayDrawCharacterNumber(int X_START, int Y_START, char CHARACTER, int bold);
int setCircularMargin(int CURRENT_Y_POSITION);

#endif /* DISPLAYFONTS_H_ */
