#ifndef BULLET_H
#define BULLET_H

#include "cocos2d.h"
#include "Plane.h"
#include "RaknetGlobals.h"
#include "MusicControl.h"

using namespace cocos2d;

class Bullet : public Layer
{
public:
	Bullet();
	~Bullet();
	CREATE_FUNC(Bullet);

	Vector<Sprite*> allBullet;
	void newBulletForP1(float t);
	void newBulletForP2(float t);
	void moveBullet(float t);

	bool isAlive;
	int score;

	MusicControl* musicControl;
};

#endif