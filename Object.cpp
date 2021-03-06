#include "Object.h"

Object::Object(int tx, int ty, int ttype){
	x = tx; y = ty;
	type = ttype;
	isDead = 0;
}

void Object::draw(){

}

void Object::update(){

}

ObjectSignal Object::getSignal(){
	ObjectSignal signal(OBJ_SIGNAL_NULL, 0);
	return signal;
}

bool Object::getIsDead(){
	return isDead;
}

int Object::getX(){
	return x;
}

int Object::getY(){
	return y;
}

void Object::die(){
	isDead = 1;
}

Suns::Suns(int tx, int ty):Object(tx, ty, OBJ_TYPE_SUN){
	num = 0;
}

Suns::~Suns(){

}

void Suns::update(){

}

void Suns::draw(){
	if(num == 0){
		return;
	}
	drawWholeRect(x, y, 2, 2, ORANGE);
	std::stringstream ss;
	ss << num;
	drawText(x, y, ss.str(), WHITE, ORANGE);
}

void Suns::addSun(){
	num++;
}

int Suns::clearSun(){
	int temp = num;
	num = 0;
	return temp;
}

Charactor::Charactor(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor): Object(tx, ty, ttype){
	attackStart = 0;
	attackCount = 0;
	hp = thp;
	defense = tdefense;
	attack = tattack;
	attackSpeed = tattackSpeed;	
	name = " "+tname+" ";
	color = tcolor;
}

Charactor::~Charactor(){

}

void Charactor::defend(int objAttack){
	if(objAttack == -1){
		hp = 0;
		isDead = 1;
	}
	int hurt = (int)((double)objAttack*(1-(double)defense/(defense+ARG_DEFENSE)));
	if(hurt <= 0){
		hurt = 1;
	}
	if(hp-hurt<0){
		hp = 0;
	}
	else{
		hp -= hurt;
	}
}

bool Charactor::getIsAttack(){
	if(attackStart && attackCount == 1){
		return 1;
	}
	else{
		return 0;
	}
}

int Charactor::getAttack(){
	return attack;
}

void Charactor::update(){
	if(attackStart){
		attackCount = (attackCount+1)%attackSpeed;
	}
	if(hp <= 0){
		isDead = 1;
	}
}

void Charactor::startAttack(){
	attackStart = 1;
}

void Charactor::stopAttack(){
	attackStart = 0;
}

void Charactor::draw(){
	std::stringstream hpString;
	hpString << " HP: " << hp << " ";
	drawText(x, y, hpString.str(), WHITE, RED);
	drawText(x, y+1, name, WHITE, color);
}


bool Charactor::getIsAttackStart(){
	return attackStart;
}

int Charactor::getW(){
	return name.length()/2;
}

Zombie::Zombie(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor, int tspeed):Charactor(tx, ty, ttype, thp, tattack, tdefense, tattackSpeed, tname, tcolor){
	speed = tspeed;
	speedCopy = tspeed;
	moveCount = 0;
	slowDownCount = 0;
	isInteractive = 0;
}

Zombie::~Zombie(){

}

void Zombie::update(){
	Charactor::update();
	if(!isInteractive){
		stopAttack();
	}
	if(moveCount >= speed-1){
		x--;
		moveCount = 0;
	}
	else if(slowDownCount != 0){
		slowDownCount--;
	}
	else if(!Charactor::getIsAttackStart()){
		moveCount++;
	}
	isInteractive = 0;
}

void Zombie::draw(){
	std::stringstream hpString;
	hpString << " HP: " << hp << " ";
	if(slowDownCount != 0){
		drawText(x, y, hpString.str(), WHITE, LIGHTBLUE);
	}
	else{
		drawText(x, y, hpString.str(), WHITE, RED);
	}
	drawText(x, y+1, name, WHITE, color);
}

void Zombie::interactive(Plant* p){
	if(p->getIsZombieValid() && getY() == p->getY() && (p->getX()+p->getW()+ATTACK_MAX_DIS >= getX() && p->getX() <= getX())){
		isInteractive = 1;
		if(!getIsAttackStart()){
			startAttack();
		}
		else if(getIsAttack()){
			p->defend(getAttack());
			stopAttack();
		}
	}
}

