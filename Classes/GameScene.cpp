#include "GameScene.h"


GameScene::GameScene()
{
	plane = nullptr;
	bullet = nullptr;
	enemy = nullptr;
}

GameScene::~GameScene()
{

}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//background 1
	auto bg1 = Sprite::create("b11.jpg");
	bg1->setAnchorPoint(Point::ZERO);
	bg1->setPosition(Point::ZERO);
	this->addChild(bg1);
	bg1->setTag(11);

	//background 2
	auto bg2 = Sprite::create("b12.jpg");
	bg2->setAnchorPoint(Point::ZERO);
	bg2->setPosition(Point::ZERO);
	bg2->setPositionY(bg1->getPositionY() + 1422);
	this->addChild(bg2);
	bg2->setTag(12);

	if (isConnected)
	{
		auto labScore1 = Label::create("0","fonts/Abduction.ttf",20);
		labScore1->setPosition(Point(150,580));
		labScore1->setColor(Color3B::BLACK);
		this->addChild(labScore1,1);
		labScore1->setTag(121);

		auto labScore2 = Label::create("0","fonts/Abduction.ttf",20);
		labScore2->setPosition(Point(650,580));
		labScore2->setColor(Color3B::BLACK);
		this->addChild(labScore2,1);
		labScore2->setTag(122);
	}
	else
	{
		auto labScore = Label::create("0","fonts/Abduction.ttf",20);
		labScore->setPosition(Point(400,580));
		labScore->setColor(Color3B::BLACK);
		this->addChild(labScore,1);
		labScore->setTag(121);
	}


	this->plane = Plane::create();
	this->addChild(plane);

	this->bullet = new Bullet();
	this->addChild(bullet);

	this->enemy = new Enemy();
	this->addChild(enemy);

	//move plane
	EventListenerTouchOneByOne* event = EventListenerTouchOneByOne::create();
	event->setSwallowTouches(true);
	event->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved,this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event,this);

	//update background
	this->schedule(schedule_selector(GameScene::moveBackground),0.01);

	if (isConnected)
	{
		this->bullet->schedule(schedule_selector(Bullet::newBulletForP1),0.5);
		this->bullet->schedule(schedule_selector(Bullet::newBulletForP2),0.5);
	}
	else
	{
		this->bullet->schedule(schedule_selector(Bullet::newBulletForP1),0.5);
	}
	this->bullet->schedule(schedule_selector(Bullet::moveBullet),0.01);
	this->enemy->schedule(schedule_selector(Enemy::newEnemy),1);
	this->enemy->schedule(schedule_selector(Enemy::moveEnemy),0.01);
	this->scheduleUpdate();
	return true;
}

Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void GameScene::moveBackground(float t)
{
	auto tbg1 = this->getChildByTag(11);
	auto tbg2 = this->getChildByTag(12);
	tbg1->setPositionY(tbg1->getPositionY()-1);
	if (tbg1->getPositionY() < -1422)
	{
		tbg1->setPositionY(0);
	}
	tbg2->setPositionY(tbg1->getPositionY() + 1422);
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	px = touch->getLocation().x;
	py = touch->getLocation().y;
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (isClient)
	{
		int mx = (touch->getLocation().x - px);
		int my = (touch->getLocation().y - py);
		auto mp1 = this->plane->getChildByTag(102);
		mp1->runAction(MoveBy::create(0,Point(mx,my)));
		px = touch->getLocation().x;
		py = touch->getLocation().y;
	}
	else
	{
		int mx = (touch->getLocation().x - px);
		int my = (touch->getLocation().y - py);
		auto mp1 = this->plane->getChildByTag(101);
		mp1->runAction(MoveBy::create(0,Point(mx,my)));
		px = touch->getLocation().x;
		py = touch->getLocation().y;
	}
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	//have to have this empty this function
}

