#ifndef GAME_0VER_SCENE
#define GAME_0VER_SCENE

#include "cocos2d.h"
#include "MusicControl.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

class GameOverScene:Layer{
public:
	bool init();
	CREATE_FUNC(GameOverScene);
	static Scene * createScene();
	void update(float dt);

	int frameCounter;

	MusicControl* musicControl;
};

#endif