void Zombie::slowDown(){
	slowDownCount = speed*0.75;
}

void Zombie::randomUpDonw(){
	if(y-BLOCKW <= SSH){
		y+=BLOCKW;
	}
	else if(y+BLOCKW >= SH){
		y-=BLOCKW;
	}
	else{
		if(rand()%2){
			y+=BLOCKW;
		}
		else{
			y-=BLOCKW;
		}
	}
}

ObjectSignal Zombie::getSignal(){
	ObjectSignal signal(OBJ_SIGNAL_NULL, 0);
	if(isDead){
		signal.type = OBJ_SIGNAL_ADD_SCORE;
		signal.data = 1000;
	}
	else if(x <= 3){
		signal.type = OBJ_SIGNAL_GET_LINE;
		signal.x = x;
		signal.y = y;
	}
	return signal;
}

NormalZombie::NormalZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_NORMALZOMBIE, NORMALZOMBIE_HP, NORMALZOMBIE_ATTACK, NORMALZOMBIE_DEFENSE, NORMALZOMBIE_ATTACK_SPEED, "NormalZ", BLACK, NORMALZOMBIE_SPEED){

}

NormalZombie::~NormalZombie(){

}

ConeZombie::ConeZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_CONEZOMBIE, CONEZOMBIE_HP, CONEZOMBIE_ATTACK, CONEZOMBIE_DEFENSE, CONEZOMBIE_ATTACK_SPEED, "ConeZ", BLACK, CONEZOMBIE_SPEED){

}

ConeZombie::~ConeZombie(){

}

BucketZombie::BucketZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_BUCKETZOMBIE, BUCKETZOMBIE_HP, BUCKETZOMBIE_ATTACK, BUCKETZOMBIE_DEFENSE, BUCKETZOMBIE_ATTACK_SPEED, "BucketZ", BLACK, BUCKETZOMBIE_SPEED){

}

BucketZombie::~BucketZombie(){

}

NewsZombie::NewsZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_NEWSZOMBIE, NEWSZOMBIE_HP, NEWSZOMBIE_ATTACK, NEWSZOMBIE_DEFENSE, NEWSZOMBIE_ATTACK_SPEED, "NewsZ", BLACK, NEWSZOMBIE_SPEED){

}

NewsZombie::~NewsZombie(){

}

void NewsZombie::update(){
	Zombie::update();
	if(hp < 150){
		speed = speedCopy*0.5;
	}
}

DoorZombie::DoorZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_DOORZOMBIE, DOORZOMBIE_HP, DOORZOMBIE_ATTACK, DOORZOMBIE_DEFENSE, DOORZOMBIE_ATTACK_SPEED, "DoorZ", LIGHTGRAY, DOORZOMBIE_SPEED){

}

DoorZombie::~DoorZombie(){

}

PoleZombie::PoleZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_POLEZOMBIE, POLEZOMBIE_HP, POLEZOMBIE_ATTACK, POLEZOMBIE_DEFENSE, POLEZOMBIE_ATTACK_SPEED, "PoleZ", PINK, POLEZOMBIE_SPEED){
	isJump = 0;
}

PoleZombie::~PoleZombie(){

}

void PoleZombie::interactive(Plant* p){
	if(p->getIsZombieValid() && getY() == p->getY() && (p->getX()+p->getW()+ATTACK_MAX_DIS >= getX() && p->getX() <= getX())){
		isInteractive = 1;
		if(isJump == 0){
			x -= BLOCKW;
			isJump = 1;
			speed = NORMALZOMBIE_SPEED;
			return;
		}
		if(!getIsAttackStart()){
			startAttack();
		}
		else if(getIsAttack()){
			p->defend(getAttack());
			stopAttack();
		}
	}
}

DancingZombie::DancingZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_DANCINGZOMBIE, DANCINGZOMBIE_HP, DANCINGZOMBIE_ATTACK, DANCINGZOMBIE_DEFENSE, DANCINGZOMBIE_ATTACK_SPEED, "DancingZ", DARKRED, DANCINGZOMBIE_SPEED){
	genZomCount = 0;
	genZomSpeed = DANCINGZOMBIE_GEN_ZOM_SPEED;
}

DancingZombie::~DancingZombie(){

}

