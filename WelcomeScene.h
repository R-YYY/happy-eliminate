
#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::ui;

class Welcome : public cocos2d::Scene
{
private:
    int is_paused;
    

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menu_StartGame(cocos2d::Ref* pSender);
    void menu_CloseGame(cocos2d::Ref* pSender);

    void music_Start(cocos2d::Ref* pSender);
    void music_Pause(cocos2d::Ref* pSender);
    void music_Stop(cocos2d::Ref* pSender);
    void music_Volume(cocos2d::Ref* pSender, Slider::EventType type);

    // implement the "static create()" method manually
    CREATE_FUNC(Welcome);
};

#endif // __WELCOME_SCENE_H__
