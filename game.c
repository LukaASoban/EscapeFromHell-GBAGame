#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "game.h"
#include "startscreen.h"
#include "ghost.h"
#include "gameover.h"
#include "text.h"

static int distance;
float upCount;
int goingUp;
int acceleration;
int speed;

/* Current state of the game: TITLE, NORMAL, GAMEOVER */
int state;

char buffer[50];

//declaration of general variables
WALL topWall[NUM_RECS];
WALL bottomWall[NUM_RECS];
WALL middleWall[3];
WALL oldMidWall[3];
WALL oldTopWall[NUM_RECS];
WALL oldBottomWall[NUM_RECS];
GARY gary;
GARY oldGary;

//prepares the game for the start
void initiate() {


	state = TITLE;
	drawImage3(0, 0, 240, 160, startscreen);

	goingUp = 0;
	acceleration = 1;
	distance = 0;
	upCount = 0;


	for (int i = 0; i < NUM_RECS; i++) {
		topWall[i].color = RED;
		topWall[i].width = RECWIDTH;
		topWall[i].height = RECHEIGHT;
		topWall[i].xPos = RECWIDTH * i;
		topWall[i].yPos = 0;
	}

	for (int i = 0; i < NUM_RECS; i++) {
		bottomWall[i].color = RED;
		bottomWall[i].width = RECWIDTH;
		bottomWall[i].height = RECHEIGHT;
		bottomWall[i].xPos = RECWIDTH * i;
		bottomWall[i].yPos = 140;
	}


	middleWall[0].color = RED;
	middleWall[0].width = RECWIDTH;
	middleWall[0].height = RECHEIGHT;
	middleWall[0].xPos = 20;
	middleWall[0].yPos = randomMidHeight();

	middleWall[1].color = RED;
	middleWall[1].width = RECWIDTH;
	middleWall[1].height = RECHEIGHT;
	middleWall[1].xPos = 120;
	middleWall[1].yPos = randomMidHeight();

	middleWall[2].color = RED;
	middleWall[2].width = RECWIDTH;
	middleWall[2].height = RECHEIGHT;
	middleWall[2].xPos = 200;
	middleWall[2].yPos = randomMidHeight();


	gary.xPos = 50;
	gary.yPos = 60;
	gary.height = 21;
	gary.width = 15;
	gary.image = ghost;

}

/*
* Changes the game state and draws the gameover image.
*/
void endGame() {
	state = GAMEOVER;
	speed = 2;
	drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover);
}

/*
* draws the initial layour of the game
*/
void startGame() {
	state = NORMAL;

	u16 bgcolor = BLACK;
	DMA[3].src = &bgcolor;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;

	for (int i = 0; i < NUM_RECS; i++) {
		drawRect3(topWall[i].yPos, topWall[i].xPos, topWall[i].width, topWall[i].height, topWall[i].color);
		drawRect3(bottomWall[i].yPos, bottomWall[i].xPos, bottomWall[i].width, bottomWall[i].height, bottomWall[i].color);
	}

}

//this method creates random heights for the bottom and top walls
int randomMidHeight() {
	int max = 1000;
	int min = 0;

	for (int x = 0; x < NUM_RECS; x++) {
		if(topWall[x].yPos > min)
			min = topWall[x].yPos;
		if (bottomWall[x].yPos < max)
			max = bottomWall[x].yPos;
	}

	min += RECHEIGHT;
	max -= (RECHEIGHT + min);

	int answer = min + (int)((rand() / (float)RAND_MAX) * max);

	return answer;
}

//updates the middle blocks in the middle by constantly moving them
void updateMiddle() {


	middleWall[0].xPos -= 4;
	middleWall[1].xPos -= 4;
	middleWall[2].xPos -= 4;

	if(middleWall[0].xPos < 0) {
		middleWall[0].xPos = 240;
		middleWall[0].yPos = randomMidHeight();
	}
	if(middleWall[1].xPos < 0) {
		middleWall[1].xPos = 240;
		middleWall[1].yPos = randomMidHeight();
	}
	if(middleWall[2].xPos < 0) {
		middleWall[2].xPos = 240;
		middleWall[2].yPos = randomMidHeight();
	}
}

//updates the bottom and top walls and shifts them to the left
void updateBarriers() {
	for (int i = 0; i < (NUM_RECS - 1); i++)
	{
		topWall[i].xPos = RECWIDTH * i;
		topWall[i].yPos = topWall[i+1].yPos;
		bottomWall[i].xPos = RECWIDTH * i;
		bottomWall[i].yPos = bottomWall[i+1].yPos;
	}

	lastRec();
}

