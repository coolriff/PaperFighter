#include "GameScene_1.h"


GameScene_1::GameScene_1()
{
	plane = new Plane();
	bullet = new Bullet();
	enemy = new Enemy();
	musicControl = new MusicControl();
	nextLevel = false;
}

GameScene_1::~GameScene_1()
{

}

bool GameScene_1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	musicControl->playLevelMusic1();

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
	event->onTouchBegan = CC_CALLBACK_2(GameScene_1::onTouchBegan,this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene_1::onTouchMoved,this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene_1::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event,this);

	//update background
	this->schedule(schedule_selector(GameScene_1::moveBackground),0.01);

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

	Sleep(1000);
	return true;
}

Scene * GameScene_1::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene_1::create();
	scene->addChild(layer);
	return scene;
}

void GameScene_1::moveBackground(float t)
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

bool GameScene_1::onTouchBegan(Touch *touch, Event *unused_event)
{
	px = touch->getLocation().x;
	py = touch->getLocation().y;
	return true;
}

void GameScene_1::onTouchMoved(Touch *touch, Event *unused_event)
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

void GameScene_1::onTouchEnded(Touch *touch, Event *unused_event)
{
	//have to have this empty this function
}

void GameScene_1::sendPosInfor(float t)
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

void GameScene_1::sendScore(float t)
{
	if (isConnected)
	{
		if (isServer)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_SCORE);
			BsOut.Write<int>(p1Score);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
		if (isClient)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_SCORE);
			BsOut.Write<int>(p2Score);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
	}
}

void GameScene_1::sendBulletInfor(float t)
{
	//did in bullet class
}

void GameScene_1::sendEmemyPosInfor(float t)
{

}

void GameScene_1::dealReceivedPosInfor(float t)
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
					isP2 = false;
					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP2));
					this->plane->removeChildByTag(102);
				}
				// other player just lost
				else if (packet->data[0] == ID_GAME_NEXT_LEVEL)
				{
					currentLevel = 2;
					musicControl->stopAllMusic();
					musicControl->playClickMenu();
					Director::getInstance()->replaceScene(TransitionFade::create(1,NextLevelScene::createScene()));
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
				else if(packet->data[0] == ID_GAME_SCORE)
				{
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read<int>(p2Score);
				}
				else if(packet->data[0] == ID_GAME_BULLET)
				{
// 					float temp;
// 					RakNet::BitStream bsIn(packet->data,packet->length,false);
// 					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
// 					bsIn.ReadVector<float>(p2BulletX,p2BulletY,temp);
					return;
				}
				else if (packet->data[0] == ID_GAME_OVER_CLIENT)
				{
					isP2 = false;
					return;
				}

				else if (packet->data[0] == ID_GAME_OVER_SERVER)
				{
					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();

					
// 					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP2));
// 					this->plane->removeChildByTag(102);
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
					isP1 = false;
					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP1));
					this->plane->removeChildByTag(101);
				}
				// other player just lost
				else if (packet->data[0] == ID_GAME_NEXT_LEVEL)
				{
					currentLevel = 2;
					musicControl->stopAllMusic();
					musicControl->playClickMenu();
					Director::getInstance()->replaceScene(TransitionFade::create(1,NextLevelScene::createScene()));
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
				else if(packet->data[0] == ID_GAME_SCORE)
				{
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read<int>(p1Score);
				}
				else if(packet->data[0] == ID_GAME_BULLET)
				{
// 					float temp;
// 					RakNet::BitStream bsIn(packet->data,packet->length,false);
// 					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
// 					bsIn.ReadVector<float>(p1BulletX,p1BulletY,temp);
					return;
				}
				else if (packet->data[0] == ID_GAME_OVER_CLIENT)
				{
					isP1 = false;
					return;
				}
				else if (packet->data[0] == ID_GAME_OVER_SERVER)
				{

					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();

					

// 					this->bullet->unschedule(schedule_selector(Bullet::newBulletForP1));
// 					this->plane->removeChildByTag(101);


				}
			}
		}
	}
}