void DancingZombie::update(){
	Zombie::update();
	genZomCount = (genZomCount+1)%genZomSpeed;
}

ObjectSignal DancingZombie::getSignal(){
	ObjectSignal signal(OBJ_SIGNAL_NULL, 0);
	if(genZomCount == genZomSpeed-1){
		signal.type = OBJ_SIGNAL_GEN_ZOMBIE;
		signal.data = OBJ_TYPE_BACKUPZOMBIE;
		signal.x = x;
		signal.y = y;
	}
	return signal;
}

BackupZombie::BackupZombie(int tx, int ty):Zombie(tx, ty, OBJ_TYPE_BACKUPZOMBIE, BACKUPZOMBIE_HP, BACKUPZOMBIE_ATTACK, BACKUPZOMBIE_DEFENSE, BACKUPZOMBIE_ATTACK_SPEED, "BackupZ", YELLOW, BACKUPZOMBIE_SPEED){

}

BackupZombie::~BackupZombie(){

}

Plant::Plant(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor, int tneedSunNumber, int tcoolDownTime, int tbulletType, bool tisZombieValid):Charactor(tx, ty, ttype, thp, tattack, tdefense, tattackSpeed, tname, tcolor){
	needSunNumber = tneedSunNumber;
	coolDownTime = tcoolDownTime;
	bulletType = tbulletType;
	isZombieValid = tisZombieValid;
}

Plant::~Plant(){

}

bool Plant:: getIsZombieValid(){
	return isZombieValid;
}

bool Plant::getHasBullet(){
	if(bulletType != -1){
		return 1;
	}
	else{
		return 0;
	}
}

int Plant::getNeedSunNumber(){
	return needSunNumber;
}

void Plant::interactive(Zombie* z){
	if(getHasBullet()){
		if(getY() == z->getY() && getX() <= z->getX()){
			if(!getIsAttackStart()){
				startAttack();
			}
		}
	}
	else if(attack != 0){
		if(getY() == z->getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && getX() <= z->getX()){
			if(!getIsAttackStart()){
				startAttack();
			}
			else if(getIsAttack()){
				z->defend(getAttack());
				stopAttack();
			}
		}
	}
}

PlantInfo Plant::getInfo(){
	return PlantInfo(name, color, type, needSunNumber, coolDownTime);
}

ObjectSignal Plant::getSignal(){
	ObjectSignal signal(OBJ_SIGNAL_NULL, 0);
	if(bulletType != -1){
		if(getIsAttack()){
			signal.type = OBJ_SIGNAL_GEN_BULLET;
			signal.data = bulletType;
			signal.x = x;
			signal.y = y;
			stopAttack();
		}	
	}
	return signal;
}

SunFlower::SunFlower(int tx, int ty):Plant(tx, ty, OBJ_TYPE_SUNFLOWER, SUNFLOWER_HP, SUNFLOWER_ATTACK, SUNFLOWER_DEFENSE, SUNFLOWER_ATTACK_SPEED, "SunFlower", YELLOW, SUNFLOWER_NEED_SUN_NUMBER, SUNFLOWER_COOLDOWN_TIME, -1, 1){
	genSunCount = 0;
	genSunSpeed = SUNFLOWER_GEN_SUN_SPEED;
}

SunFlower::~SunFlower(){

}

ObjectSignal SunFlower::getSignal(){
	ObjectSignal signal(OBJ_SIGNAL_NULL, 0);
	if(genSunCount==1){
		signal.type = OBJ_SIGNAL_GEN_SUN;
		signal.x = x;
		signal.y = y;
	}
	return signal;
}

void SunFlower::update(){
	Charactor::update();
	genSunCount = (genSunCount+1)%genSunSpeed;
}

PeaShooter::PeaShooter(int tx, int ty):Plant(tx, ty, OBJ_TYPE_PEASHOOTER, PEASHOOTER_HP, PEASHOOTER_ATTACK, PEASHOOTER_DEFENSE, PEASHOOTER_ATTACK_SPEED, "PeaShooter", GREEN, PEASHOOTER_NEED_SUN_NUMBER, PEASHOOTER_COOLDOWN_TIME, OBJ_TYPE_PEABULLET, 1){

}

