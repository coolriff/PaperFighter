#include "HelloWorldScene.h"

USING_NS_CC;


HelloWorld::HelloWorld()
{
	musicControl = nullptr;
}

HelloWorld::~HelloWorld()
{

}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	musicControl->stopAllMusic();
	musicControl->playClickMenu();
	musicControl->playMenuMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	closeItem->setTag(10);

	MenuItemFont::setFontName( "fonts/Abduction.ttf" );
	MenuItemFont::setFontSize( 36 );

	auto gameItem = MenuItemFont::create("StartGame", CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	auto serverItem = MenuItemFont::create("Server", CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	auto clientItem = MenuItemFont::create("Client", CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	auto helpItem = MenuItemFont::create("Help", CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	auto aboutItem = MenuItemFont::create("About", CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));

	gameItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2, 300));
	serverItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2, 250));
	clientItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2, 200));
	helpItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2, 150));
	aboutItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2, 100));

	gameItem->setColor(Color3B::BLACK);
	serverItem->setColor(Color3B::BLACK);
	clientItem->setColor(Color3B::BLACK);
	helpItem->setColor(Color3B::BLACK);
	aboutItem->setColor(Color3B::BLACK);

	gameItem->setTag(11);
	serverItem->setTag(12);
	clientItem->setTag(13);
	helpItem->setTag(14);
	aboutItem->setTag(15);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, gameItem, serverItem, clientItem, helpItem, aboutItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("e.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	MenuItem * tempItem = (MenuItem *)pSender;
	switch(tempItem->getTag()) 
	{
		case 10:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
		break;

		case 11://game
			currentLevel = 1;
			musicControl->stopAllMusic();
			musicControl->playClickMenu();
			//Director::getInstance()->replaceScene(GameScene_1::createScene());
			Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_1::createScene()));
			//Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene_2::createScene()));
			break;
		case 12://server
			musicControl->stopAllMusic();
			musicControl->playClickMenu();
			Director::getInstance()->replaceScene(TransitionFade::create(1,NetworkServerScene::createScene()));
			break;
		case 13://client
			musicControl->stopAllMusic();
			musicControl->playClickMenu();
			Director::getInstance()->replaceScene(TransitionFade::create(1,NetworkClientScene::createScene()));
			break;
		case 14://help
			musicControl->stopAllMusic();
			musicControl->playClickMenu();
			Director::getInstance()->replaceScene(TransitionFade::create(1,HelpScene::createScene()));
			break;
		case 15://about
			musicControl->stopAllMusic();
			musicControl->playClickMenu();
			Director::getInstance()->replaceScene(TransitionFade::create(1,AboutSecne::createScene()));
			break;
	}
}
