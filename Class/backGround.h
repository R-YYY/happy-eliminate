#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"

using namespace CocosDenshion;
using namespace cocos2d::ui;

USING_NS_CC;
class backGroundLayer : public cocos2d::CCLayer
{
private:
	bool is_paused;

public:
	virtual bool init();

    //播放音乐
    void music_Start(cocos2d::Ref* pSender);

    //暂停音乐
    void music_Pause(cocos2d::Ref* pSender);

    //停止音乐
    void music_Stop(cocos2d::Ref* pSender);

    //控制音量
    void music_Volume(cocos2d::Ref* pSender, Slider::EventType type);

    //返回开始界面
    void menuCallBack(cocos2d::Ref* pSender);

	CREATE_FUNC(backGroundLayer);
};