//makes it so the last rectangle can't be off the screen
void lastRec() {
	if(topWall[22].yPos < 2)
		moveDown();
	else if(bottomWall[22].yPos > 158)
		moveUp();
	else {
		if((int)((rand() / (float)RAND_MAX) * 2) == 1)
			moveUp();
		else
			moveDown();
	}
}

//moves the walls down
void moveDown() {
	topWall[NUM_RECS - 1].xPos = RECWIDTH * (NUM_RECS - 1);
	topWall[NUM_RECS - 1].yPos += 1;
	bottomWall[NUM_RECS - 1].xPos = RECWIDTH * (NUM_RECS - 1);
	bottomWall[NUM_RECS - 1].yPos += 1;

}

//moves the walls up
void moveUp() {
	topWall[NUM_RECS - 1].xPos = RECWIDTH * (NUM_RECS - 1);
	topWall[NUM_RECS - 1].yPos -= 1;
	bottomWall[NUM_RECS - 1].xPos = RECWIDTH * (NUM_RECS - 1);
	bottomWall[NUM_RECS - 1].yPos -= 1;
}

/*
 *
 * @ destination_row: the row on the screen that the animation will begin at
 * @ destination_col: the column on the screen that the animation will begin at
 */
void animator(int theRow, int theCol)
{
	for (int i = 0; i < 4; i++)
	{
		WaitForVblank();
		draw_image_portion(theRow,theCol,0, i*15, 15, 21, ghost, GHOST_WIDTH);
	}
	
}

//checks to see if gary the ghost was hit and then returns either a zero or one
int isHit() {
	for(int x = 4; x <= 7; x++)
		if(gary.xPos + 21 >= bottomWall[x].yPos)
			return 1;
	for(int y = 4; y <= 7; y++)
		if(gary.xPos <= topWall[y].yPos + RECHEIGHT)
			return 1;
	for(int z = 0; z <= 3; z++)
		if(isInMidRange(z))
			return 1;
	return 0;

}

//checks to see if gary has collided with anything
int isInMidRange(int n) {
	if(gary.yPos <= (middleWall[n].xPos + middleWall[n].width) &&//editited this
		(gary.yPos + gary.width) >= middleWall[n].xPos &&
		gary.xPos <= (middleWall[n].yPos + middleWall[n].height) &&
		(gary.xPos + gary.height) >= middleWall[n].yPos)
		return 1;
	return 0;
}

//updates gary the ghost to move about
void updateGary() {

	speed += acceleration;
	gary.xPos += speed;
	
	if(isHit())
		endGame();
	
}

//makes Gary jump!
void jump() {
	speed = -6;
}


//calls all the updates and then checks for user button presses and draws everything to the screen.
void runGame() {

	if(state == GAMEOVER) {
		return;
	}

	oldGary = gary;

	updateGary();


	oldMidWall[0] = middleWall[0];
	oldMidWall[1] = middleWall[1];
	oldMidWall[2] = middleWall[2];
	updateMiddle();

	for (int i = 0; i < 3; i++) {
		drawRect3(oldMidWall[i].yPos, oldMidWall[i].xPos, oldMidWall[i].width, oldMidWall[i].height, BLACK);
		drawRect3(middleWall[i].yPos, middleWall[i].xPos, middleWall[i].width, middleWall[i].height, middleWall[i].color);
	}

	for (int i = 0; i < NUM_RECS; i++)
	{
		oldTopWall[i] = topWall[i];
		oldBottomWall[i] = bottomWall[i];
	}

	updateBarriers();

	

	for (int i = 0; i < NUM_RECS; i++) {
		drawRect3(oldBottomWall[i].yPos, bottomWall[i].xPos, bottomWall[i].width, bottomWall[i].height, BLACK);
	}

	for (int i = 0; i < NUM_RECS; i++) {
		drawRect3(bottomWall[i].yPos, bottomWall[i].xPos, bottomWall[i].width, bottomWall[i].height, bottomWall[i].color);
	}



	for (int i = 0; i < NUM_RECS; i++) {
		
		drawRect3(oldTopWall[i].yPos, topWall[i].xPos, topWall[i].width, topWall[i].height, BLACK);

	}

	for (int i = 0; i < NUM_RECS; i++) {
		
		drawRect3(topWall[i].yPos, topWall[i].xPos, topWall[i].width, topWall[i].height, topWall[i].color);
		
	}


	distance += 1;
	sprintf(buffer, "Distance: %d", distance);
	drawString(150, 5, buffer, WHITE);
	drawRect3(0,0,240,20,RED);
	
	drawRect3(oldGary.xPos, oldGary.yPos, 15, 21, BLACK);
	animator(gary.xPos, gary.yPos);
	if(isHit())
		endGame();
	

}
