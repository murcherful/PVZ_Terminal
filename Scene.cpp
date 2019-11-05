#include "Scene.h"

StateScene::StateScene(int tx, int ty){
	x = tx; y = ty; w = SSW; h = SSH;
	startIndex = 0;
	selectIndex = 0;
	state = STATE_NORMAL;
	score = 0;
	sunsNumber = 8;
	blockNumber = (w-2)/BLOCKW;
	plantsNumber = blockNumber-1;
	stateW = w-2-plantsNumber*BLOCKW;
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
}

void StateScene::setState(int state){
	this->state = state;
}

void StateScene::addPlant(){
	plants.push_back(new SunFlower(0, 0));
	plants.push_back(new PeaShooter(0, 0));
	plants.push_back(new SnowPea(0, 0));
	plants.push_back(new MelonPult(0, 0));
	plants.push_back(new SnowMelon(0, 0));
	plants.push_back(new SpikeWeed(0, 0));
	plants.push_back(new Garlic(0, 0));
	plants.push_back(new Chomper(0, 0));


	for(int i = 0; i < plants.size(); ++i){
		infos.push_back(plants[i]->getInfo());
	}
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
	for(int i = 0; i < GSBH; ++i){
		for(int j = 0; j < GSBW; ++j){
			plants[i][j] = NULL;
		}
	}

	/* objects.push_back(new Suns(x+1+3, y+1)); */
	/* objects.push_back(new SunFlower(x+1+3+1, y+1+2)); */
	/* objects.push_back(new PeaShooter(x+1+3+1, y+1+2+BLOCKW)); */
	/* objects.push_back(new NewsZombie(x+1+3+7*BLOCKW+1, y+1+2)); */
	/* objects.push_back(new NewsZombie(x+1+3+8*BLOCKW+1, y+1+2)); */
	objects.push_back(new BucketZombie(x+1+3+7*BLOCKW+1, y+1+2+1*BLOCKW));
	objects.push_back(new BucketZombie(x+1+3+8*BLOCKW+1, y+1+2+1*BLOCKW));
	/* objects.push_back(new BucketZombie(x+1+3+7*BLOCKW+1, y+1+2+2*BLOCKW)); */
	/* objects.push_back(new BucketZombie(x+1+3+8*BLOCKW+1, y+1+2+2*BLOCKW)); */
	genPlant(4, 1, OBJ_TYPE_CHOMPER);
	/* genPlant(0, 0, OBJ_TYPE_SNOWPEA); */
	/* genPlant(0, 1, OBJ_TYPE_SNOWMELON); */
	/* genPlant(6, 0, OBJ_TYPE_SPIKEWEED); */
	/* genPlant(6, 2, OBJ_TYPE_SPIKEWEED); */
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
	if(state == STATE_NORMAL){
		drawRect(x+1+3+selectX*BLOCKW, y+1+selectY*BLOCKW, BLOCKW, BLOCKW, GRAY);
	}
	else if(state == STATE_PLANT){
		drawRect(x+1+3+selectX*BLOCKW, y+1+selectY*BLOCKW, BLOCKW, BLOCKW, DARKGREEN);
	}
	else if(state == STATE_REMOVE){
		drawRect(x+1+3+selectX*BLOCKW, y+1+selectY*BLOCKW, BLOCKW, BLOCKW, RED);
	}
	for(int i = 0; i < objects.size(); ++i){
		objects[i]->draw();
	}
}

SceneSignal GroundScene::update(){
	SceneSignal signal(GS_SIGNAL_NULL, 0);
	std::vector<Object*>::iterator ite = objects.begin()+sunVectorN;
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
	}
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
}

void GroundScene::setState(int state){
	this->state = state;
}

Scene::Scene(int tx, int ty){
	x = tx; y = ty; w = SW; h = SH;
	state = STATE_NORMAL;
	ss = new StateScene(x, y);
	gs = new GroundScene(x, y+SSH);
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
	SceneSignal sss = ss->update();
	SceneSignal gss = gs->update();
	ss->processSignal(gss);
	gs->processSignal(sss);
}

void Scene::stateSceneSelectLeft(){
	ss->selectLeft();
}

void Scene::stateSceneSelectRight(){
	ss->selectRight();
}

void Scene::groundSceneSelectLeft(){
	gs->selectLeft();
}

void Scene::groundSceneSelectRight(){
	gs->selectRight();
}

void Scene::groundSceneSelectDown(){
	gs->selectDown();
}

void Scene::groundSceneSelectUp(){
	gs->selectUp();
}

void Scene::process(){
	SceneSignal sss = ss->process(state);
	SceneSignal gss = gs->process(state);
	ss->processSignal(gss);
	gs->processSignal(sss);
}

void Scene::changeState(){
	state = (state+1)%STATE_NUMBER;
	ss->setState(state);
	gs->setState(state);
}
