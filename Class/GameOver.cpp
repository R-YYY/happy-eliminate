
#include "Gameover.h"
#include "GameScene.h"
#include "WelcomeScene.h"

USING_NS_CC;

Scene* Gameover::createScene()
{
    return Gameover::create();
}

bool Gameover::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //加载背景
    Sprite* background1 = Sprite::create("gameover.jpg");
    background1->setContentSize(winSize);
    background1->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(background1);

    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("fail.mp3", false);

    //开始游戏
    auto startGame = Button::create("restart.jpg");
    startGame->setPosition(Vec2(winSize.width * 0.25, winSize.height * 0.20));
    startGame->addTouchEventListener(CC_CALLBACK_1(Gameover::menuStartGame, this));
    this->addChild(startGame);

    //返回
    auto returnWelcome = Button::create("back.jpg");
    returnWelcome->setPosition(Vec2(winSize.width * 0.75, winSize.height * 0.20));
    returnWelcome->addTouchEventListener(CC_CALLBACK_1(Gameover::menuCallBack, this));
    this->addChild(returnWelcome);



    return true;
}

void Gameover::menuStartGame(Ref* pSender)
{
    Director::getInstance()->replaceScene(Game::createScene());
}

//返回开始界面
void Gameover::menuCallBack(Ref* pSender)
{
    Director::getInstance()->replaceScene(Welcome::createScene());
}