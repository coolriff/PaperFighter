#include "GameOver.h"

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	musicControl->playGameOver();
	frameCounter = 0;


	auto bk = Sprite::create("end.jpg");
	bk->setPosition(Point::ZERO);
	bk->setAnchorPoint(Point::ZERO);
	this->addChild(bk);

	this->scheduleUpdate();

	return true;
}

void GameOverScene::update(float dt)
{
	frameCounter++;
	if (frameCounter == 300)
	{
		currentLevel = 2;
		musicControl->stopAllMusic();
		//peer->CloseConnection(otherPlayer,true);
		Director::getInstance()->pushScene(TransitionFade::create(1,HelloWorld::createScene()));
	}
}


Scene * GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

