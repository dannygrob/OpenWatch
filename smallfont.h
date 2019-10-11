/*
 * smallfont.h
 *
 *  Created on: 28 Sep 2019
 *      Author: Nitwit
 */

#ifndef SMALLFONT_H_
#define SMALLFONT_H_

#define FONT_CHARACTER_WIDTH 15
#define FONT_CHARACTER_HEIGHT 25
#define FONT_CHARACTERS_COLUMNS 17
#define FONT_CHARACTERS_ROWS 10

int getWidthSmallCharacter(char CHARACTER);
int displayDrawCharacter(int X_START, int Y_START, char CHARACTER);

#endif /* SMALLFONT_H_ */