PeaShooter::~PeaShooter(){

}

SnowPea::SnowPea(int tx, int ty):Plant(tx, ty, OBJ_TYPE_SNOWPEA, SNOWPEA_HP, SNOWPEA_ATTACK, SNOWPEA_DEFENSE, SNOWPEA_ATTACK_SPEED, "SnowPea", BLUE, SNOWPEA_NEED_SUN_NUMBER, SNOWPEA_COOLDOWN_TIME, OBJ_TYPE_SNOWBULLET, 1){

}

SnowPea::~SnowPea(){

}

MelonPult::MelonPult(int tx, int ty):Plant(tx, ty, OBJ_TYPE_MELONPULT, MELONPULT_HP, MELONPULT_ATTACK, MELONPULT_DEFENSE, MELONPULT_ATTACK_SPEED, "MelonPult", GREEN, MELONPULT_NEED_SUN_NUMBER, MELONPULT_COOLDOWN_TIME, OBJ_TYPE_MELONBULLET, 1){

}

MelonPult::~MelonPult(){

}

SnowMelon::SnowMelon(int tx, int ty):Plant(tx, ty, OBJ_TYPE_SNOWMELON, SNOWMELON_HP, SNOWMELON_ATTACK, SNOWMELON_DEFENSE, SNOWMELON_ATTACK_SPEED, "SnowMelon", BLUE, SNOWMELON_NEED_SUN_NUMBER, SNOWMELON_COOLDOWN_TIME, OBJ_TYPE_SNOWMELONBULLET, 1){

}

SnowMelon::~SnowMelon(){

}

SpikeWeed::SpikeWeed(int tx, int ty):Plant(tx, ty, OBJ_TYPE_SPIKEWEED, SPIKEWEED_HP, SPIKEWEED_ATTACK, SPIKEWEED_DEFENSE, SPIKEWEED_ATTACK_SPEED, "SpikeWeed", BROWN, SPIKEWEED_NEED_SUN_NUMBER, SPIKEWEED_COOLDOWN_TIME, -1, 0){

}

SpikeWeed::~SpikeWeed(){

}

void SpikeWeed::interactive(Zombie* z){
	if(getY() == z->getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && 
			z->getX()+z->getW()+ATTACK_MAX_DIS >= getX()){
		if(!getIsAttackStart()){
			startAttack();
		}
		else if(getIsAttack()){
			z->defend(getAttack());
			hp-=SPIKEWEED_HP_DESC;
			stopAttack();
		}
	}
}

Garlic::Garlic(int tx, int ty):Plant(tx, ty, OBJ_TYPE_GARLIC, GARLIC_HP, GARLIC_ATTACK, GARLIC_DEFENSE, GARLIC_ATTACK_SPEED, "Garlic", LIGHTGRAY, GARLIC_NEED_SUN_NUMBER, GARLIC_COOLDOWN_TIME, -1, 1){

}

Garlic::~Garlic(){

}

void Garlic::interactive(Zombie* z){
	if(z->getY() == getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && z->getX()+z->getW()+ATTACK_MAX_DIS >= getX()){
		z->randomUpDonw();	
		hp -= GARLIC_HP_DESC;
	}	
}

Chomper::Chomper(int tx, int ty):Plant(tx, ty, OBJ_TYPE_CHOMPER, CHOMPER_HP, CHOMPER_ATTACK, CHOMPER_DEFENSE, CHOMPER_ATTACK_SPEED, "Chomper", PURPLE, CHOMPER_NEED_SUN_NUMBER, CHOMPER_COOLDOWN_TIME, -1, 1){

}

Chomper::~Chomper(){

}


void Chomper::update(){
	if(attackCount != 0){
		attackCount--;
	}
	if(hp <= 0){
		isDead = 1;
	}
}

void Chomper::draw(){
	std::stringstream hpString;
	hpString << " HP: " << hp << " ";
		drawText(x, y, hpString.str(), WHITE, RED);
	if(attackCount != 0){
		drawText(x, y+1, name, WHITE, BLACK);
	}
	else{
		drawText(x, y+1, name, WHITE, color);
	}

}

