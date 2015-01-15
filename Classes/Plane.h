#ifndef PLANE_H
#define PLANE_H

#include "cocos2d.h"
#include "RaknetGlobals.h"

using namespace cocos2d;

class Plane : public Layer
{
public:
	Plane();
	static Plane* create();
	bool init();
	static Plane* plane;
	bool isAlive;
	int score;
	Vector<Sprite*> allPlane;
};

#endif