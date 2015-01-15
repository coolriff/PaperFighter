#ifndef GAME_SCENE_2
#define GAME_SCENE_2

#include "cocos2d.h"
#include "Plane.h"
#include "Bullet.h"
#include "Enemy.h"
#include "RaknetGlobals.h"
#include "MusicControl.h"
#include "NextLevelScene.h"
#include "GameOver.h"

using namespace cocos2d;

class GameScene_2:Layer{
public:

	GameScene_2();
	~GameScene_2();

	bool init();
	CREATE_FUNC(GameScene_2);
	static Scene * createScene();
	void moveBackground(float t);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void collisionDetection();
	void PlayerCollisionDetection();

	int px,py;
	bool nextLevel;

	void update(float dt);
	void sendPosInfor(float t);
	void sendScore(float t);
	void sendBulletInfor(float t);
	void sendEmemyPosInfor(float t);
	void dealReceivedPosInfor(float t);
	void nextLevelChecker();
	void formation();

	Plane* plane;
	Bullet* bullet;
	Enemy* enemy;
	MusicControl* musicControl;
};

#endif