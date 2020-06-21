#include "backGround.h"
#include "WelcomeScene.h"

bool backGroundLayer::init()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

    //���ؿ�ʼ����
    auto returnWelcome = Button::create("normal_close.png");
    returnWelcome->setPosition(Vec2(winSize.width * 0.93, winSize.height * 0.08));
    returnWelcome->addTouchEventListener(CC_CALLBACK_1(backGroundLayer::menuCallBack, this));
    this->addChild(returnWelcome);

    //����Ԥ����
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("bgm.mp3");

    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3",true);

    is_paused = false;

    //��������
    auto startBtn = Button::create("start.png");
    startBtn->setScale(0.08);
    startBtn->setPosition(Vec2(winSize.width * 0.05, winSize.height * 0.92));
    startBtn->addTouchEventListener(CC_CALLBACK_1(backGroundLayer::music_Start, this));
    this->addChild(startBtn);

    //��ͣ����
    auto pauseBtn = Button::create("pause.png");
    pauseBtn->setScale(0.08);
    pauseBtn->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.92));
    pauseBtn->addTouchEventListener(CC_CALLBACK_1(backGroundLayer::music_Pause, this));
    this->addChild(pauseBtn);

    //ֹͣ����
    auto stopBtn = Button::create("stop.png");
    stopBtn->setScale(0.08);
    stopBtn->setPosition(Vec2(winSize.width * 0.17, winSize.height * 0.92));
    stopBtn->addTouchEventListener(CC_CALLBACK_1(backGroundLayer::music_Stop, this));
    this->addChild(stopBtn);

    //��������
    auto bgmVolume = Slider::create();
    bgmVolume->loadBarTexture("slider");
    bgmVolume->loadSlidBallTextures("circle.png");
    bgmVolume->setScale(0.60);
    bgmVolume->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.85));
    bgmVolume->setPercent(100);
    bgmVolume->addEventListener(CC_CALLBACK_2(backGroundLayer::music_Volume, this));
    this->addChild(bgmVolume);
	return true;
}

//��������
void backGroundLayer::music_Start(Ref* pSender)
{
    if (is_paused)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    else
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3");
}

//��ͣ����
void backGroundLayer::music_Pause(Ref* pSender)
{
    is_paused = true;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//ֹͣ����
void backGroundLayer::music_Stop(Ref* pSender)
{
    is_paused = false;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//��������
void backGroundLayer::music_Volume(cocos2d::Ref* pSender, Slider::EventType type)
{
    auto temp = (Slider*)pSender;
    if (temp->getPercent() != 100)
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(temp->getPercent() / 100.0f);
}

//���ؿ�ʼ����
void backGroundLayer::menuCallBack(Ref* pSender)
{
    Director::getInstance()->replaceScene(Welcome::createScene());
}