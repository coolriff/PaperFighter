#ifndef ABOUT_SCENE
#define ABOUT_SCENE

#include "cocos2d.h"
#include "MusicControl.h"

using namespace cocos2d;

class AboutSecne:Layer{
public:
	bool init();
	CREATE_FUNC(AboutSecne);
	static Scene * createScene();

	MusicControl* musicControl;
};

#endif