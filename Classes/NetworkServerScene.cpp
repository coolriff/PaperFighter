#include "NetworkServerScene.h"

NetworkServerScene::NetworkServerScene()
{
	musicControl = new MusicControl();
}

NetworkServerScene::~NetworkServerScene()
{

}

bool NetworkServerScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	musicControl->playMenuMusic();

	initServer();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bk = Sprite::create("e.jpg");
	bk->setPosition(Point::ZERO);
	bk->setAnchorPoint(Point::ZERO);
	this->addChild(bk);

	auto label = Label::createWithTTF("No Player Connected", "fonts/Abduction.ttf", 24);
	label->setPosition(Point(origin.x + visibleSize.width/2, 300));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
	label->setTag(1);

	EventListenerTouchOneByOne * touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [](Touch * touch, Event * event){
		return true;
	};
	touch->onTouchMoved = [](Touch * touch, Event * event){
		Director::getInstance()->replaceScene(TransitionFade::create(1,HelloWorld::createScene()));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch,this);

	this->scheduleUpdate();

	return true;
}


Scene * NetworkServerScene::createScene()
{
	auto scene = Scene::create();
	auto layer = NetworkServerScene::create();
	scene->addChild(layer);
	return scene;
}


void NetworkServerScene::update(float dt)
{
	if (isServer)
	{
		if (!isConnected)
		{
			this->schedule(schedule_selector(NetworkServerScene::serverUpdate),0.01);
		}
	}
}


void NetworkServerScene::net_StartGame()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_START);
	peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
}


void NetworkServerScene::serverUpdate(float dt)
{
	RakNet::Packet* packet;
	for (packet = peer->Receive();packet;peer->DeallocatePacket(packet),packet = peer->Receive())
	{
		if (packet->data[0] == ID_NEW_INCOMING_CONNECTION)
		{
			//reply
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
			peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			otherPlayer = packet->systemAddress;
		}
		else if (packet->data[0] == ID_GAME_MESSAGE_1)
		{
			otherPlayer = packet->systemAddress;
			isConnected = true;
			isP2 = true;

			this->removeChildByTag(1);

			Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			auto label = Label::createWithTTF("Player Connected", "fonts/Abduction.ttf", 24);
			label->setPosition(Point(origin.x + visibleSize.width/2, 300));
			label->setColor(Color3B::BLACK);
			this->addChild(label, 1);
			label->setTag(4);

			auto gameItem = MenuItemFont::create("StartGame", CC_CALLBACK_1(NetworkServerScene::menuCloseCallback,this));
			gameItem->setPosition(Point(origin.x + visibleSize.width/2, 250));
			gameItem->setColor(Color3B::BLACK);

			auto menu = Menu::create(gameItem, NULL);
			menu->setPosition(Vec2::ZERO);
			this->addChild(menu, 1);
		}
		else if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION || packet->data[0] == ID_CONNECTION_LOST)
		{
			isConnected = false;
		}
	}
}


void NetworkServerScene::closeNetworking()
{
	if(peer == NULL)
	{
		return;
	}
	peer->Shutdown(500, 0, LOW_PRIORITY);
	RakNet::RakPeerInterface::DestroyInstance(peer);
	peer = NULL;
}


void NetworkServerScene::initServer()
{
	closeNetworking();
	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(SERVER_PORT,0);
	peer->Startup(MAX_CLIENTS, &sd, 1);
	isServer = true;
	isClient = false;
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
}


void NetworkServerScene::menuCloseCallback(cocos2d::Ref* pSender)
{
	MenuItem * tempItem = (MenuItem *)pSender;
	net_StartGame();
	currentLevel = 1;
	musicControl->stopAllMusic();
	musicControl->playClickMenu();
	//Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_2::createScene()));
	Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_1::createScene()));
}