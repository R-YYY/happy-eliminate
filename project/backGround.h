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

    //��������
    void music_Start(cocos2d::Ref* pSender);

    //��ͣ����
    void music_Pause(cocos2d::Ref* pSender);

    //ֹͣ����
    void music_Stop(cocos2d::Ref* pSender);

    //��������
    void music_Volume(cocos2d::Ref* pSender, Slider::EventType type);

    //���ؿ�ʼ����
    void menuCallBack(cocos2d::Ref* pSender);

	CREATE_FUNC(backGroundLayer);
};