void GameScene_1::update(float dt)
{
	if (isConnected)
	{
		if (isServer)
		{
			this->schedule(schedule_selector(GameScene_1::sendPosInfor),0.01);
			this->schedule(schedule_selector(GameScene_1::sendScore),0.01);
			this->schedule(schedule_selector(GameScene_1::dealReceivedPosInfor),0.01);
		}

		if (isClient)
		{
			this->schedule(schedule_selector(GameScene_1::sendPosInfor),0.01);
			this->schedule(schedule_selector(GameScene_1::sendScore),0.01);
			this->schedule(schedule_selector(GameScene_1::dealReceivedPosInfor),0.01);
		}
	}

 	collisionDetection();
	PlayerCollisionDetection();


	if (p1Score == 500 || p2Score == 500)
	{
		nextLevel = true;
	}

	if (nextLevel && (p1Score == 500 || p2Score == 500))
	{
		nextLevelChecker();
	}
}

void GameScene_1::collisionDetection()
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
					//musicControl->playExplosion2();
					Label* tempLabel1 = (Label*)this->getChildByTag(121);
					p1Score += 100;
					tempLabel1->setString(String::createWithFormat("%d",p1Score)->_string);
				}

				if (currentBullet->getName() == "p2")
				{
					//musicControl->playExplosion2();
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

void GameScene_1::nextLevelChecker()
{
	for (int i = 0; i < enemy->allEnemy.size(); i++)
	{
		auto currentEnemy = enemy->allEnemy.at(i);
		currentEnemy->removeFromParent();
		enemy->allEnemy.eraseObject(currentEnemy);
	}

	for (int j = 0; j < bullet->allBullet.size(); j++)
	{
		auto currentBullet = bullet->allBullet.at(j);
		currentBullet->removeFromParent();
		bullet->allBullet.eraseObject(currentBullet);
	}

	nextLevel = false;

	if (isConnected)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_NEXT_LEVEL);
		peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
	}

	musicControl->stopAllMusic();
	Director::getInstance()->replaceScene(TransitionFade::create(1,NextLevelScene::createScene()));
	this->unscheduleAllCallbacks();
	this->unscheduleAllSelectors();
	this->unscheduleUpdate();
}
		
void GameScene_1::PlayerCollisionDetection()
{
	if (isConnected)
	{
		if (isServer)
		{
			for (int i = 0; i < enemy->allEnemy.size(); i++)
			{
				auto currentEnemy = enemy->allEnemy.at(i);
				Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);

				auto currentPlane_1 = this->plane->getChildByTag(101);
				Rect ep_1(currentPlane_1->getPositionX(),currentPlane_1->getPositionY(),36,98);

				auto currentPlane_2 = this->plane->getChildByTag(102);
				Rect ep_2(currentPlane_2->getPositionX(),currentPlane_2->getPositionY(),54,98);

				if (er.intersectsRect(ep_1))
				{
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_SERVER);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);

					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();
				}

				if (er.intersectsRect(ep_2))
				{
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_CLIENT);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);

					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();
				}

				break;
			}
		}

		if (isClient)
		{
			for (int i = 0; i < enemy->allEnemy.size(); i++)
			{
				auto currentEnemy = enemy->allEnemy.at(i);
				Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);

				auto currentPlane_1 = this->plane->getChildByTag(101);
				Rect ep_1(currentPlane_1->getPositionX(),currentPlane_1->getPositionY(),36,98);

				auto currentPlane_2 = this->plane->getChildByTag(102);
				Rect ep_2(currentPlane_2->getPositionX(),currentPlane_2->getPositionY(),54,98);

				if (er.intersectsRect(ep_1))
				{
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_CLIENT);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);

					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();
				}

				if (er.intersectsRect(ep_2))
				{
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_SERVER);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);

					musicControl->stopAllMusic();
					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
					this->unscheduleAllCallbacks();
					this->unscheduleAllSelectors();
					this->unscheduleUpdate();
				}

				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < enemy->allEnemy.size(); i++)
		{
			auto currentEnemy = enemy->allEnemy.at(i);
			Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);

			auto currentPlane = this->plane->getChildByTag(101);
			Rect ep(currentPlane->getPositionX(),currentPlane->getPositionY(),36,98);

			if (er.intersectsRect(ep))
			{
				musicControl->stopAllMusic();
				Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
				this->unscheduleAllCallbacks();
				this->unscheduleAllSelectors();
				this->unscheduleUpdate();
				break;
			}
		}

	}
}