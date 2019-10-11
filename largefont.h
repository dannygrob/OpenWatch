/*
 * largefont.h
 *
 *  Created on: 28 Sep 2019
 *      Author: Nitwit
 */

#ifndef LARGEFONT_H_
#define LARGEFONT_H_

#define NUMBER_FONT_CHARACTER_WIDTH 50
#define NUMBER_FONT_CHARACTER_HEIGHT 50
#define NUMBER_FONT_CHARACTERS_COLUMNS 5
#define NUMBER_FONT_CHARACTERS_ROWS 5

int displayGetWidthCharacter(char CHARACTER, int bold);
int displayDrawCharacterNumber(int X_START, int Y_START, char CHARACTER, int bold);

#endif /* LARGEFONT_H_ */
