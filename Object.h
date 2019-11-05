#pragma once

#include "common.h"
#include "ScreenDraw.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

#define ARG_DEFENSE 50

#define OBJ_TYPE_NULL             0
#define OBJ_TYPE_SUN              1
#define OBJ_TYPE_CHARACTOR        2
#define OBJ_TYPE_PLANT            3
#define OBJ_TYPE_ZOMBIE           4
#define OBJ_TYPE_BULLET           5

#define OBJ_TYPE_SUNFLOWER        13
#define OBJ_TYPE_PEASHOOTER       23
#define OBJ_TYPE_SNOWPEA          33
#define OBJ_TYPE_MELONPULT        43
#define OBJ_TYPE_SNOWMELON        53
#define OBJ_TYPE_SPIKEWEED        63
#define OBJ_TYPE_GARLIC           73

#define OBJ_TYPE_NORMALZOMBIE     14
#define OBJ_TYPE_CONEZOMBIE       24
#define OBJ_TYPE_BUCKETZOMBIE     34
#define OBJ_TYPE_NEWSZOMBIE       44

#define OBJ_TYPE_PEABULLET        15
#define OBJ_TYPE_SNOWBULLET       25
#define OBJ_TYPE_MELONBULLET      35
#define OBJ_TYPE_SNOWMELONBULLET  45


typedef struct ObjectSignal{
	int type;
	int data;
	int x, y;
	ObjectSignal(int t, int d): type(t), data(d){
		x = 0;
		y = 0;
	}
}ObjectSignal;

#define OBJ_SIGNAL_NULL           0
#define OBJ_SIGNAL_GEN_SUN        1
#define OBJ_SIGNAL_GEN_BULLET     2

class Object{
	private:
	public:
		int x, y;
		int type;
		bool isDead;

		Object(int tx, int ty, int ttype);
		virtual void update();
		virtual void draw();
		virtual ObjectSignal getSignal();
		virtual bool getIsDead();
		virtual int getY();
		virtual int getX();
		virtual void die();
};

class Suns:public Object{
	private:
	public:
		int num;
		
		Suns(int tx, int ty);
		~Suns();
		void update();
		void draw();
		void addSun();
		int clearSun();
};

#define ATTACK_MAX_DIS 0

class Charactor:public Object{
	private:
	public:
		std::string name;
		int color;

		bool attackStart;
		int attackCount;
			
		int hp;
		int attack;
		int defense;
		int attackSpeed;
		bool isAttack;
		
		Charactor(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor);
		~Charactor();
		void update();
		void draw();
		virtual void defend(int objAttack);
		virtual bool getIsAttack();
		virtual int getAttack();
		virtual void startAttack();
		virtual void stopAttack();
		virtual bool getIsAttackStart();
		virtual int getW();
};

class Plant;

class Zombie:public Charactor{
	private:
	public:
		int speed;
		int speedCopy;
		int moveCount;
		int slowDownCount;
		
		Zombie(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor, int tspeed);
		~Zombie();
		void update();
		void draw();
		virtual void interactive(Plant* p);
		virtual void slowDown();
		virtual void randomUpDonw();
};

#define NORMALZOMBIE_HP             180
#define NORMALZOMBIE_ATTACK         50
#define NORMALZOMBIE_DEFENSE        50
#define NORMALZOMBIE_ATTACK_SPEED   30
#define NORMALZOMBIE_SPEED          10 

class NormalZombie:public Zombie{
	private:
	public:
		NormalZombie(int tx, int ty);
		~NormalZombie();
};

#define CONEZOMBIE_HP             220
#define CONEZOMBIE_ATTACK         50
#define CONEZOMBIE_DEFENSE        70
#define CONEZOMBIE_ATTACK_SPEED   30
#define CONEZOMBIE_SPEED          10 

class ConeZombie:public Zombie{
	private:
	public:
		ConeZombie(int tx, int ty);
		~ConeZombie();
};

#define BUCKETZOMBIE_HP             280
#define BUCKETZOMBIE_ATTACK         50
#define BUCKETZOMBIE_DEFENSE        120
#define BUCKETZOMBIE_ATTACK_SPEED   30
#define BUCKETZOMBIE_SPEED          10 

class BucketZombie:public Zombie{
	private:
	public:
		BucketZombie(int tx, int ty);
		~BucketZombie();
};

#define NEWSZOMBIE_HP             220
#define NEWSZOMBIE_ATTACK         50
#define NEWSZOMBIE_DEFENSE        50
#define NEWSZOMBIE_ATTACK_SPEED   30
#define NEWSZOMBIE_SPEED          10 

class NewsZombie:public Zombie{
	private:
	public:
		NewsZombie(int tx, int ty);
		~NewsZombie();
		void update();
};

typedef struct PlantInfo{
	std::string name;
	int color;
	int type;
	int needSunNumber;
	int coolDownTime;
	int count;
	PlantInfo(std::string tname, int tcolor, int ttype, int tneedSunNumber, int tcoolDownTime):name(tname), color(tcolor), type(ttype) ,needSunNumber(tneedSunNumber), coolDownTime(tcoolDownTime), count(0){}
} PlantInfo;


class Plant:public Charactor{
	private:
	public:
		bool hasBullet;
		bool isZombieValid;
		int needSunNumber;
		int coolDownTime;
		
