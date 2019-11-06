#pragma once
#include "common.h"
#include "ScreenDraw.h"
#include "Object.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


#define STATE_NORMAL 0  // BLACK
#define STATE_PLANT  1  // GREEN 
#define STATE_REMOVE 2  // RED 
#define STATE_NUMBER 3  
#define STATE_PAUSE  4  // YELLOW
#define STATE_OVER   5  // PURPLE 

typedef struct SceneSignal{
	int type;
	int data;
	SceneSignal(int t, int d):type(t), data(d){}
}SceneSignal;

#define SS_SIGNAL_NULL         		0
#define SS_SIGNAL_GEN_PLANT        	1
#define SS_SIGNAL_REMOVE_PLANT 		2
	
#define GS_SIGNAL_NULL         		10
#define GS_SIGNAL_ADD_SUN      		11
#define GS_SIGNAL_PLANT_VALID  		12


class StateScene{
	private:
		
	public:
		int x, y, w, h;
		int startIndex;
		int selectIndex;
		int state;
		int score;
		int sunsNumber;
		int blockNumber;
		int plantsNumber;
		int stateW;
		std::vector<Plant*> plants;
		std::vector<PlantInfo> infos;
		
		StateScene(int tx, int ty);
		~StateScene();
		void draw();
		SceneSignal update();
		void selectLeft();
		void selectRight();
		SceneSignal process(int state);
		void processSignal(SceneSignal& signal);
		void setState(int state);
		void addPlant();
};

class GroundScene{
	private:
		
	public:
		int x, y, w, h;
		int selectX, selectY;
		int count;
		std::vector<Object*> objects;
		int sunVectorN;
		Suns* suns[GSBH][GSBW];
		Plant* plants[GSBH][GSBW];
		int state;
		int randSortCount;
		bool isRandSort;
		int specialColor[GSBH][GSBW];

		GroundScene(int tx, int ty);
		~GroundScene();
		void draw();
		SceneSignal update();
		void selectLeft();
		void selectRight();
		void selectDown();
		void selectUp();
		void genSunRandom();
		void initSun();	
		SceneSignal process(int state);
		void processSignal(SceneSignal& signal);
		void processObjSignal(ObjectSignal& signal);
		void genPlant(int x, int y, int type);
		void setState(int state);
};

class Scene{
	private:
	
	public:
		int x, y, w, h;
		StateScene* ss;
		GroundScene* gs;
		int state;

		Scene(int tx, int ty);
		~Scene();
		void draw();
		void update();
		void stateSceneSelectLeft();
		void stateSceneSelectRight();
		void groundSceneSelectLeft();
		void groundSceneSelectRight();
		void groundSceneSelectDown();
		void groundSceneSelectUp();
		void process();
		void changeState();
};
