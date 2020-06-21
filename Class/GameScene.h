
#ifndef __GAME_SCENE_H__
#define __GAEM_SCENE_H__

#include "cocos2d.h"
#include "SpriteType.h"

#define MAX_ROW 8
#define MAX_COL 8

class Game : public cocos2d::Scene
{
private:

    //地图
    SpriteType* myMap[MAX_ROW][MAX_COL];

    //是否需要填补空缺
    bool isNeedFilled = false;

    //方块是否在动
    bool isAtion = true;

    //游戏分数
    int gameScore = 0;

    //游戏步数
    int gameSteps = 0;

    //触摸起始点
    SpriteType* staTouch;

    //触摸结束点
    SpriteType* endTouch;

public:

    static cocos2d::Scene* createScene();

    virtual bool init();

    //加载地图
    void initMap();

    //产生方块
    void createSprite(int r,int c);

    //实时更新
    void update(float time);

    //填补空缺方块
    void FillSprite();

    //查找并消除
    void checkAndRemove();

    //检查行消除
    void checkRow(SpriteType* spr, std::list<SpriteType*>* lineList);

    //检查列消除
    void checkCol(SpriteType* spr, std::list<SpriteType*>* lineList);

    //标记需要消除的方块
    void markRemove(SpriteType* spr);

    //标记消除过程产生特殊方块的方块
    void markNextToSpecial(SpriteType* spr);

    //消除方块
    void remove();

    //特殊方块的消除
    void specialRemove(SpriteType* spr);

    //获取触摸位置的方块
    SpriteType* getTouchedSprite(Vec2& position);

    //触摸开始
    bool onTouchBegan(Touch* touch,Event* event);

    //触摸中
    bool onTouchMoved(Touch* touch, Event* event);

    //触摸结束
    bool onTouchEnded(Touch* touch, Event* event); 

    //交换方块并进行检查
    void tryToSwapSprite();

    //若交换的方块含特殊方块，可能产生特殊效果
    bool swapSpecialSperite();

    CREATE_FUNC(Game);
};

#endif // __GAME_SCENE_H__