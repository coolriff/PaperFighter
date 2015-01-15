#include "Bullet.h"

Bullet::Bullet()
{
	musicControl = new MusicControl();
}

Bullet::~Bullet()
{
}

void Bullet::moveBullet(float t)
{
	for (int i = 0; i < allBullet.size(); i++)
	{
		auto nowbullet = allBullet.at(i);
		nowbullet->setPositionY(nowbullet->getPositionY()+3);
		if (nowbullet->getPositionY() > Director::getInstance()->getWinSize().height)
		{
			nowbullet->removeFromParent();
			allBullet.eraseObject(nowbullet);
			i--;
		}
	}
}

//sync bullet will slow down the connection speed a lot! 
// void Bullet::newBulletForP1(float t)
// {
// 	if (isConnected)
// 	{
// 		if (isServer)
// 		{
// 			Sprite * bullet1 = Sprite::create("bullet.png");
// 			bullet1->setPosition(Plane::plane->getChildByTag(101)->getPositionX(),Plane::plane->getChildByTag(101)->getPositionY()+75);
// 			bullet1->setName("p1");
// 			this->addChild(bullet1);
// 			this->allBullet.pushBack(bullet1);
// 			musicControl->playShootMusic();
// 
// 			RakNet::BitStream BsOut;
// 			BsOut.Write((RakNet::MessageID)ID_GAME_BULLET);
// 			BsOut.WriteVector<float>(bullet1->getPositionX(),bullet1->getPositionY(),0);
// 			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 		}
// 		else
// 		{
// 			Sprite * bullet1 = Sprite::create("bullet.png");
// 			bullet1->setPosition(p1BulletX,p1BulletY);
// 			bullet1->setName("p1");
// 			this->addChild(bullet1);
// 			this->allBullet.pushBack(bullet1);
// 			musicControl->playShootMusic();
// 		}
// 	}
// 	else
// 	{
// 		Sprite * bullet1 = Sprite::create("bullet.png");
// 		bullet1->setPosition(Plane::plane->getChildByTag(101)->getPositionX(),Plane::plane->getChildByTag(101)->getPositionY()+75);
// 		bullet1->setName("p1");
// 		this->addChild(bullet1);
// 		this->allBullet.pushBack(bullet1);
// 		musicControl->playShootMusic();
// 	}
// }
// 
// void Bullet::newBulletForP2(float t)
// {
// 	if (isConnected)
// 	{
// 		if (isClient)
// 		{
// 			Sprite * bullet2 = Sprite::create("bullet.png");
// 			bullet2->setPosition(Plane::plane->getChildByTag(102)->getPositionX(),Plane::plane->getChildByTag(102)->getPositionY()+75);
// 			bullet2->setName("p2");
// 			this->addChild(bullet2);
// 			this->allBullet.pushBack(bullet2);
// 			musicControl->playShootMusic();
// 
// 			RakNet::BitStream BsOut;
// 			BsOut.Write((RakNet::MessageID)ID_GAME_BULLET);
// 			BsOut.WriteVector<float>(bullet2->getPositionX(),bullet2->getPositionY(),0);
// 			peer->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,otherPlayer,false);
// 		}
// 		else
// 		{
// 			Sprite * bullet2 = Sprite::create("bullet.png");
// 			bullet2->setPosition(p2BulletX,p2BulletY);
// 			bullet2->setName("p2");
// 			this->addChild(bullet2);
// 			this->allBullet.pushBack(bullet2);
// 			musicControl->playShootMusic();
// 		}
// 	}
// 	else
// 	{
// 		Sprite * bullet2 = Sprite::create("bullet.png");
// 		bullet2->setPosition(Plane::plane->getChildByTag(102)->getPositionX(),Plane::plane->getChildByTag(102)->getPositionY()+75);
// 		bullet2->setName("p2");
// 		this->addChild(bullet2);
// 		this->allBullet.pushBack(bullet2);
// 		musicControl->playShootMusic();
// 	}
// }

void Bullet::newBulletForP1(float t)
{
	Sprite * bullet1 = Sprite::create("bullet.png");
	bullet1->setPosition(Plane::plane->getChildByTag(101)->getPositionX(),Plane::plane->getChildByTag(101)->getPositionY()+75);
	bullet1->setName("p1");
	this->addChild(bullet1);
	this->allBullet.pushBack(bullet1);
	musicControl->playShootMusic();
}

void Bullet::newBulletForP2(float t)
{
	Sprite * bullet2 = Sprite::create("bullet.png");
	bullet2->setPosition(Plane::plane->getChildByTag(102)->getPositionX(),Plane::plane->getChildByTag(102)->getPositionY()+75);
	bullet2->setName("p2");
	this->addChild(bullet2);
	this->allBullet.pushBack(bullet2);
	musicControl->playShootMusic();
}

