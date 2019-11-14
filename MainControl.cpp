#include "MainControl.h"
#include "ScreenDraw.h"
#include "Scene.h"

#include <iostream>
#include <unistd.h>

using namespace std;

MainControl::MainControl(int bt){
    breakTime = bt;
    init_keyboard();
    cout << "\033[?25l\n"; 
}

MainControl::~MainControl(){
    close_keyboard();
    cout << "\033[?25h\n"; 
}

void MainControl::start(){
	Scene scene(0, 0);
    while(1){
		scene.update();
		scene.draw();
		if(kbhit()){
			int key = readch();
			/* std::cout << key << std::endl; */
			if(key == 'a'){
				/* std::cout << "left" << std::endl; */
				scene.groundSceneSelectLeft();
			}
			else if(key == 'd'){
				scene.groundSceneSelectRight();
			}
			else if(key == 'w'){
				scene.groundSceneSelectUp();
			}
			else if(key == 's'){
				scene.groundSceneSelectDown();
			}
			else if(key == 'q'){
				scene.stateSceneSelectLeft();
			}
			else if(key == 'e'){
				scene.stateSceneSelectRight();
			}
			else if(key == 'f'){
				scene.changeState();
			}
			else if(key == 'p'){
				scene.stopOrContinue();
			}
			else if(scene.isStopOrOver() && key == 27){
				break;
			}
			else if(key == 10){
				/* std::cout << "enter" << std::endl; */
				scene.process();
			}
        }
        usleep(breakTime*1000);
    }
    screenClear();
    cout << "\033[0;0H";
} 

/* void MainControl::loadWelcome(){ */
/*     int shiftX = (MWIDTH-TITLEW)/2; */
/*     int shiftY = (MHEIGHT-TITLEH)/2; */
/*     for(int i = 0; i < TITLEH; ++i){ */
/*         for(int j = 0; j < TITLEW; ++j){ */
/*             drawPixel(shiftX+j, shiftY+i, Title[i][j]); */
/*         } */
/*     } */ 
/*     /1* drawText(shiftX+43, shiftY+12, "PLAYER1: PRESS '1' TO PALY", WHITE, BLACK); *1/ */
/*     /1* drawText(shiftX+45, shiftY+13, "press 'space' to jump", WHITE, BLACK); *1/ */
/*     /1* drawText(shiftX+43, shiftY+14, "PLAYER2: PRESS '2' TO PALY", WHITE, BLACK); *1/ */
/*     drawText(shiftX+43, shiftY+15, "PRESS 'h' TO SEE HELP", WHITE, BLACK); */
/*     /1* drawText(shiftX+45, shiftY+16, "player1 press 'l' to jump", WHITE, BLACK); *1/ */
/*     /1* drawText(shiftX+43, shiftY+17, "PRESS 'p' TO PAUSE", WHITE, BLACK); *1/ */
/*     /1* drawText(shiftX+43, shiftY+18, "PRESS 'ESC' TO QUIT", WHITE, BLACK); *1/ */
/*     /1* drawText(shiftX+43, shiftY+18, "PRESS 'r' TO SEE RECORD", WHITE, BLACK); *1/ */
/*     drawText(shiftX+51, shiftY+18, "HAVE FUN!!!:)", RED, BLACK); */
/*     drawText(shiftX+51, shiftY+20, "BY  MURCHERFUL", BULE, BLACK); */
/* } */

/* void MainControl::loadHelper(){ */
/*     int shiftX = (MWIDTH-HELPERW)/2; */
/*     int shiftY = (MHEIGHT-HELPERH)/2; */
/*     drawRect(shiftX, shiftY, HELPERW, HELPERH, BULE); */
/*     drawWholeRect(shiftX+1, shiftY+1, HELPERW-2, HELPERH-2, WHITE); */

/*     drawText(shiftX+8, shiftY+2, "HELPER", BLACK, WHITE); */

/*     drawText(shiftX+2, shiftY+4, "ONE PLAYER: PRESS '1' TO PALY", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+5, "press 'w' to jump", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+6, "press 'a' to move left", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+7, "press 'd' to move right", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+8, "TWO PLAYERS: PRESS '2' TO PALY", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+9, "player1 press 'w' to jump", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+10, "player1 press 'a' to move left", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+11, "player1 press 'd' to move right", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+12, "player2 press 'i' to jump", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+13, "player2 press 'j' to move left", BLACK, WHITE); */
/*     drawText(shiftX+4, shiftY+14, "player2 press 'l' to move right", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+15, "PRESS 'p' TO PAUSE", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+16, "PRESS 'ESC' TO QUIT OR GO BACK", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+17, "PRESS 'r' TO SEE RECORD", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+19, "IN THIS GAME YOU NEED TO KEEP THE", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+20, "BLUE BLOCK AWAY FROM RED BLOCKS AND", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+21, "WHITE BREAKS. YOUR SCORE WILL", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+22, "INCREASE UNTIL YOU DIE. YOU ONLY", BLACK, WHITE); */
/*     drawText(shiftX+2, shiftY+23, "GOT ONE LIFE.", BLACK, WHITE); */

/*     /1* drawText(shiftX+2, shiftY+10, "HAVE FUN!!!:)", RED, WHITE); *1/ */
/*     /1* drawText(shiftX+2, shiftY+11, "BY  MURCHERFUL", BULE, WHITE); *1/ */
   
/* } */

int MainControl::waitKey(){
    while(!kbhit()){
        usleep(breakTime*1000);
    }
    return readch();
}
