
#ifndef __GAME_SCENE_H__
#define __GAEM_SCENE_H__

#include "cocos2d.h"
#include "SpriteType.h"

#define MAX_ROW 8
#define MAX_COL 8

class Game : public cocos2d::Scene
{
private:

    //��ͼ
    SpriteType* myMap[MAX_ROW][MAX_COL];

    //�Ƿ���Ҫ���ȱ
    bool isNeedFilled = false;

    //�����Ƿ��ڶ�
    bool isAtion = true;

    //��Ϸ����
    int gameScore = 0;

    //��Ϸ����
    int gameSteps = 0;

    //������ʼ��
    SpriteType* staTouch;

    //����������
    SpriteType* endTouch;

public:

    static cocos2d::Scene* createScene();

    virtual bool init();

    //���ص�ͼ
    void initMap();

    //��������
    void createSprite(int r,int c);

    //ʵʱ����
    void update(float time);

    //���ȱ����
    void FillSprite();

    //���Ҳ�����
    void checkAndRemove();

    //���������
    void checkRow(SpriteType* spr, std::list<SpriteType*>* lineList);

    //���������
    void checkCol(SpriteType* spr, std::list<SpriteType*>* lineList);

    //�����Ҫ�����ķ���
    void markRemove(SpriteType* spr);

    //����������̲������ⷽ��ķ���
    void markNextToSpecial(SpriteType* spr);

    //��������
    void remove();

    //���ⷽ�������
    void specialRemove(SpriteType* spr);

    //��ȡ����λ�õķ���
    SpriteType* getTouchedSprite(Vec2& position);

    //������ʼ
    bool onTouchBegan(Touch* touch,Event* event);

    //������
    bool onTouchMoved(Touch* touch, Event* event);

    //��������
    bool onTouchEnded(Touch* touch, Event* event); 

    //�������鲢���м��
    void tryToSwapSprite();

    //�������ķ��麬���ⷽ�飬���ܲ�������Ч��
    bool swapSpecialSperite();

    CREATE_FUNC(Game);
};

#endif // __GAME_SCENE_H__