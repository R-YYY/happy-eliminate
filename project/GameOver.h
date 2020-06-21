#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::ui;

USING_NS_CC;
class Gameover : public cocos2d::Scene
{
private:

public:
    virtual bool init();

    static cocos2d::Scene* createScene();

    void menuStartGame(cocos2d::Ref* pSender);

    void menuCallBack(cocos2d::Ref* pSender);

    CREATE_FUNC(Gameover);
};