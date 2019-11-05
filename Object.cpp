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
	name = tname;
	color = tcolor;
}

Charactor::~Charactor(){

}

void Charactor::defend(int objAttack){
	if(attack == -1){
		hp = 0;
		isDead = 1;
	}
	int hurt = (int)((double)objAttack*(1-(double)defense/(defense+ARG_DEFENSE)));
	if(hurt <= 0){
		hurt = 1;
	}
	hp -= hurt;
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
	hpString << "HP: " << hp << " ";
	drawText(x, y, hpString.str(), WHITE, RED);
	drawText(x, y+1, name, WHITE, color);
}


bool Charactor::getIsAttackStart(){
	return attackStart;
}

int Charactor::getW(){
	return name.length()/2;
}

Zombie::Zombie(int tx, int ty, int ttype, int thp, int attack, int tdefense, int tattackSpeed, std::string tname, int color, int speed):Charactor(tx, ty, ttype, thp, tattack, tdefense, tattackSpeed, tname, tcolor){
	speed = tspeed;
	speedCopy = tspeed;
	moveCount = 0;
	slowDownCount = 0;
}

void Zombie::update(){
	Charactor::update();
	if(moveCount == speed-1){
		x--;
		moveCount = 0;
	}
	else if(slowDownCount != 0){
		slowDownCount--;
	}
	else if(!Charactor::getIsAttackStart()){
		moveCount++;
	}
}

void Zombie::interactive(Plant* p){
	if(p->getIsZombieValid() && 
			((speed < 0 && p->getX()+p->getW()+ATTACK_MAX_DIS >= getX()) || 
			 (speed > 0 && getX()+getW()+ATTACK_MAX_DIS >= p->getX()))){
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



	hp-=SPIKEWEED_HP_DESC;
			stopAttack();
		}
	}	
}

Garlic::Garlic(int tx, int ty):Plant(tx, ty, OBJ_TYPE_GARLIC, GARLIC_HP, GARLIC_ATTACK, GARLIC_DEFENSE, GARLIC_ATTACK_SPEED, "Garlic", LIGHTGRAY, GARLIC_NEED_SUN_NUMBER, GARLIC_COOLDOWN_TIME){

}

Garlic::~Garlic(){

}

void Garlic::interactive(Zombie* z){
	if(z->getY() == getY() && getX()+getW()+ATTACK_MAX_DIS >= z->getX() && z->getX()+z->getW()+ATTACK_MAX_DIS >= getX()){
		z->randomUpDonw();	
	}	
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
