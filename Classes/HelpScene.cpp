#include "HelpScene.h"
#include "HelloWorldScene.h"

bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	musicControl->playMenuMusic();

	auto bk = Sprite::create("e.jpg");
	bk->setPosition(Point::ZERO);
	bk->setAnchorPoint(Point::ZERO);
	this->addChild(bk);

	EventListenerTouchOneByOne * touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [](Touch * touch, Event * event){
		return true;
	};
	touch->onTouchMoved = [](Touch * touch, Event * event){
		Director::getInstance()->replaceScene(TransitionFade::create(1,HelloWorld::createScene()));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch,this);

	return true;
}

Scene * HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