void Chomper::interactive(Zombie* z){
	if(getY() == z->getY() && getX()+getW()+CHOMPER_ATTACK_DIS >= z->getX() && getX() <= z->getX()){
		if(attackCount == 0){
			z->defend(getAttack());
			attackCount = attackSpeed;
		}
	}
}

Squash::Squash(int tx, int ty):Plant(tx, ty, OBJ_TYPE_SQUASH, SQUASH_HP, SQUASH_ATTACK, SQUASH_DEFENSE, SQUASH_ATTACK_SPEED, "Squash", SQUASHGREEN, SQUASH_NEED_SUN_NUMBER, SQUASH_COOLDOWN_TIME, -1, 0){
	isBreak = 0;
	isMove = 0;
}

Squash::~Squash(){

}


void Squash::update(){
	if(isBreak == 1){
		if(x + BLOCKW < SW){
			x+=BLOCKW;
		}
		isMove = 6;
		isBreak = 2;
	}
	else if(isBreak == 2 && isMove != 0){
		isMove--;
	}	
	else if(isBreak == 2 && isMove == 0){
		isDead = 1;
	}
}

void Squash::draw(){
	std::stringstream hpString;
	hpString << " HP: " << hp << " ";
		drawText(x, y, hpString.str(), WHITE, RED);
	if(isBreak==2){
		drawText(x, y+1, name, WHITE, DARKGREEN);
	}
	else{
		drawText(x, y+1, name, WHITE, color);
	}

}

void Squash::interactive(Zombie* z){
	if(isBreak == 0 && getY() == z->getY() && getX()+BLOCKW >= z->getX() && getX() <= z->getX()){
		if(isMove == 0){
			z->defend(getAttack());
			isBreak = 1;
		}
	}
	else if(isBreak == 2 && getY() == z->getY() && getX()+BLOCKW*0.5 >= z->getX() && getX()-BLOCKW*0.5 <= z->getX()){
		if(isMove == 0){
			z->defend(getAttack());
		}
	}
}

ObjectSignal Squash::getSignal(){
	ObjectSignal singal(OBJ_SIGNAL_NULL, 0);
	if(isBreak == 2){
		singal.type = OBJ_SIGNAL_SINGAL_COLOR;
		singal.data = SQUASHGREEN;
		singal.x = x;
		singal.y = y;
	}
	return singal;
}

PotatoMine::PotatoMine(int tx, int ty):Plant(tx, ty, OBJ_TYPE_POTATOMINE, POTATOMINE_HP, POTATOMINE_ATTACK, POTATOMINE_DEFENSE, POTATOMINE_ATTACK_SPEED, "PotatoMine", BROWN, POTATOMINE_NEED_SUN_NUMBER, POTATOMINE_COOLDOWN_TIME, -1, 0){
	isBreak = 0;
	readyCount = POTATOMINE_READY_COUNT;
	stayCount = 0;
}

PotatoMine::~PotatoMine(){

}

void PotatoMine::update(){
	if(readyCount != 0){
		readyCount--;
	}
	if(isBreak == 1){
		isBreak = 2;
	}
	else if(isBreak == 2){
		stayCount = 5;
		isBreak = 3;
	}
	else if(stayCount != 0){
		stayCount--;
	}
	if(isBreak == 3 && stayCount == 0){
		isDead = 1;
	}
}

void PotatoMine::draw(){
	std::stringstream hpString;
	if(readyCount != 0){
		drawText(x, y+1, name, WHITE, BLACK);
	}
	else if(isBreak == 0){
		drawText(x, y+1, name, WHITE, color);
	}
	else{
		drawText(x, y+1, name, WHITE, BLACK);
	}

}

void PotatoMine::interactive(Zombie* z){
	if(readyCount == 0 && isBreak == 0 && getY() == z->getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && getX() <= z->getX()){
		isBreak = 1;
	}
	else if(isBreak == 2){
		if(getY() == z->getY() && getX()+getW()+BLOCKW >= z->getX() && getX()-BLOCKW <= z->getX()){
			z->defend(getAttack());
		}
		else if((getY() == z->getY()+BLOCKW || getY() == z->getY()-BLOCKW) && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && z->getX()+z->getW()+ATTACK_MAX_DIS >= getX()){
			z->defend(getAttack());
		}
	}  
}

