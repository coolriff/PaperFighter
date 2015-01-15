#include "Plane.h"

Plane* Plane::plane = nullptr;

Plane::Plane()
{
	isAlive = true;
	score = 0;
}

Plane* Plane::create()
{
	Plane *pRet = new Plane();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		plane = pRet;
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool Plane::init()
{
	bool tempB = false;

	if (isConnected)
	{
		auto p1 = Sprite::create();
		p1->setPosition(160, 100);
		this->addChild(p1);
		p1->setTag(101);
		p1->setName("plane_1");
		Vector<SpriteFrame*> allframeP1;
		for (int i = 4 ; i < 9 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframeP1.pushBack(sf);
		}
		for (int i = 7 ; i > -1 ; i--)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframeP1.pushBack(sf);
		}
		for (int i = 1 ; i < 5 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframeP1.pushBack(sf);
		}
		Animation * aniP1 = Animation::createWithSpriteFrames(allframeP1,0.1);
		p1->runAction(RepeatForever::create(Animate::create(aniP1)));

		allPlane.pushBack(p1);

		auto p2 = Sprite::create();
		p2->setPosition(640, 100);
		this->addChild(p2);
		p2->setTag(102);
		p2->setName("plane_2");
		Vector<SpriteFrame*> allframeP2;
		for (int i = 4 ; i < 9 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p3.png",Rect(i*54,0,54,98));
			allframeP2.pushBack(sf);
		}
		for (int i = 7 ; i > -1 ; i--)
		{
			SpriteFrame * sf = SpriteFrame::create("p3.png",Rect(i*54,0,54,98));
			allframeP2.pushBack(sf);
		}
		for (int i = 1 ; i < 5 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p3.png",Rect(i*54,0,54,98));
			allframeP2.pushBack(sf);
		}
		Animation * aniP2 = Animation::createWithSpriteFrames(allframeP2,0.1);
		p2->runAction(RepeatForever::create(Animate::create(aniP2)));

		allPlane.pushBack(p2);

		tempB = true;
	}
	else
	{
		auto p1 = Sprite::create();
		p1->setPosition(160, 100);
		this->addChild(p1);
		p1->setTag(101);
		Vector<SpriteFrame*> allframe;
		for (int i = 4 ; i < 9 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframe.pushBack(sf);
		}
		for (int i = 7 ; i > -1 ; i--)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframe.pushBack(sf);
		}
		for (int i = 1 ; i < 5 ; i++)
		{
			SpriteFrame * sf = SpriteFrame::create("p1.png",Rect(i*36,0,36,98));
			allframe.pushBack(sf);
		}
		Animation * ani = Animation::createWithSpriteFrames(allframe,0.1);
		p1->runAction(RepeatForever::create(Animate::create(ani)));

		allPlane.pushBack(p1);

		tempB = true;
	}
	return tempB;
}




