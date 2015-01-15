#ifndef NETWORK_CLIENT_SCENE
#define NETWORK_CLIENT_SCENE

#include "cocos2d.h"
#include "GameScene_1.h"
#include "RaknetGlobals.h"
#include "HelloWorldScene.h"
#include "MusicControl.h"

using namespace cocos2d;

class NetworkClientScene:Layer{
public:
	NetworkClientScene();
	~NetworkClientScene();

	bool init();
	CREATE_FUNC(NetworkClientScene);
	static Scene * createScene();

	MusicControl* musicControl;

	void initClient();
	void closeNetworking();
	void update(float dt);
	void net_StartGame();
	void clientUpdate(float dt);
};

#endif