ObjectSignal PotatoMine::getSignal(){
	ObjectSignal singal(OBJ_SIGNAL_NULL, 0);
	if(isBreak > 1){
		singal.type = OBJ_SIGNAL_CROSS_COLOR;
		singal.data = BLACK;
		singal.x = x;
		singal.y = y;
	}
	return singal;
}

Jalapeno::Jalapeno(int tx, int ty):Plant(tx, ty, OBJ_TYPE_JALAPENO, JALAPENO_HP, JALAPENO_ATTACK, JALAPENO_DEFENSE, JALAPENO_ATTACK_SPEED, "Jalapeno", RED, JALAPENO_NEED_SUN_NUMBER, JALAPENO_COOLDOWN_TIME, -1, 0){
	isBreak = 0;
	readyCount = JALAPENO_READY_COUNT;
	stayCount = 0;
}

Jalapeno::~Jalapeno(){

}

void Jalapeno::update(){
	if(readyCount != 0){
		readyCount--;
	}
	else if(isBreak == 0){
		isBreak = 1;
	}
	if(isBreak == 2 && stayCount != 0){
		stayCount--;
	}
	else if(isBreak == 2 && stayCount == 0){
		isDead = 1;
	}
}

void Jalapeno::draw(){
	if(isBreak != 2){
		drawText(x, y+1, name, WHITE, color);
	}
	else{
		drawText(x, y+1, name, WHITE, BLACK);	
	}
}

void Jalapeno::interactive(Zombie* z){
	if(isBreak > 0 && getY() == z->getY()){
		z->defend(getAttack());
		stayCount = 5;
		isBreak = 2;
	}
}

ObjectSignal Jalapeno::getSignal(){
	ObjectSignal singal(OBJ_SIGNAL_NULL, 0);
	if(isBreak > 1){
		singal.type = OBJ_SIGNAL_LINE_COLOR;
		singal.data = BLACK;
		singal.x = x;
		singal.y = y;
	}
	return singal;
}

WallNut::WallNut(int tx, int ty):Plant(tx, ty, OBJ_TYPE_WALLNUT, WALLNUT_HP, WALLNUT_ATTACK, WALLNUT_DEFENSE, WALLNUT_ATTACK_SPEED, "WallNut", BROWN, WALLNUT_NEED_SUN_NUMBER, WALLNUT_COOLDOWN_TIME, -1, 1){

}

WallNut::~WallNut(){

}

Repeator::Repeator(int tx, int ty):Plant(tx, ty, OBJ_TYPE_REPEATOR, REPEATOR_HP, REPEATOR_ATTACK, REPEATOR_DEFENSE, REPEATOR_ATTACK_SPEED, "Repeator", GREEN, REPEATOR_NEED_SUN_NUMBER, REPEATOR_COOLDOWN_TIME, OBJ_TYPE_PEABULLET, 1){

}

Repeator::~Repeator(){

}


Bullet::Bullet(int tx, int ty, int ttype, int tspeed, int tattack, int teffection, std::string tname, int tcolor):Object(tx, ty, ttype){
	speed = tspeed;
	attack = tattack;
	effection = teffection;
	name = tname;
	color = tcolor;
}

Bullet::~Bullet(){

}

void Bullet::update(){
	x += speed;
	if(x+name.length() >= SSW || x < 0){
		isDead = 1;
	}
}

void Bullet::draw(){
	drawText(x, y, name, WHITE, color);
}

void Bullet::interactive(Zombie* z){
	if(isDead){
		return;
	}
	if(z->getY()+1 == getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX()){
		z->defend(attack);
		if(effection == BULLET_EFF_NORMAL){

		}
		else if(effection == BULLET_EFF_SLOW){
			z->slowDown();
		}
		isDead = 1;
	}
}

int Bullet::getW(){
	return name.length();
}

PeaBullet::PeaBullet(int tx, int ty):Bullet(tx, ty, OBJ_TYPE_PEABULLET, PEABULLET_SPEED, PEABULLET_ATTACK, BULLET_EFF_NORMAL, "PeaBullet", LIGHTGREEN){

}

PeaBullet::~PeaBullet(){

}


SnowBullet::SnowBullet(int tx, int ty):Bullet(tx, ty, OBJ_TYPE_SNOWBULLET, SNOWBULLET_SPEED, SNOWBULLET_ATTACK, BULLET_EFF_SLOW, "SnowBullet", LIGHTBLUE){

}

