#ifndef GAME_SCENE
#define GAME_SCENE

#include "cocos2d.h"
#include "Plane.h"
#include "Bullet.h"
#include "Enemy.h"
#include "RaknetGlobals.h"

using namespace cocos2d;

class GameScene:Layer{
public:

	GameScene();
	~GameScene();

	bool init();
	CREATE_FUNC(GameScene);
	static Scene * createScene();
	void moveBackground(float t);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void collisionDetection();

	int px,py;

	void update(float dt);
	void sendPosInfor(float t);
	void dealReceivedPosInfor(float t);

	Plane* plane;
	Bullet* bullet;
	Enemy* enemy;
};

#endif