#include "Scene.h"

StateScene::StateScene(int tx, int ty){
	x = tx; y = ty; w = SSW; h = SSH;
	startIndex = 0;
	selectIndex = 0;
	state = STATE_NORMAL;
	score = 0;
	sunsNumber = START_SUN_NUMBER;
	blockNumber = (w-2)/BLOCKW;
	plantsNumber = blockNumber-1;
	stateW = w-2-plantsNumber*BLOCKW;
	level = 0;
	addPlant();
}

StateScene::~StateScene(){

}

void StateScene::draw(){
	drawRect(x, y, w, h, BLUE);
	std::stringstream stateString;
	switch(state){
		case STATE_NORMAL:
			stateString << "NORMAL";
			drawText(x+2, y+1, stateString.str(), BLACK, WHITE);
			break;
		case STATE_PAUSE:
			stateString << "PAUSE";
			drawText(x+2, y+1, stateString.str(), YELLOW, WHITE);
			break;
		case STATE_PLANT:
			stateString << "PLANT";
			drawText(x+2, y+1, stateString.str(), GREEN, WHITE);
			break;
		case STATE_REMOVE:
			stateString << "REMOVE";
			drawText(x+2, y+1, stateString.str(), RED, WHITE);
			break;
		case STATE_OVER:
			stateString << "GAME OVER";
			drawText(x+2, y+1, stateString.str(), PURPLE, WHITE);
			break;

	}
	std::stringstream sunsString;
	sunsString << "SUN: " << sunsNumber;
	drawText(x+2, y+2, sunsString.str(), ORANGE, WHITE);
	std::stringstream scoreString;
	scoreString << "SCORE: " << score;
	drawText(x+2, y+3, scoreString.str(), BLUE, WHITE);
	std::stringstream levelString;
	levelString << "LEVEL: " << level;
	drawText(x+2, y+4, levelString.str(), BLACK, WHITE);



	drawLine(x+1, y+1, h-2, 0, LIGHTYELLOW);	
	drawLine(x+1+stateW-1, y+1, h-2, 0, LIGHTYELLOW);	

	
	for(int i = startIndex; i < std::min((int)infos.size(), startIndex+plantsNumber); ++i){
		drawText(x+1+stateW+(i-startIndex)*BLOCKW+1, y+3, infos[i].name, WHITE, infos[i].color);
		std::stringstream sunNumberString;
		sunNumberString << infos[i].needSunNumber;
		drawText(x+1+stateW+(i-startIndex)*BLOCKW+1, y+4, sunNumberString.str(), WHITE, ORANGE);
		std::stringstream countString;
		countString << infos[i].count;
		drawText(x+1+stateW+(i-startIndex)*BLOCKW+1, y+5, countString.str(), WHITE, LIGHTBLUE);
	}


	drawRect(x+1+stateW+selectIndex*BLOCKW, y+1, BLOCKW, BLOCKW, RED);
}

SceneSignal StateScene::update(){
	SceneSignal signal(SS_SIGNAL_NULL, 0);
	score++;	
	for(int i = 0; i < infos.size(); ++i){
		if(infos[i].count != 0){
			infos[i].count--;
		}
	}
	return signal;
}

void StateScene::selectRight(){
	if(selectIndex < plantsNumber-1 && selectIndex < infos.size()-1){
		selectIndex++;
	}
	else if(startIndex+selectIndex < infos.size() - 1){
		startIndex++;
	}
}

void StateScene::selectLeft(){
	if(selectIndex > 0){
		selectIndex--;
	}
	else if(startIndex > 0){
		startIndex--;
	}
}

SceneSignal StateScene::process(int state){
	SceneSignal signal(SS_SIGNAL_NULL, 0);
	this->state = state;
	if(state == STATE_NORMAL){

	}
	else if(state == STATE_PLANT){
		int tIdx = startIndex+selectIndex;
		if(infos[tIdx].count==0 && sunsNumber >= infos[tIdx].needSunNumber){
			signal.type = SS_SIGNAL_GEN_PLANT;
			signal.data = infos[tIdx].type;
		}
	}
	else if(state == STATE_REMOVE){
		signal.type = SS_SIGNAL_REMOVE_PLANT; 
	}
	return signal;
}

