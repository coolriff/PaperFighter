#ifndef NEXT_LEVEL_SCENE
#define NEXT_LEVEL_SCENE

#include "cocos2d.h"
#include "MusicControl.h"
#include "GameScene_2.h"

using namespace cocos2d;

class NextLevelScene:Layer{
public:
	bool init();
	CREATE_FUNC(NextLevelScene);
	static Scene * createScene();
	void update(float dt);

	int frameCounter;

	MusicControl* musicControl;
};

#endif