void GameScene::sendPosInfor(float t)
{
	if (isConnected)
	{
		if (isServer)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_NEW_POINTS);
			auto mp1Pos = this->plane->getChildByTag(101);
			float t[3];
			t[0] = mp1Pos->getPositionX();
			t[1] = mp1Pos->getPositionY();
			t[2] = 0.0f;
			BsOut.WriteVector<float>(t[0],t[1],t[2]);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
		if (isClient)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_NEW_POINTS);
			auto mp1Pos = this->plane->getChildByTag(102);
			float t[3];
			t[0] = mp1Pos->getPositionX();
			t[1] = mp1Pos->getPositionY();
			t[2] = 0.0f;
			BsOut.WriteVector<float>(t[0],t[1],t[2]);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
	}
}

void GameScene::dealReceivedPosInfor(float t)
{
	if (isConnected)
	{
		if (isServer)
		{
			RakNet::Packet* packet;
			for (packet = peer->Receive();packet;peer->DeallocatePacket(packet),packet=peer->Receive())
			{
				// player disconnected
				if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION || 
					packet->data[0] == ID_CONNECTION_LOST)
				{
					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP2));
					this->plane->removeChildByTag(102);
				}
				// other player just lost
				else if (packet->data[0] == ID_GAME_PLAYER_LOST)
				{
					return;
				}
				else if (packet->data[0] == ID_GAME_NEW_POINTS)
				{
					float t[3];
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.ReadVector<float>(t[0],t[1],t[2]);
					auto mp1Pos = this->plane->getChildByTag(102);
					mp1Pos->setPosition(t[0],t[1]);
				}
				else if(packet->data[0] == ID_GAME_INIT_SWAP)
				{
					return;
				}
				else if(packet->data[0] == ID_GAME_SWAP_SCREENS)
				{
					return;
				}
			}
		}
		if (isClient)
		{
			RakNet::Packet* packet;
			for (packet = peer->Receive();packet;peer->DeallocatePacket(packet),packet=peer->Receive())
			{
				// player disconnected
				if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION || 
					packet->data[0] == ID_CONNECTION_LOST)
				{
					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP1));
					this->plane->removeChildByTag(101);
				}
				// other player just lost
				else if (packet->data[0] == ID_GAME_PLAYER_LOST)
				{
					return;
				}
				else if (packet->data[0] == ID_GAME_NEW_POINTS)
				{
					float t[3];
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.ReadVector<float>(t[0],t[1],t[2]);
					auto mp1Pos = this->plane->getChildByTag(101);
					mp1Pos->setPosition(t[0],t[1]);
				}
				else if(packet->data[0] == ID_GAME_INIT_SWAP)
				{
					return;
				}
				else if(packet->data[0] == ID_GAME_SWAP_SCREENS)
				{
					return;
				}
			}
		}
	}
}

void GameScene::update(float dt)
{
	if (isConnected)
	{
		if (isServer)
		{
			this->schedule(schedule_selector(GameScene::sendPosInfor),0.01);
			this->schedule(schedule_selector(GameScene::dealReceivedPosInfor),0.01);
		}

		if (isClient)
		{
			this->schedule(schedule_selector(GameScene::dealReceivedPosInfor),0.01);
			this->schedule(schedule_selector(GameScene::sendPosInfor),0.01);
		}
	}

	collisionDetection();
}

void GameScene::collisionDetection()
{
	for (int i = 0; i < enemy->allEnemy.size(); i++)
	{
		auto currentEnemy = enemy->allEnemy.at(i);
		Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);

		for (int j = 0; j < bullet->allBullet.size(); j++)
		{
			auto currentBullet = bullet->allBullet.at(j);
			Rect eb(currentBullet->getPositionX(),currentBullet->getPositionY(),30,30);

			if (er.intersectsRect(eb))
			{
				if (currentBullet->getName() == "p1")
				{
					Label* tempLabel1 = (Label*)this->getChildByTag(121);
					p1Score += 100;
					tempLabel1->setString(String::createWithFormat("%d",p1Score)->_string);
				}

				if (currentBullet->getName() == "p2")
				{
					Label* tempLabel2 = (Label*)this->getChildByTag(122);
					p2Score += 100;
					tempLabel2->setString(String::createWithFormat("%d",p2Score)->_string);
				}

				currentBullet->removeFromParent();
				bullet->allBullet.eraseObject(currentBullet);

				currentEnemy->removeFromParent();
				enemy->allEnemy.eraseObject(currentEnemy);
				i--;
				break;
			}
		}
	}
}