void StateScene::processSignal(SceneSignal& signal){
	if(signal.type == GS_SIGNAL_ADD_SUN){
		sunsNumber += signal.data;
	}
	else if(signal.type == GS_SIGNAL_PLANT_VALID){
		int tIdx = startIndex+selectIndex;
		if(signal.data && infos[tIdx].count == 0 && sunsNumber >= infos[tIdx].needSunNumber){
			infos[tIdx].count = infos[tIdx].coolDownTime;
			sunsNumber -= infos[tIdx].needSunNumber;
		}
	}
	else if(signal.type == GS_SIGNAL_ADD_SCORE){
		score += signal.data;
	}
	else if(signal.type == GS_SIGNAL_OVER){
		state = STATE_OVER;
	}
}

void StateScene::setState(int state){
	this->state = state;
}

void StateScene::addPlant(){
	plants.push_back(new SunFlower(0, 0));
	plants.push_back(new PeaShooter(0, 0));
	plants.push_back(new WallNut(0, 0));
	plants.push_back(new SnowPea(0, 0));
	plants.push_back(new MelonPult(0, 0));
	plants.push_back(new Repeator(0, 0));

	plants.push_back(new SnowMelon(0, 0));
	plants.push_back(new SpikeWeed(0, 0));
	plants.push_back(new Garlic(0, 0));
	plants.push_back(new Chomper(0, 0));
	plants.push_back(new Squash(0, 0));
	plants.push_back(new PotatoMine(0, 0));
	plants.push_back(new Jalapeno(0, 0));


	for(int i = 0; i < plants.size(); ++i){
		infos.push_back(plants[i]->getInfo());
	}
}

