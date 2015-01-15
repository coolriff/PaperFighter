#include "GameScene_2.h"


GameScene_2::GameScene_2()
{
	plane = new Plane();
	bullet = new Bullet();
	enemy = new Enemy();
	musicControl = new MusicControl();
	nextLevel = false;
}

GameScene_2::~GameScene_2()
{

}

bool GameScene_2::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Sleep(1000);

	musicControl->playLevelMusic2();

	//background 1
	auto bg1 = Sprite::create("b21.jpg");
	bg1->setAnchorPoint(Point::ZERO);
	bg1->setPosition(Point::ZERO);
	this->addChild(bg1);
	bg1->setTag(11);

	//background 2
	auto bg2 = Sprite::create("b22.jpg");
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

		Label* tempLabel1 = (Label*)this->getChildByTag(121);
		tempLabel1->setString(String::createWithFormat("%d",p1Score)->_string);

		auto labScore2 = Label::create("0","fonts/Abduction.ttf",20);
		labScore2->setPosition(Point(650,580));
		labScore2->setColor(Color3B::BLACK);
		this->addChild(labScore2,1);
		labScore2->setTag(122);

		Label* tempLabel2 = (Label*)this->getChildByTag(122);
		tempLabel2->setString(String::createWithFormat("%d",p2Score)->_string);
	}
	else
	{
		auto labScore = Label::create("0","fonts/Abduction.ttf",20);
		labScore->setPosition(Point(400,580));
		labScore->setColor(Color3B::BLACK);
		this->addChild(labScore,1);
		labScore->setTag(121);

		Label* tempLabel1 = (Label*)this->getChildByTag(121);
		tempLabel1->setString(String::createWithFormat("%d",p1Score)->_string);
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
	event->onTouchBegan = CC_CALLBACK_2(GameScene_2::onTouchBegan,this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene_2::onTouchMoved,this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene_2::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event,this);

	//update background
	this->schedule(schedule_selector(GameScene_2::moveBackground),0.01);

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
	//this->enemy->schedule(schedule_selector(Enemy::newEnemy),1);
	this->enemy->schedule(schedule_selector(Enemy::moveEnemy),0.01);
	this->scheduleUpdate();
	return true;
}

Scene * GameScene_2::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene_2::create();
	scene->addChild(layer);
	return scene;
}

void GameScene_2::moveBackground(float t)
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

bool GameScene_2::onTouchBegan(Touch *touch, Event *unused_event)
{
	px = touch->getLocation().x;
	py = touch->getLocation().y;
	return true;
}

void GameScene_2::onTouchMoved(Touch *touch, Event *unused_event)
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

void GameScene_2::onTouchEnded(Touch *touch, Event *unused_event)
{
	//have to have this empty this function
}

void GameScene_2::sendPosInfor(float t)
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

void GameScene_2::sendScore(float t)
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

void GameScene_2::sendBulletInfor(float t)
{
	//did in bullet class
}

void GameScene_2::sendEmemyPosInfor(float t)
{
	if (isConnected)
	{
		if (isServer)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_TIME_STAMP);
			BsOut.Write<int>(p1Time);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
		if (isClient)
		{
			RakNet::BitStream BsOut;
			BsOut.Write((RakNet::MessageID)ID_GAME_TIME_STAMP);
			BsOut.Write<int>(p2Time);
			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
		}
	}
}

void GameScene_2::dealReceivedPosInfor(float t)
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
					if (mp1Pos)
					{
						mp1Pos->setPosition(t[0],t[1]);
					}

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
				else if (packet->data[0] == ID_GAME_TIME_STAMP)
				{
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read<int>(timeStamp);
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
					if (mp1Pos)
					{
						mp1Pos->setPosition(t[0],t[1]);
					}
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
				else if (packet->data[0] == ID_GAME_TIME_STAMP)
				{
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read<int>(timeStamp);
				}
			}
		}
	}
}

