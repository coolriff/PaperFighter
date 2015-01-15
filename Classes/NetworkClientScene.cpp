#include "NetworkClientScene.h"


NetworkClientScene::NetworkClientScene()
{
	musicControl = new MusicControl();
}

NetworkClientScene::~NetworkClientScene()
{

}

bool NetworkClientScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	musicControl->playMenuMusic();

	initClient();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bk = Sprite::create("e.jpg");
	bk->setPosition(Point::ZERO);
	bk->setAnchorPoint(Point::ZERO);
	this->addChild(bk);

	auto label = Label::createWithTTF("No Server found", "fonts/Abduction.ttf", 24);
	label->setPosition(Point(origin.x + visibleSize.width/2, 300));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
	label->setTag(3);

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

Scene * NetworkClientScene::createScene()
{
	auto scene = Scene::create();
	auto layer = NetworkClientScene::create();
	scene->addChild(layer);
	return scene;
}

void NetworkClientScene::update(float dt)
{
	if (!isConnected)
	{
		this->schedule(schedule_selector(NetworkClientScene::clientUpdate),0.01);
	}
	else
	{
		net_StartGame();
	}
}

void NetworkClientScene::initClient()
{
	closeNetworking();
	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(CLIENT_PORT,0);
	peer->Startup(MAX_CLIENTS,&sd, 1);
	isServer = false;
	isClient = true;
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
}

void NetworkClientScene::closeNetworking()
{
	if(peer == NULL)
	{
		return;
	}
	peer->Shutdown(500, 0, LOW_PRIORITY);
	RakNet::RakPeerInterface::DestroyInstance(peer);
	peer = NULL;
}

void NetworkClientScene::clientUpdate(float dt)
{
	RakNet::Packet* packet;

	if (peer->Ping("127.0.0.1",SERVER_PORT,false) && isConnected == false)
	{
		for (packet = peer->Receive();packet;peer->DeallocatePacket(packet),packet=peer->Receive())
		{
			if (packet->data[0] == ID_UNCONNECTED_PONG)
			{
				peer->Connect(packet->systemAddress.ToString(false),SERVER_PORT,0,0);
			}
			else if (packet->data[0] == ID_GAME_MESSAGE_1)
			{
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				isServer = false;
				isConnected = true;
				isP1 = true;
				otherPlayer = packet->systemAddress;

				this->removeChildByTag(3);

				Size visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();

				auto label = Label::createWithTTF("connected with Server! ", "fonts/Abduction.ttf", 24);
				label->setPosition(Point(origin.x + visibleSize.width/2, 300));
				label->setColor(Color3B::BLACK);
				this->addChild(label, 1);
				label->setTag(5);
			}
		}
	}
}

void NetworkClientScene::net_StartGame()
{
	if (isConnected)
	{
		RakNet::Packet* packet;
		for (packet = peer->Receive();packet;peer->DeallocatePacket(packet),packet=peer->Receive())
		{
			if (packet->data[0] == ID_GAME_START)
			{
				currentLevel = 1;
				musicControl->stopAllMusic();
				musicControl->playClickMenu();
				//Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_2::createScene()));
				Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_1::createScene()));
			}
		}
	}
}