SnowBullet::~SnowBullet(){

}

MelonBullet::MelonBullet(int tx, int ty):Bullet(tx, ty, OBJ_TYPE_MELONBULLET, MELONBULLET_SPEED, MELONBULLET_ATTACK, BULLET_EFF_NORMAL, "MelonBullet", LIGHTGREEN){
	isBreak = 0;
}

MelonBullet::~MelonBullet(){

}

void MelonBullet::update(){
	x += speed;
	if(isBreak){	
		for(int i = 0; i < zombies.size(); ++i){
			if(getX()-MELON_DIS <= zombies[i]->getX() + zombies[i]->getW() && getX()+getW()+MELON_DIS >= zombies[i]->getX()){
				zombies[i]->defend(attack*0.75);
			}
		}
	}
	if(x+name.length() >= SSW || x < 0 || isBreak){
		isDead = 1;
	}
}

void MelonBullet::interactive(Zombie* z){
	if(z->getY()+1 == getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX()){
		z->defend(attack);
		isBreak = 1;
	}
	else if(!isBreak && (z->getY()+1-BLOCKW == getY() || z->getY()+1+BLOCKW == getY()) && (getX()-MELON_DIS <= z->getX()+z->getW() && getX()+getW()+MELON_DIS >= z->getX())){
		int flag = 1;
		for(int i = 0; i < zombies.size(); ++i){
			if(zombies[i] == z){
				flag = 0;
			}
		}
		if(flag){
			zombies.push_back(z);
		}
	}
	else if(isBreak && (z->getY()+1 == getY() || z->getY()+1-BLOCKW == getY() || z->getY()+1+BLOCKW == getY()) && (getX()-MELON_DIS <= z->getX()+z->getW() && getX()+getW()+MELON_DIS >= z->getX())){
		int flag = 1;
		for(int i = 0; i < zombies.size(); ++i){
			if(zombies[i] == z){
				flag = 0;
			}
		}
		if(flag){
			zombies.push_back(z);
		}
	}
}

SnowMelonBullet::SnowMelonBullet(int tx, int ty):Bullet(tx, ty, OBJ_TYPE_SNOWMELONBULLET, SNOWMELONBULLET_SPEED, SNOWMELONBULLET_ATTACK, BULLET_EFF_SLOW, "SnowMelonBullet", LIGHTBLUE){
	isBreak = 0;
}

SnowMelonBullet::~SnowMelonBullet(){

}

void SnowMelonBullet::update(){
	x += speed;
	if(isBreak){	
		for(int i = 0; i < zombies.size(); ++i){
			if(getX()-MELON_DIS <= zombies[i]->getX() + zombies[i]->getW() && getX()+getW()+MELON_DIS >= zombies[i]->getX()){
				zombies[i]->defend(attack*0.75);
				zombies[i]->slowDown();
			}
		}
	}
	if(x+name.length() >= SSW || x < 0 || isBreak){
		isDead = 1;
	}
}

void SnowMelonBullet::interactive(Zombie* z){
	if(z->getY()+1 == getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX()){
		z->defend(attack);
		z->slowDown();
		isBreak = 1;
	}
	else if(!isBreak && (z->getY()+1-BLOCKW == getY() || z->getY()+1+BLOCKW == getY()) && (getX()-MELON_DIS <= z->getX()+z->getW() && getX()+getW()+MELON_DIS >= z->getX())){
		int flag = 1;
		for(int i = 0; i < zombies.size(); ++i){
			if(zombies[i] == z){
				flag = 0;
			}
		}
		if(flag){
			zombies.push_back(z);
		}
	}
	else if(isBreak && (z->getY()+1 == getY() || z->getY()+1-BLOCKW == getY() || z->getY()+1+BLOCKW == getY()) && (getX()-MELON_DIS <= z->getX()+z->getW() && getX()+getW()+MELON_DIS >= z->getX())){
		int flag = 1;
		for(int i = 0; i < zombies.size(); ++i){
			if(zombies[i] == z){
				flag = 0;
			}
		}
		if(flag){
			zombies.push_back(z);
		}
	}
}