void GameScene_2::update(float dt)
{
	if (isConnected)
	{
		if (isServer)
		{
			p1Time ++;
			this->schedule(schedule_selector(GameScene_2::sendPosInfor),0.01);
			this->schedule(schedule_selector(GameScene_2::sendScore),0.01);
			this->schedule(schedule_selector(GameScene_2::sendBulletInfor),0.01); //TIME STAMP
			this->schedule(schedule_selector(GameScene_2::dealReceivedPosInfor),0.01);

			if (p1Time > timeStamp && timeStamp != 0)
			{
				p1Time = timeStamp;
			}
		}

		if (isClient)
		{
			p2Time ++;
			this->schedule(schedule_selector(GameScene_2::sendPosInfor),0.01);
			this->schedule(schedule_selector(GameScene_2::sendScore),0.01);
			this->schedule(schedule_selector(GameScene_2::sendBulletInfor),0.01);  //TIME STAMP
			this->schedule(schedule_selector(GameScene_2::dealReceivedPosInfor),0.01);

			if (p2Time > timeStamp && timeStamp != 0)
			{
				p2Time = timeStamp;
			}
		}
	}
	else
	{
		p1Time++;
	}

	formation();
	collisionDetection();
	PlayerCollisionDetection();
	
// 
// 	if (p1Score == 300 || p2Score == 300)
// 	{
// 		nextLevel = true;
// 	}
// 
// 	if (nextLevel && (p1Score == 300 || p2Score == 300))
// 	{
// 		nextLevelChecker();
// 	}
}

void GameScene_2::collisionDetection()
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


void GameScene_2::nextLevelChecker()
{
}

// void GameScene_2::PlayerCollisionDetection()
// {
// 	if (isConnected)
// 	{
// 		if (isServer)
// 		{
// 			for (int i = 0; i < enemy->allEnemy.size(); i++)
// 			{
// 				auto currentEnemy = enemy->allEnemy.at(i);
// 				Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);
// 
// 				auto currentPlane_1 = this->plane->getChildByTag(101);
// 				Rect ep_1(currentPlane_1->getPositionX(),currentPlane_1->getPositionY(),36,98);
// 
// 				if (er.intersectsRect(ep_1))
// 				{
// 					RakNet::BitStream bsOut;
// 					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_SERVER);
// 					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 
// 					musicControl->stopAllMusic();
// 					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
// 					this->unscheduleAllCallbacks();
// 					this->unscheduleAllSelectors();
// 					this->unscheduleUpdate();
// 				}
// 
// 				if (isP2)
// 				{
// 					auto currentPlane_2 = this->plane->getChildByTag(102);
// 					Rect ep_2(currentPlane_2->getPositionX(),currentPlane_2->getPositionY(),54,98);
// 
// 					if (er.intersectsRect(ep_2))
// 					{
// 						RakNet::BitStream bsOut;
// 						bsOut.Write((RakNet::MessageID)ID_GAME_OVER_CLIENT);
// 						peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 
// 						this->bullet->unschedule(schedule_selector(Bullet::newBulletForP2));
// 						this->plane->removeChildByTag(102);
// 
// 						currentEnemy->removeFromParent();
// 						enemy->allEnemy.eraseObject(currentEnemy);
// 					}
// 				}
// 
// 				break;
// 			}
// 		}
// 
// 		if (isClient)
// 		{
// 			for (int i = 0; i < enemy->allEnemy.size(); i++)
// 			{
// 				auto currentEnemy = enemy->allEnemy.at(i);
// 				Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);
// 
// 				if (isP1)
// 				{
// 					auto currentPlane_1 = this->plane->getChildByTag(101);
// 					Rect ep_1(currentPlane_1->getPositionX(),currentPlane_1->getPositionY(),36,98);
// 
// 					if (er.intersectsRect(ep_1))
// 					{
// 						RakNet::BitStream bsOut;
// 						bsOut.Write((RakNet::MessageID)ID_GAME_OVER_CLIENT);
// 						peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 
// 						this->bullet->unschedule(schedule_selector(Bullet::newBulletForP2));
// 						this->plane->removeChildByTag(101);
// 
// 						currentEnemy->removeFromParent();
// 						enemy->allEnemy.eraseObject(currentEnemy);
// 					}
// 				}
// 
// 				auto currentPlane_2 = this->plane->getChildByTag(102);
// 				Rect ep_2(currentPlane_2->getPositionX(),currentPlane_2->getPositionY(),54,98);
// 
// 				if (er.intersectsRect(ep_2))
// 				{
// 					RakNet::BitStream bsOut;
// 					bsOut.Write((RakNet::MessageID)ID_GAME_OVER_SERVER);
// 					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 
// 					musicControl->stopAllMusic();
// 					Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
// 					this->unscheduleAllCallbacks();
// 					this->unscheduleAllSelectors();
// 					this->unscheduleUpdate();
// 				}
// 
// 				break;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < enemy->allEnemy.size(); i++)
// 		{
// 			auto currentEnemy = enemy->allEnemy.at(i);
// 			Rect er(currentEnemy->getPositionX(),currentEnemy->getPositionY(),50,50);
// 
// 			auto currentPlane = this->plane->getChildByTag(101);
// 			Rect ep(currentPlane->getPositionX(),currentPlane->getPositionY(),36,98);
// 
// 			if (er.intersectsRect(ep))
// 			{
// 				musicControl->stopAllMusic();
// 				Director::getInstance()->replaceScene(TransitionFade::create(1,GameOverScene::createScene()));
// 				this->unscheduleAllCallbacks();
// 				this->unscheduleAllSelectors();
// 				this->unscheduleUpdate();
// 				break;
// 			}
// 		}
// 
// 	}
// }


