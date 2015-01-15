#include "NextLevelScene.h"


bool NextLevelScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	musicControl->playNextLevel();
	frameCounter = 0;


	auto bk = Sprite::create("8.jpg");
	bk->setPosition(Point::ZERO);
	bk->setAnchorPoint(Point::ZERO);
	this->addChild(bk);

	this->scheduleUpdate();

	return true;
}

void NextLevelScene::update(float dt)
{
	frameCounter++;
	if (frameCounter == 120)
	{
		currentLevel = 2;
		musicControl->stopAllMusic();
		Director::getInstance()->pushScene(TransitionFade::create(1,GameScene_2::createScene()));
	}
}


Scene * NextLevelScene::createScene()
{
	auto scene = Scene::create();
	auto layer = NextLevelScene::create();
	scene->addChild(layer);
	return scene;
}

