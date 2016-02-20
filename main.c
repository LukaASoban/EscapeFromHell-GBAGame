//Luka Antolic-Soban
#include <stdlib.h>
#include "myLib.h"
#include "game.h"
#include "startscreen.h"
#include "ghost.h"
#include "gameover.h"
#include "text.h"


int main() {

	REG_DISPCTL = MODE3 | BG2_ENABLE;

	int isReset = 1;

	while(1) {

		// WaitForVblank();
		if (KEY_DOWN_NOW(BUTTON_START)) {
			switch(state) {
			case TITLE: // start the game
				state = NORMAL;
				startGame();
				break;
			case GAMEOVER: // reset
				initiate();
				break;
			}
		} else if (KEY_DOWN_NOW(BUTTON_SELECT) || isReset) {
			initiate(); // reset whenever select is hit or at the start
			isReset = 0;
		} else if (state == NORMAL) {

			if(KEY_DOWN_NOW(BUTTON_A)) {
				goingUp = 1;
				jump();
			} else {
				goingUp = 0;
			}
			runGame();

		}
		
	}
	return 0;
}