void GameScene_2::PlayerCollisionDetection()
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

void GameScene_2::formation()
{
	if (p1Time == 120 || p2Time == 120)
	{
		this->enemy->newEnemy1(100,825);
		this->enemy->newEnemy2(200,825); 
		this->enemy->newEnemy1(300,825); 
		this->enemy->newEnemy2(400,825); 
		this->enemy->newEnemy1(500,825); 
		this->enemy->newEnemy2(600,825);
		this->enemy->newEnemy1(700,825); 
	}

	if (p1Time == 150 || p2Time == 150)
	{
		this->enemy->newEnemy2(100,825);
		this->enemy->newEnemy1(200,825); 
		this->enemy->newEnemy2(300,825); 
		this->enemy->newEnemy1(400,825); 
		this->enemy->newEnemy2(500,825); 
		this->enemy->newEnemy1(600,825);
		this->enemy->newEnemy2(700,825); 
	}

	if (p1Time == 180 || p2Time == 180)
	{
		this->enemy->newEnemy1(100,825);
		this->enemy->newEnemy2(200,825); 
		this->enemy->newEnemy1(300,825); 
		this->enemy->newEnemy2(400,825); 
		this->enemy->newEnemy1(500,825); 
		this->enemy->newEnemy2(600,825);
		this->enemy->newEnemy1(700,825); 
	}

	if (p1Time == 270 || p2Time == 270)
	{
		this->enemy->unschedule(schedule_selector(Enemy::moveEnemy));
	}

	if (isConnected)
	{
		if (p1Time > 420 && p1Time < 720 || p2Time > 420 && p2Time < 720)
		{
			auto currentPlane_1 = this->plane->getChildByTag(101);
			auto currentPlane_2 = this->plane->getChildByTag(102);
			auto currentEnemy_1 = this->enemy->getChildByName("e1");
			auto currentEnemy_2 = this->enemy->getChildByName("e2");

			if (currentPlane_1)
			{
				currentEnemy_1->runAction(MoveTo::create(300,currentPlane_1->getPosition()));
			}

			if (currentPlane_2)
			{
				currentEnemy_2->runAction(MoveTo::create(300,currentPlane_2->getPosition()));
			}
		}

	}
	else
	{
		if (p1Time > 420 && p1Time < 720)
		{
			auto currentPlane_1 = this->plane->getChildByTag(101);
			auto currentEnemy_1 = this->enemy->getChildByName("e1");
			auto currentEnemy_2 = this->enemy->getChildByName("e2");

			currentEnemy_1->runAction(MoveTo::create(300,currentPlane_1->getPosition()));
			currentEnemy_2->runAction(MoveTo::create(300,currentPlane_1->getPosition()));
		}
	}

	if (p1Time == 720 || p2Time == 720)
	{
		this->enemy->schedule(schedule_selector(Enemy::newEnemy),1);
		this->enemy->schedule(schedule_selector(Enemy::moveEnemy),0.01);
	}
}