		Plant(int tx, int ty, int ttype, int thp, int tattack, int tdefense, int tattackSpeed, std::string tname, int tcolor, int tneedSunNumber, int tcoolDownTime);
		~Plant();
		virtual bool getIsZombieValid();
		virtual bool getHasBullet();
		virtual int getNeedSunNumber();
		virtual void interactive(Zombie* z);
		virtual PlantInfo getInfo();
};

#define SUNFLOWER_GEN_SUN_SPEED   70
#define SUNFLOWER_HP              150
#define SUNFLOWER_ATTACK          0
#define SUNFLOWER_DEFENSE         70
#define SUNFLOWER_ATTACK_SPEED     -1
#define SUNFLOWER_NEED_SUN_NUMBER 4
#define SUNFLOWER_COOLDOWN_TIME   70

class SunFlower:public Plant{
	private:
	public:
		int genSunCount;
		int genSunSpeed;


		SunFlower(int tx, int ty);
		~SunFlower();
		ObjectSignal getSignal();
		void update();
};

#define PEASHOOTER_HP              125
#define PEASHOOTER_ATTACK          0
#define PEASHOOTER_DEFENSE         50
#define PEASHOOTER_ATTACK_SPEED    20
#define PEASHOOTER_NEED_SUN_NUMBER 8 
#define PEASHOOTER_COOLDOWN_TIME   100

class PeaShooter:public Plant{
	private:
	public:
		PeaShooter(int tx, int ty);
		~PeaShooter();
		ObjectSignal getSignal();
};

#define SNOWPEA_HP              150
#define SNOWPEA_ATTACK          0
#define SNOWPEA_DEFENSE         50
#define SNOWPEA_ATTACK_SPEED    20
#define SNOWPEA_NEED_SUN_NUMBER 10
#define SNOWPEA_COOLDOWN_TIME   140

class SnowPea:public Plant{
	private:
	public:
		SnowPea(int tx, int ty);
		~SnowPea();
		ObjectSignal getSignal();
};

#define MELONPULT_HP              125
#define MELONPULT_ATTACK          0
#define MELONPULT_DEFENSE         50
#define MELONPULT_ATTACK_SPEED    30
#define MELONPULT_NEED_SUN_NUMBER 12
#define MELONPULT_COOLDOWN_TIME   160

class MelonPult:public Plant{
	private:
	public:
		MelonPult(int tx, int ty);
		~MelonPult();
		ObjectSignal getSignal();
};

#define SNOWMELON_HP              125
#define SNOWMELON_ATTACK          0
#define SNOWMELON_DEFENSE         50
#define SNOWMELON_ATTACK_SPEED    30
#define SNOWMELON_NEED_SUN_NUMBER 16
#define SNOWMELON_COOLDOWN_TIME   200

class SnowMelon:public Plant{
	private:
	public:
		SnowMelon(int tx, int ty);
		~SnowMelon();
		ObjectSignal getSignal();
};

#define SPIKEWEED_HP              150
#define SPIKEWEED_ATTACK          25
#define SPIKEWEED_DEFENSE         50
#define SPIKEWEED_ATTACK_SPEED    10
#define SPIKEWEED_NEED_SUN_NUMBER 12
#define SPIKEWEED_COOLDOWN_TIME   140
#define SPIKEWEED_HP_DESC         5

class SpikeWeed:public Plant{
	private:
	public:
		SpikeWeed(int tx, int ty);
		~SpikeWeed();
		void interactive(Zombie* z);
};

#define GARLIC_HP              100
#define GARLIC_ATTACK          0
#define GARLIC_DEFENSE         50
#define GARLIC_ATTACK_SPEED    10
#define GARLIC_NEED_SUN_NUMBER 10
#define GARLIC_COOLDOWN_TIME   120

class Garlic:public Plant{
	private:
	public:
		Garlic(int tx, int ty);
		~Garlic();
		void interactive(Zombie* z);
};

#define BULLET_EFF_NORMAL      0
#define BULLET_EFF_SLOW        1

class Bullet:public Object{
	private:
	public:
		int speed;
		int attack;
		int effection;
		std::string name;
		int color;
		
		Bullet(int tx, int ty, int ttype, int tspeed, int tattack, int teffection, std::string tname, int tcolor);
		~Bullet();
		void update();
		void draw();
		virtual void interactive(Zombie* z);
		virtual int getW();
};

#define PEABULLET_SPEED      2
#define PEABULLET_ATTACK     50

class PeaBullet:public Bullet{
	private:
	public:
		PeaBullet(int tx, int ty);
		~PeaBullet();
};

#define SNOWBULLET_SPEED      2
#define SNOWBULLET_ATTACK     50

class SnowBullet:public Bullet{
	private:
	public:
		SnowBullet(int tx, int ty);
		~SnowBullet();
};

#define MELON_DIS              4

#define MELONBULLET_SPEED      1
#define MELONBULLET_ATTACK     55

class MelonBullet:public Bullet{
	private:
	public:
		bool isBreak;
		std::vector<Zombie*> zombies;

		MelonBullet(int tx, int ty);
		~MelonBullet();
		void update();
		void interactive(Zombie* z);
};

#define SNOWMELONBULLET_SPEED      1
#define SNOWMELONBULLET_ATTACK     55

class SnowMelonBullet:public Bullet{
	private:
	public:
		bool isBreak;
		std::vector<Zombie*> zombies;

		SnowMelonBullet(int tx, int ty);
		~SnowMelonBullet();
		void update();
		void interactive(Zombie* z);
};


