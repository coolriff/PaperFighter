#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::newEnemy(float t)
{
	Sprite * enemy = nullptr;
	int num = rand()%10;
	if (num >= 5)
	{
		enemy = Sprite::create("e1.png");
	}
	else
	{
		enemy = Sprite::create("e2.png");
	}

	enemy->setPosition(Point(rand()%775 + 25, 825));
	this->addChild(enemy);
	this->allEnemy.pushBack(enemy);
}

void Enemy::newEnemy1(float x, float y)
{
	Sprite * enemy = Sprite::create("e1.png");
	enemy->setPosition(Point(x,y));
	enemy->setName("e1");
	this->addChild(enemy);
	this->allEnemy.pushBack(enemy);
}

void Enemy::newEnemy2(float x, float y)
{
	Sprite * enemy = Sprite::create("e2.png");
	enemy->setPosition(Point(x,y));
	enemy->setName("e2");
	this->addChild(enemy);
	this->allEnemy.pushBack(enemy);
}

void Enemy::moveEnemy(float t)
{
	for (int i = 0; i < allEnemy.size(); i++)
	{
		auto nowEnemy = allEnemy.at(i);
		nowEnemy->setPositionY(nowEnemy->getPositionY()-3);
		if (nowEnemy->getPositionY() < -100)
		{
			nowEnemy->removeFromParent();
			allEnemy.eraseObject(nowEnemy);
			i--;
		}
	}
}