void StateScene::setLevel(int level){
	this->level = level;
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


GroundScene::GroundScene(int tx, int ty){
	x = tx; y = ty; w = GSW; h = GSH;
	selectX = 0;
	selectY = 0;
	count = 0;
	initSun();
	sunVectorN = GSBH*GSBW;
	randSortCount = 0;
	isRandSort = 0;
	score = 0;
	level = 0;
	genZombieCount = 0;
	genZombieSpeed = 0;
	genZombieNumber = 0;
	for(int i = 0; i < GSBH; ++i){
		weedKiller[i] = 1;
		for(int j = 0; j < GSBW; ++j){
			plants[i][j] = NULL;
			specialColor[i][j] = WHITE;
		}
	}

	/* objects.push_back(new Suns(x+1+3, y+1)); */
	/* objects.push_back(new SunFlower(x+1+3+1, y+1+2)); */
	/* objects.push_back(new PeaShooter(x+1+3+1, y+1+2+BLOCKW)); */
	/* objects.push_back(new NewsZombie(x+1+3+7*BLOCKW+1, y+1+2)); */
	/* objects.push_back(new NewsZombie(x+1+3+8*BLOCKW+1, y+1+2)); */
	/* objects.push_back(new DoorZombie(x+1+3+8*BLOCKW+1, y+1+2+1*BLOCKW)); */
	/* objects.push_back(new DancingZombie(x+1+3+8*BLOCKW+1, y+1+2+0*BLOCKW)); */
	/* weedKiller[1] = 0; */
	/* objects.push_back(new BucketZombie(x+1+3+6*BLOCKW+1, y+1+2+0*BLOCKW)); */
	/* objects.push_back(new BucketZombie(x+1+3+6*BLOCKW+1, y+1+2+2*BLOCKW)); */
	/* objects.push_back(new NormalZombie(x+1+3+7*BLOCKW+1+2, y+1+2+1*BLOCKW)); */
	/* objects.push_back(new NormalZombie(x+1+3+6*BLOCKW+1+2, y+1+2+0*BLOCKW)); */
	/* objects.push_back(new NormalZombie(x+1+3+6*BLOCKW+1+2, y+1+2+2*BLOCKW)); */
	/* objects.push_back(new BucketZombie(x+1+3+8*BLOCKW+1, y+1+2+1*BLOCKW)); */
	/* objects.push_back(new NormalZombie(x+1+3+8*BLOCKW+1+2, y+1+2+1*BLOCKW)); */
	/* objects.push_back(new BucketZombie(x+1+3+7*BLOCKW+1, y+1+2+2*BLOCKW)); */
	/* objects.push_back(new BucketZombie(x+1+3+8*BLOCKW+1, y+1+2+2*BLOCKW)); */
	/* genPlant(4, 1, OBJ_TYPE_POTATOMINE); */
	/* genPlant(0, 1, OBJ_TYPE_SNOWPEA); */
	/* genPlant(0, 1, OBJ_TYPE_MELONPULT); */
	/* genPlant(6, 0, OBJ_TYPE_SPIKEWEED); */
	/* genPlant(6, 2, OBJ_TYPE_WALLNUT); */
	/* genPlant(6, 1, OBJ_TYPE_GARLIC); */
	/* genPlant(0, 0, OBJ_TYPE_PEASHOOTER); */
	/* genPlant(0, 2, OBJ_TYPE_SNOWPEA); */
	/* genPlant(6, 1, OBJ_TYPE_SPIKEWEED); */
	/* genPlant(0, 2, OBJ_TYPE_SNOWMELON); */
	/* genPlant(0, 0, OBJ_TYPE_PEASHOOTER); */
}

GroundScene::~GroundScene(){
	
}

void GroundScene::initSun(){
	for(int i = 0; i < GSBH; ++i){
		for(int j = 0; j < GSBW; ++j){
			suns[i][j] = new Suns(x+1+3+j*BLOCKW, y+1+i*BLOCKW);
			objects.push_back(suns[i][j]);
		}
	}
}

void GroundScene::draw(){
	drawRect(x, y, w, h, DARKGREEN);
	drawLine(x+1+3-1, y+1, h-2, 0, LIGHTYELLOW);
	for(int i = 0; i < GSBH; ++i){
		if(weedKiller[i]){
			drawLine(x+1+1, y+1+2+i*BLOCKW, 3, 0, GRAY);
		}
		for(int j = 0; j < GSBW; ++j){
			if(specialColor[i][j] != WHITE){
				drawWholeRect(x+1+3+j*BLOCKW, y+1+i*BLOCKW, BLOCKW, BLOCKW, specialColor[i][j]);
				specialColor[i][j] = WHITE;
			}
		}
	}
	int selectBoxColor = BLACK;
	if(state == STATE_NORMAL){
		selectBoxColor = GRAY;
	}
	else if(state == STATE_PLANT){
		selectBoxColor = GREEN;
	}
	else if(state == STATE_REMOVE){
		selectBoxColor = RED;
	}
	drawRect(x+1+3+selectX*BLOCKW, y+1+selectY*BLOCKW, BLOCKW, BLOCKW, selectBoxColor);
	if(isRandSort){
		randSortCount = 10;
		isRandSort = 0;
	}
	else if(randSortCount != 0){
		randSortCount--;
	}
	for(int i = 0; i < objects.size(); ++i){
		if(randSortCount == 0 && i >= sunVectorN){
			int j = rand()%(objects.size()-i)+i;
			std::swap(objects[i], objects[j]);
			isRandSort = 1;
		}
		objects[i]->draw();
	}
}

SceneSignal GroundScene::update(){
	SceneSignal signal(GS_SIGNAL_NULL, 0);
	if(score != 0){
		signal.type = GS_SIGNAL_ADD_SCORE;
		signal.data = score;
		score = 0;
	}
	std::vector<Object*>::iterator ite = objects.begin()+sunVectorN;
	for(int i = 0; i < GSBH; ++i){
		for(int j = 0; j < GSBW; ++j){
			if(plants[i][j] != NULL && plants[i][j]->getIsDead()){
				plants[i][j] = NULL;
			}
		}
	}
	while(ite != objects.end()){
		if((*ite)->getIsDead()){
			ite = objects.erase(ite);
		}
		else{
			ite++;
		}
	}
	if(count == SUNFLOWER_GEN_SUN_SPEED){
		genSunRandom();
		count = 0;
	}
	else{
		count++;
	}
	for(int i = sunVectorN; i < objects.size(); ++i){
		int type0 = objects[i]->type%10;
		for(int j = i+1; j < objects.size(); ++j){
			int type1 = objects[j]->type%10;
			if(type0 == type1){
				continue;
			}
			else{
				if(type0 == OBJ_TYPE_BULLET && type1 == OBJ_TYPE_ZOMBIE){
					/* drawText(0, SH+2, "here1", BLACK, WHITE); */
					((Bullet*)objects[i])->interactive((Zombie*)objects[j]);
				}
				else if(type0 == OBJ_TYPE_PLANT && type1 == OBJ_TYPE_ZOMBIE){
					((Plant*)objects[i])->interactive((Zombie*)objects[j]);
					((Zombie*)objects[j])->interactive((Plant*)objects[i]);
				}
				else if(type0 == OBJ_TYPE_ZOMBIE){
					if(type1 == OBJ_TYPE_PLANT){
						((Plant*)objects[j])->interactive((Zombie*)objects[i]);
						((Zombie*)objects[i])->interactive((Plant*)objects[j]);
					}
					else if(type1 == OBJ_TYPE_BULLET){
						/* drawText(0, SH+2, "here2", BLACK, WHITE); */
						((Bullet*)objects[j])->interactive((Zombie*)objects[i]);
					}
				}
			}
		}
	}
	for(int i = 0; i < objects.size(); ++i){
		objects[i]->update();
		ObjectSignal os = objects[i]->getSignal();
		processObjSignal(os);
		if(state == STATE_OVER){
			signal.type = GS_SIGNAL_OVER; 
			return signal;
		}
	}
	randomGenZombie();


	std::stringstream debugString;
	debugString << "n: " << objects.size();
	drawText(0, SH+1, debugString.str(), BLACK, WHITE);
	return signal;
}

void GroundScene::selectLeft(){
	if(selectX > 0){
		selectX--;
	}
}

void GroundScene::selectRight(){
	if(selectX < GSBW-1){
		selectX++;
	}
}

void GroundScene::selectUp(){
	if(selectY > 0){
		selectY--;
	}
}

void GroundScene::selectDown(){
	if(selectY < GSBH-1){
		selectY++;
	}
}

void GroundScene::genSunRandom(){
	if(rand()%2){
		int x = rand()%GSBW;
		int y = rand()%GSBH;
		suns[y][x]->addSun();
	}		
}

SceneSignal GroundScene::process(int state){
	SceneSignal signal(GS_SIGNAL_NULL, 0);
	if(state == STATE_NORMAL){
		signal.type = GS_SIGNAL_ADD_SUN;
		signal.data = suns[selectY][selectX]->clearSun();
	}
	else if(state == STATE_PLANT){
		signal.type = GS_SIGNAL_PLANT_VALID;
		if(plants[selectY][selectX] == NULL){
			signal.data = 1;
		}	
		else{
			signal.data = 0;
		}
	}
	return signal;
}

void GroundScene::processSignal(SceneSignal& signal){
	if(signal.type == SS_SIGNAL_GEN_PLANT){
		genPlant(selectX, selectY, signal.data);
	}
	else if(signal.type == SS_SIGNAL_REMOVE_PLANT){
		if(plants[selectY][selectX]){
			plants[selectY][selectX]->die();
			plants[selectY][selectX] = NULL;
		}
	}
}

void GroundScene::genPlant(int xIndex, int yIndex, int type){
	if(plants[yIndex][xIndex] == NULL){
		int tx = x+1+3+xIndex*BLOCKW+1;
		int ty = y+1+yIndex*BLOCKW+2;
		if(type == OBJ_TYPE_SUNFLOWER){
			drawText(0, SH+2, "here2", BLACK, WHITE);
			plants[yIndex][xIndex] = new SunFlower(tx, ty);
		}
		else if(type == OBJ_TYPE_PEASHOOTER){
			plants[yIndex][xIndex] = new PeaShooter(tx, ty);	
		}
		else if(type == OBJ_TYPE_SNOWPEA){
			plants[yIndex][xIndex] = new SnowPea(tx, ty);
		}
		else if(type == OBJ_TYPE_MELONPULT){
			plants[yIndex][xIndex] = new MelonPult(tx, ty);
		}
		else if(type == OBJ_TYPE_SNOWMELON){
			plants[yIndex][xIndex] = new SnowMelon(tx, ty);
		}
		else if(type == OBJ_TYPE_SPIKEWEED){
			plants[yIndex][xIndex] = new SpikeWeed(tx, ty);
		}
		else if(type == OBJ_TYPE_GARLIC){
			plants[yIndex][xIndex] = new Garlic(tx, ty);
		}
		else if(type == OBJ_TYPE_CHOMPER){
			plants[yIndex][xIndex] = new Chomper(tx, ty);
		}
		else if(type == OBJ_TYPE_SQUASH){
			plants[yIndex][xIndex] = new Squash(tx, ty);
		}
		else if(type == OBJ_TYPE_POTATOMINE){
			plants[yIndex][xIndex] = new PotatoMine(tx, ty);
		}
		else if(type == OBJ_TYPE_JALAPENO){
			plants[yIndex][xIndex] = new Jalapeno(tx, ty);
		}
		else if(type == OBJ_TYPE_WALLNUT){
			plants[yIndex][xIndex] = new WallNut(tx, ty);
		}
		else if(type == OBJ_TYPE_REPEATOR){
			plants[yIndex][xIndex] = new Repeator(tx, ty);
		}
		objects.push_back(plants[yIndex][xIndex]);
	}
}

void GroundScene::processObjSignal(ObjectSignal& signal){
	if(signal.type == OBJ_SIGNAL_GEN_SUN){
		int tx = (signal.x-3-1-x)/BLOCKW;
		int ty = (signal.y-1-y)/BLOCKW;
		suns[ty][tx]->addSun();
	}
	else if(signal.type == OBJ_SIGNAL_GEN_BULLET){
		if(signal.data == OBJ_TYPE_PEABULLET){
			objects.push_back(new PeaBullet(signal.x, signal.y+1));
		}
		else if(signal.data == OBJ_TYPE_SNOWBULLET){
			objects.push_back(new SnowBullet(signal.x, signal.y+1));
		}
		else if(signal.data == OBJ_TYPE_MELONBULLET){
			objects.push_back(new MelonBullet(signal.x, signal.y+1));
		}
		else if(signal.data == OBJ_TYPE_SNOWMELONBULLET){
			objects.push_back(new SnowMelonBullet(signal.x, signal.y+1));
		}

	}
	else if(signal.type == OBJ_SIGNAL_SINGAL_COLOR){
		int tx = (signal.x-3-1-x)/BLOCKW;
		int ty = (signal.y-1-y)/BLOCKW;
		specialColor[ty][tx] = signal.data;
	}
	else if(signal.type == OBJ_SIGNAL_CROSS_COLOR){
		int tx = (signal.x-3-1-x)/BLOCKW;
		int ty = (signal.y-1-y)/BLOCKW;
		specialColor[ty][tx] = signal.data;
		if(ty-1 >= 0){
			specialColor[ty-1][tx] = signal.data;
		}
		if(ty+1 < GSBH){
			specialColor[ty+1][tx] = signal.data;
		}
		if(tx-1 >= 0){
			specialColor[ty][tx-1] = signal.data;
		}
		if(tx+1 < GSBW){
			specialColor[ty][tx+1] = signal.data;
		}
	}
	else if(signal.type == OBJ_SIGNAL_LINE_COLOR){
		int tx = (signal.x-3-1-x)/BLOCKW;
		int ty = (signal.y-1-y)/BLOCKW;
		for(int i = 0; i < GSBW; ++i){
			specialColor[ty][i] = signal.data;
		}	
	}
	else if(signal.type == OBJ_SIGNAL_ADD_SCORE){
		score += signal.data;
	}
	else if(signal.type == OBJ_SIGNAL_GET_LINE){	
		int ty = (signal.y-1-y)/BLOCKW;
		if(weedKiller[ty]){
			weedKiller[ty] = 0;
			for(int i = sunVectorN; i < objects.size(); ++i){
				if(objects[i]->type%10 == OBJ_TYPE_ZOMBIE){
					int oy = (objects[i]->y-1-y)/BLOCKW;
					if(ty == oy){
						objects[i]->die();
						score += 1000;
					}
				}
			}
			for(int i = 0; i < GSBW; ++i){
				specialColor[ty][i] = BLACK;
			}
		}
		else{
			state = STATE_OVER;
		}
	}
	else if(signal.type == OBJ_SIGNAL_GEN_ZOMBIE){
		int tx = signal.x;
		int ty = signal.y;
		if(tx - BLOCKW >= x+1+3){
			objects.push_back(new BackupZombie(tx-BLOCKW, ty));
		}
		if(tx + BLOCKW <= x+GSW){
			objects.push_back(new BackupZombie(tx+BLOCKW, ty));
		}
		if(ty - BLOCKW >= y+1){
			objects.push_back(new BackupZombie(tx, ty-BLOCKW));
		}
		if(ty + BLOCKW <= y+GSH){
			objects.push_back(new BackupZombie(tx, ty+BLOCKW));
		}
	}
}

void GroundScene::setState(int state){
	this->state = state;
}

void GroundScene::setLevel(int level){
	this->level = level;
}

void GroundScene::randomGenZombie(){
	if(level == 0){
		return;
	}
	genZombieSpeed = MAX_GAP_TIME-(level-1)*TIME_DESC;
	if(genZombieCount >= genZombieSpeed-1){
		genZombieCount = 0;
		genZombieNumber += (level+1)/2;
	}
	else{
		genZombieCount++;
	}
	if(genZombieNumber > 0){
		genZombieNumber--;
		genAZombie();
	}
}

void GroundScene::genAZombie(){
	int ty = rand()%GSBH;
	int type = (rand()%OBJ_ZOMBIE_NUMBER+1)*10+OBJ_TYPE_ZOMBIE;
	if(type == OBJ_TYPE_NORMALZOMBIE){
		objects.push_back(new NormalZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_CONEZOMBIE){
		objects.push_back(new ConeZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_BUCKETZOMBIE){
		objects.push_back(new BucketZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_NEWSZOMBIE){
		objects.push_back(new NewsZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_DOORZOMBIE){
		objects.push_back(new DoorZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_POLEZOMBIE){
		objects.push_back(new PoleZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_DANCINGZOMBIE){
		objects.push_back(new DancingZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
	else if(type == OBJ_TYPE_BACKUPZOMBIE){
		objects.push_back(new BackupZombie(x+1+3+(GSBW-1)*BLOCKW+1, y+1+ty*BLOCKW+2));
	}
}

Scene::Scene(int tx, int ty){
	x = tx; y = ty; w = SW; h = SH;
	state = STATE_NORMAL;
	level = 1;
	levelCount = 0;
	ss = new StateScene(x, y);
	gs = new GroundScene(x, y+SSH);
	ss->setLevel(level);
	gs->setLevel(level);
}

Scene::~Scene(){
	delete ss;
	delete gs;
}

void Scene::draw(){
	drawWholeRect(x, y, w, h, WHITE);
	ss->draw();
	gs->draw();
}

void Scene::update(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	if(levelCount >= LEVEL_UP_TIME-1){
		levelCount = 0;
		level++;
		ss->setLevel(level);
		gs->setLevel(level);
	}
	else if(level < MAX_LEVEL){
		levelCount++;
	}
	SceneSignal sss = ss->update();
	SceneSignal gss = gs->update();
	if(gss.type == GS_SIGNAL_OVER){
		state = STATE_OVER;
	}
	ss->processSignal(gss);
	gs->processSignal(sss);
}

void Scene::stateSceneSelectLeft(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	ss->selectLeft();
}

void Scene::stateSceneSelectRight(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	ss->selectRight();
}

void Scene::groundSceneSelectLeft(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	gs->selectLeft();
}

void Scene::groundSceneSelectRight(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	gs->selectRight();
}

void Scene::groundSceneSelectDown(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	gs->selectDown();
}

void Scene::groundSceneSelectUp(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	gs->selectUp();
}

void Scene::process(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	SceneSignal sss = ss->process(state);
	SceneSignal gss = gs->process(state);
	ss->processSignal(gss);
	gs->processSignal(sss);
}

void Scene::changeState(){
	if(state == STATE_OVER || state == STATE_PAUSE){
		return;
	}
	state = (state+1)%STATE_NUMBER;
	ss->setState(state);
	gs->setState(state);
}

void Scene::stopOrContinue(){
	if(state == STATE_PAUSE){
		state = STATE_NORMAL;
	}	
	else if(state != STATE_OVER){
		state = STATE_PAUSE;
	}
	ss->setState(state);
	gs->setState(state);

}

bool Scene::isStopOrOver(){
	if(state == STATE_PAUSE || state == STATE_OVER){
		return 1;
	}
	else{
		return 0;
	}
}
