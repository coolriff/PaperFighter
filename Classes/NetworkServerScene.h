#ifndef NETWORK_SERVER_SCENE
#define NETWORK_SERVER_SCENE

#include "cocos2d.h"
#include "GameScene_1.h"
#include "RaknetGlobals.h"
#include "HelloWorldScene.h"
#include "MusicControl.h"

using namespace cocos2d;

class NetworkServerScene:Layer{
public:

	NetworkServerScene();
	~NetworkServerScene();

	bool init();
	CREATE_FUNC(NetworkServerScene);
	static Scene * createScene();

	MusicControl* musicControl;

	void update(float dt);
	void net_StartGame();
	void sendPosInfor(float t);
	void dealReceivedPosInfor(float t);
	void serverUpdate(float dt);
	void closeNetworking();
	void initServer();
	void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif