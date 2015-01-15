#ifndef ENEMY_H
#define ENEMY_H

#include "cocos2d.h"
#include "RaknetGlobals.h"

using namespace cocos2d;

class Enemy : public Layer
{
public:
	Enemy();
	~Enemy();

	Vector<Sprite*> allEnemy;
	void newEnemy(float t);
	void newEnemy1(float x, float y);
	void newEnemy2(float x, float y);
	void moveEnemy(float t);

	bool isAlive;
	int score;
};

#endif