
#include "WelcomeScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* Welcome::createScene()
{
    return Welcome::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in XxlScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Welcome::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    //size
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //ÓÎÏ·±³¾°
    Sprite* background1 = Sprite::create("newworld.jpg");
    background1->setContentSize(winSize);
    background1->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(background1);

    //¿ªÊ¼ÓÎÏ·
    auto startGame = Button::create("start_game.png");
    startGame->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.21));
    startGame->addTouchEventListener(CC_CALLBACK_1(Welcome::menu_StartGame,this));
    this->addChild(startGame);

    //Ô¤¼ÓÔØ
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("bgm.mp3");

    is_paused = 0;

    //²¥·ÅÒôÀÖ
    auto startBtn = Button::create("start.png");
    startBtn->setScale(0.08);
    startBtn->setPosition(Vec2(winSize.width* 0.05,winSize.height * 0.92));
    startBtn->addTouchEventListener(CC_CALLBACK_1(Welcome::music_Start, this));
    this->addChild(startBtn);

    //ÔÝÍ£ÒôÀÖ
    auto pauseBtn = Button::create("pause.png");
    pauseBtn->setScale(0.08);
    pauseBtn->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.92));
    pauseBtn->addTouchEventListener(CC_CALLBACK_1(Welcome::music_Pause, this));
    this->addChild(pauseBtn);

    //Í£Ö¹ÒôÀÖ
    auto stopBtn = Button::create("stop.png");
    stopBtn->setScale(0.08);
    stopBtn->setPosition(Vec2(winSize.width * 0.17, winSize.height * 0.92));
    stopBtn->addTouchEventListener(CC_CALLBACK_1(Welcome::music_Stop, this));
    this->addChild(stopBtn);

    //ÒôÁ¿¿ØÖÆ
    auto bgmVolume = Slider::create();
    bgmVolume->loadBarTexture("slider");
    bgmVolume->loadSlidBallTextures("circle.png");
    bgmVolume->setScale(0.60);
    bgmVolume->setPosition(Vec2(winSize.width * 0.11,winSize.height * 0.85));    
    bgmVolume->setPercent(100);
    bgmVolume->addEventListener(CC_CALLBACK_2(Welcome::music_Volume, this));
    this->addChild(bgmVolume);

    //ÍË³öÓÎÏ·
    auto closeGame = Button::create("normal_close.png");
    closeGame->setPosition(Vec2(winSize.width * 0.93, winSize.height * 0.08));
    closeGame->addTouchEventListener(CC_CALLBACK_1(Welcome::menu_CloseGame, this));
    this->addChild(closeGame);

    return true;
}


void Welcome::menu_StartGame(Ref* pSender)
{
    Director::getInstance()->replaceScene(Game::createScene());
}

void Welcome::menu_CloseGame(Ref* pSender)
{
    Director::getInstance()->end();
}

void Welcome::music_Start(Ref* pSender)
{
    if (is_paused)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    else 
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3");
}

void Welcome::music_Pause(Ref* pSender)
{
    is_paused = 1;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void Welcome::music_Stop(Ref* pSender)
{
    is_paused = 0;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void Welcome::music_Volume(cocos2d::Ref* pSender, Slider::EventType type)
{
    auto temp = (Slider*)pSender;
    if (temp->getPercent() != 100)
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(temp->getPercent()/100.0f);
}