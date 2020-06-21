
#include "GameScene.h"
#include "SpriteType.h"
#include "backGround.h"
#include "GameOver.h"
#include "SecondScene.h"

using namespace std;

#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 640

USING_NS_CC;

Scene* Game::createScene()
{
    return Game::create();
}

bool Game::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //����
    Sprite* background1 = Sprite::create("onepiece.jpg");
    background1->setContentSize(winSize);
    background1->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(background1);

    //�������ּ����ְ�ť
    backGroundLayer* player_bg = backGroundLayer::create();
    addChild(player_bg);
    
    //Ŀ�����
    auto goal = LabelTTF::create("Goal: 10000", "Arial", 30);
    goal->setColor(ccc3(255, 0, 0));
    goal->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.75));
    this->addChild(goal);

    //��ǰ����
    auto scoreLabel = LabelTTF::create(CCString::createWithFormat("Score: %d", gameScore)->getCString(), "Arial", 30);
    scoreLabel->setColor(ccc3(255,0,0));
    scoreLabel->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.65));
    this->addChild(scoreLabel, 0, 1);

    //ʣ�ಽ��
    gameSteps = 25;
    auto stepsLabel = LabelTTF::create(CCString::createWithFormat("Steps: %d", gameSteps)->getCString(), "Arial", 30);
    stepsLabel->setColor(ccc3(255, 0, 0));
    stepsLabel->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.55));
    this->addChild(stepsLabel, 0, 2);

    
    srand(unsigned(time(0)));

    //���ص�ͼ
    initMap();

    //����
    schedule(schedule_selector(Game::update),0.5);

    //�����괥���¼�
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
}

//���ص�ͼ
void Game::initMap()
{
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            createSprite(i, j);
        }
    }
    isAtion = false;
}

//��������
void Game::createSprite(int r, int c)
{
    SpriteType* spr = SpriteType::setSprite(r, c);
    Point start = Point(WINDOWS_WIDTH * (0.25 + 0.07 * c), WINDOWS_HEIGHT * (0.15 + 0.105 * r) + WINDOWS_HEIGHT / 2);
    Point end = Point(WINDOWS_WIDTH * (0.25 + 0.07 * c), WINDOWS_HEIGHT * (0.14 + 0.105 * r));
    spr->setPosition(start);
    spr->runAction(MoveTo::create(0.5, end));
    this->addChild(spr);
    myMap[r][c] = spr;   
}

//ʵʱ����
void Game::update(float time)
{
    //������з����ڶ����򲻽�����������
    if (isAtion)
    {
        isAtion = false;
        for (int i = 0; i < MAX_ROW; i++)
        {
            for (int j = 0; j < MAX_COL; j++)
            {               
                if (myMap[i][j] && myMap[i][j]->getNumberOfRunningActions() > 0)
                {
                    isAtion = true;
                    break;
                }
            }
        }
    }

    if (!isAtion)
    {
        if (isNeedFilled)
        {
            FillSprite();
            isNeedFilled = false;
        }
        else checkAndRemove();
    }    

    if (!isNeedFilled && !isAtion)
    {
        if (gameScore >= 10000 && gameSteps > 0)//���Ŀ���һ���ʣ�ಽ��
        {
            vector<int>finalSpecial;//��������ʣ�ಽ����Ӧ�����ⷽ��λ��
            
            int i = 0;
            while (i < gameSteps)
            {
                bool same = false;
                int temp = rand() % (MAX_ROW * MAX_COL);

                for (int j = 0; j < i; j++)//�·���λ��֮ǰ�Ѵ���
                    if (temp == finalSpecial[j])
                        same = true;

                if (same)//����·���λ��֮ǰ�Ѵ��ڣ�Ѱ����һ��
                    continue;

                if (myMap[temp / 8][temp % 8] == NULL)
                    continue;

                i++;
                finalSpecial.push_back(temp);

                myMap[temp / 8][temp % 8]->setSpecial(rand() % 3 + 1);//�·������������
                myMap[temp / 8][temp % 8]->setIsNeedRemove(true);//��Ҫ���Ƴ�
                SpriteType* spr = myMap[temp / 8][temp % 8];

                //�����·���ͼƬ
                if (spr->getSpecialType() == 0)
                {
                    Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[16]);
                    spr->setTexture(texture);
                    spr->setType(4);
                }
                else
                {
                    Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[spr->getType() + spr->getSpecialType() * TOTAL_KIND]);
                    spr->setTexture(texture);
                }
                spr->setScale(0.7);
                spr->runAction(Blink::create(0.5, 2));
            }

            gameSteps = 0;
            CCLabelTTF* stepsLable = (CCLabelTTF*)this->getChildByTag(2);
            stepsLable->setString(CCString::createWithFormat("Steps: %d", gameSteps)->getCString());
        }
        else if (gameSteps <= 0 && gameScore >= 10000)//û�в������Ǵﵽ��Ŀ���������һ��
        {
            Director::getInstance()->replaceScene(Second::createScene());
        }
        else if (gameSteps <= 0 && gameScore < 10000)//û�в�������δ�ﵽĿ�������������Ϸ
        {
            Director::getInstance()->replaceScene(Gameover::createScene());
        }
    }

}

//���ȱ
void Game::FillSprite()
{
    isAtion = true;

    //������ÿ�б������ķ�����
    int* colEmptyNumber = new int[MAX_ROW]();

    SpriteType* spr = NULL;
    for (int j = 0; j < MAX_COL; j++)
    {
        int colEmpty = 0;
        for (int i = 0; i < MAX_ROW; i++)
        {
            spr = myMap[i][j];
            if (spr == NULL)
                colEmpty++;
            else if (colEmpty > 0)
            {
                int newRow = i - colEmpty;
                myMap[newRow][j] = spr;
                spr->setRow(newRow);
                myMap[i][j] = NULL;

                //���·��п�ȱ�ķ���������
                Point start = myMap[newRow][j]->getPosition();
                Point end = Point(WINDOWS_WIDTH * (0.25 + 0.07 * j), WINDOWS_HEIGHT * (0.14 + 0.105 * newRow));
                myMap[newRow][j]->stopAllActions();
                myMap[newRow][j]->runAction(MoveTo::create(0.5, end));
            }
        }
        colEmptyNumber[j] = colEmpty;
    }

    //��ÿ�е��Ϸ������µķ���
    for (int j = 0; j < MAX_COL; j++) 
    {
        for (int i = MAX_ROW - colEmptyNumber[j]; i < MAX_ROW; i++) 
        {
            createSprite(i, j);
        }
    }

    free(colEmptyNumber);
}

//���Ҳ�����
void Game::checkAndRemove()
{
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            SpriteType* spr = myMap[i][j];
            if(spr == NULL)
                continue;

            //����������������δ�������ⷽ��ķ����Լ����������ķ���
            if (spr->getIsNeedRemove() && spr->getIsNextToSpecial())
                continue;
            
            list<SpriteType*> rowList;
            checkRow(spr, &rowList);
            list<SpriteType*> colList;
            checkCol(spr, &colList);

            list<SpriteType*>::iterator count;
            
            if (rowList.size() >= 5 && colList.size() < 3)//һ�������������ͬ��
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//�²��������ⷽ�鱣��������
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(0); 
            }           
            else if(colList.size() >= 5 && rowList.size() < 3)//һ�������������ͬ��
            {
                for (count = colList.begin(); count != colList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//�²��������ⷽ�鱣��������
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(0);
            }
            else if (rowList.size() == 4 && colList.size() < 3)//һ���ĸ���ͬ��
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//�²��������ⷽ�鱣��������
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(1);
            }            
            else if (colList.size() == 4 && rowList.size() < 3)//һ���ĸ���ͬ��
            {
                for (count = colList.begin(); count != colList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//�²��������ⷽ�鱣��������
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(2);
            }           
            else if (colList.size() >= 3 && colList.size() < 5 && rowList.size() >= 3 && rowList.size() < 5)//   T \ L��
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                for (count = colList.begin(); count != colList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//�²��������ⷽ�鱣��������
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(3);
            }           
            else if (rowList.size() == 3 && colList.size() < 3)//һ������
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                    markRemove((SpriteType*)*count);
            }           
            else if (colList.size() == 3 && rowList.size() < 3)//һ������
            {
                for (count = colList.begin(); count != colList.end(); count++)
                    markRemove((SpriteType*)*count);
            }
        }
    }

    SpriteType* spr;
    //�²��������ⷽ�����ͼƬ
    for(int i = 0;i < MAX_ROW;i++)
    {
        for (int j = 0; j < MAX_COL; j++) 
        {
            spr = myMap[i][j];
            if (spr != NULL && spr->getIsSpecial())
            {
                if (spr->getSpecialType() == 0)
                {
                    Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[16]);
                    spr->setTexture(texture);
                    spr->setType(4);
                }
                else
                {
                    Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[spr->getType() + spr->getSpecialType() * TOTAL_KIND]);
                    spr->setTexture(texture);
                }
                spr->setScale(0.7);
            }
        }
    }

    remove();
    isAtion = false;
}

//�����Ҫ�����ķ���
void Game:: markRemove(SpriteType* spr)
{
    spr->setIsNeedRemove(true);
}

//����������̲������ⷽ��ķ���
void Game::markNextToSpecial(SpriteType* spr)
{
    spr->setIsNextToSpecial(true);
}

//���������
void Game::checkRow(SpriteType* spr,list<SpriteType*>* lineList)
{
    lineList->push_back(spr);

    //�����
    int count = spr->getCol() - 1;
    while (count >= 0)
    {
        SpriteType* leftSpr = myMap[spr->getRow()][count];
        if (!leftSpr)
            break;
        if (spr->getType() == leftSpr->getType())
        {
            lineList->push_back(leftSpr);
            count--;
        }
        else break;
    }

    //���Ҳ�
    count = spr->getCol() +1;
    while (count < MAX_COL)
    {
        SpriteType* rightSpr = myMap[spr->getRow()][count];
        if (!rightSpr)
            break;
        if (spr->getType() == rightSpr->getType())
        {
            lineList->push_back(rightSpr);
            count++;
        }
        else break;
    }
}

//���������
void Game::checkCol(SpriteType* spr, list<SpriteType*>* lineList)
{
    lineList->push_back(spr);

    //���ϲ�
    int count = spr->getRow() + 1;
    while (count < MAX_ROW)
    {
        SpriteType* upSpr = myMap[count][spr->getCol()];
        if (!upSpr)
            break;
        if (spr->getType() == upSpr->getType())
        {
            lineList->push_back(upSpr);
            count++;
        }
        else break;
    }

    //���²�
    count = spr->getRow() - 1;
    while (count >= 0)
    {
        SpriteType* downSpr = myMap[count][spr->getCol()];
        if (!downSpr)
            break;
        if (spr->getType() == downSpr->getType())
        {
            lineList->push_back(downSpr);
            count--;
        }
        else break;
    }
}

//��������
void Game::remove()
{
    isAtion = true;
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (myMap[i][j] == NULL)
                continue;

            if (myMap[i][j]->getIsNeedRemove())
            {
                isNeedFilled = true;

                if (!myMap[i][j]->getIsSpecial())
                {
                    myMap[i][j]->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
                    myMap[i][j] = NULL;

                    gameScore += 30;
                    CCLabelTTF* scoreLable = (CCLabelTTF*)this->getChildByTag(1);
                    scoreLable->setString(CCString::createWithFormat("Score: %d", gameScore)->getCString());
                }
                else//���ⷽ��������������
                    specialRemove(myMap[i][j]);
            }
        }
    }
}

//���ⷽ�������
void Game::specialRemove(SpriteType* spr)
{
    if (spr == NULL || !spr->getIsSpecial())
        return;

    int row = spr->getRow();
    int col = spr->getCol();
    if (!myMap[row][col])
        return;
    int specialT = myMap[row][col]->getSpecialType();

    myMap[row][col]->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
    myMap[row][col] = NULL;

    gameScore += 60;
    CCLabelTTF* scoreLable = (CCLabelTTF*)this->getChildByTag(1);
    scoreLable->setString(CCString::createWithFormat("Score: %d", gameScore)->getCString());

    if (specialT == 0)//���������ķ���
    {

    }
    else if (specialT == 1)//һ���ĸ���ͬ���������ⷽ�����������ǰ�����еķ���
    {
        //��������Ч
        Sprite* rowLine = Sprite::create("rowLine.jpg");
        rowLine->setScaleX(0.55 * WINDOWS_WIDTH / rowLine->getContentSize().width);
        rowLine->setScaleY(0.01 * WINDOWS_HEIGHT / rowLine->getContentSize().height);
        rowLine->setPosition(Point(WINDOWS_WIDTH * (0.25 + 0.07 * 3.5), WINDOWS_HEIGHT * (0.15 + 0.105 * row)));
        rowLine->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
        this->addChild(rowLine);

        for (int k = 0; k < MAX_COL; k++)
        {
            if (myMap[row][k] == NULL)
                continue;

            if (!myMap[row][k]->getIsSpecial())
            {
                myMap[row][k]->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
                myMap[row][k] = NULL;
            }
            else
                specialRemove(myMap[row][k]);
        }
    }
    else if (specialT == 2)//һ���ĸ���ͬ���������ⷽ�����������ǰ�����еķ���
    {
        for (int k = 0; k < MAX_ROW; k++)
        {
            //��������Ч
            Sprite* colLine = Sprite::create("colLine.jpg");
            colLine->setScaleX(0.005 * WINDOWS_WIDTH / colLine->getContentSize().width);
            colLine->setScaleY(0.80 * WINDOWS_HEIGHT / colLine->getContentSize().height);
            colLine->setPosition(Point(WINDOWS_WIDTH * (0.25 + 0.07 * col), WINDOWS_HEIGHT * (0.15 + 0.105 * 3.5)));
            colLine->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
            this->addChild(colLine);

            if (myMap[k][col] == NULL)
                continue;

            if (!myMap[k][col]->getIsSpecial())
            {
                myMap[k][col]->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
                myMap[k][col] = NULL;
            }
            else
                specialRemove(myMap[k][col]);
        }
    }
    else if (specialT == 3)//���ƽ���·�ڲ��������ⷽ�����������Χ�˸�
    {
        //һȦ������Ч
        spr->runAction(ScaleTo::create(0.2, 2.0, 2.0));

        for (int m = row - 1; m <= row + 1; m++)
        {
            for (int n = col - 1; n <= col + 1; n++)
            {
                if (myMap[m][n] == NULL || m < 0 || m >= MAX_ROW || n < 0 || n >= MAX_COL )
                    continue;
                
                if (!myMap[m][n]->getIsSpecial())
                {
                    myMap[m][n]->runAction(Sequence::create(Blink::create(0.5, 2), ScaleTo::create(0.5, 0.0, 0.0), NULL));
                    myMap[m][n] = NULL;
                }
                else
                    specialRemove(myMap[m][n]);
            }
        }
    }
}

//�����¼���ʼ
bool Game::onTouchBegan(Touch* touch, Event* event)
{
    //���黹�ڶ����ܴ���
    if (isAtion)
        return false;

    Vec2 staPosition = touch->getLocation();

    staTouch = getTouchedSprite(staPosition);

    return true;
}

//��ȡ����λ�õķ���
SpriteType* Game::getTouchedSprite(Vec2& position)
{
    Rect rect ;

    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (!myMap[i][j])
                continue;
            rect = myMap[i][j]->getBoundingBox();//�жϴ������Ƿ�����ĳһ��������ľ�����
            if (rect.containsPoint(position))
                return myMap[i][j];
        }
    }

    return NULL;
}

//������
bool Game::onTouchMoved(Touch* touch, Event* event)
{
    return true;
}

//��������
bool Game::onTouchEnded(Touch* touch, Event* event)
{
    SpriteType* spr;
    Vec2 endPosition = touch->getLocation();

    if (!staTouch)
        return false;

    int row = staTouch->getRow();
    int col = staTouch->getCol();

    //�жϴ����������Ƿ��ڴ�����ʼ����Ϸ�
    if (row < MAX_ROW - 1 && myMap[row + 1][col])
    {
        Rect rect = myMap[row+1][col]->getBoundingBox();
        if (rect.containsPoint(endPosition))
        {
            endTouch = myMap[row + 1][col];
            tryToSwapSprite();
            return true;
        }
    }

    //�жϴ����������Ƿ��ڴ�����ʼ����·�
    if (row > 0 && myMap[row - 1][col])
    {
        Rect rect = myMap[row - 1][col]->getBoundingBox();
        if (rect.containsPoint(endPosition))
        {
            endTouch = myMap[row - 1][col];
            tryToSwapSprite();
            return true;
        }
    }

    //�жϴ����������Ƿ��ڴ�����ʼ����ҷ�
    if (col < MAX_COL - 1 && myMap[row][col + 1])
    {
        Rect rect = myMap[row ][col + 1]->getBoundingBox();
        if (rect.containsPoint(endPosition))
        {
            endTouch = myMap[row][col + 1];
            tryToSwapSprite();
            return true;
        }
    }

    //�жϴ����������Ƿ��ڴ�����ʼ�����
    if (col > 0 && myMap[row][col - 1])
    {
        Rect rect = myMap[row][col - 1]->getBoundingBox();
        if (rect.containsPoint(endPosition))
        {
            endTouch = myMap[row][col - 1];
            tryToSwapSprite();
            return true;
        }
    }

    return true;
}

//�������鲢���м��
void Game::tryToSwapSprite()
{
    if (staTouch == NULL && endTouch == NULL)
        return;

    myMap[endTouch->getRow()][endTouch->getCol()] = staTouch;
    myMap[staTouch->getRow()][staTouch->getCol()] = endTouch;

    Point start = staTouch->getPosition();
    Point end = endTouch->getPosition();

    int tempRow = staTouch->getRow();
    int tempCol = staTouch->getCol();

    staTouch->setRow(endTouch->getRow());
    staTouch->setCol(endTouch->getCol());
    endTouch->setRow(tempRow);
    endTouch->setCol(tempCol);

    list<SpriteType*> staRowList;
    checkRow(staTouch, &staRowList);
    list<SpriteType*> staColList;
    checkCol(staTouch, &staColList);

    list<SpriteType*> endRowList;
    checkRow(endTouch, &endRowList);
    list<SpriteType*> endColList;
    checkCol(endTouch, &endColList); 

    //���������ⷽ�� �� ��������������
    if (swapSpecialSperite() || staRowList.size() >= 3 || staColList.size() >= 3 || endRowList.size() >= 3 || endColList.size() >= 3)
    {
        staTouch->runAction(MoveTo::create(0.25, end));
        endTouch->runAction(MoveTo::create(0.25, start));

        --gameSteps;
        CCLabelTTF* stepsLable = (CCLabelTTF*)this->getChildByTag(2);
        stepsLable->setString(CCString::createWithFormat("Steps: %d", gameSteps)->getCString());
        return;
    }

    //����������Ч�Ҳ��������򻻻���
    myMap[endTouch->getRow()][endTouch->getCol()] = staTouch;
    myMap[staTouch->getRow()][staTouch->getCol()] = endTouch;

    tempRow = staTouch->getRow();
    tempCol = staTouch->getCol();

    staTouch->setRow(endTouch->getRow());
    staTouch->setCol(endTouch->getCol());
    endTouch->setRow(tempRow);
    endTouch->setCol(tempCol);

    //���ؽ����Ķ���
    staTouch->runAction(Sequence::create(MoveTo::create(0.25, end), MoveTo::create(0.25, start),NULL));
    endTouch->runAction(Sequence::create(MoveTo::create(0.25, start), MoveTo::create(0.25, end), NULL));
}

//�������ķ��麬���ⷽ�飬���ܲ�������Ч��
bool Game::swapSpecialSperite()
{
    if (endTouch == NULL || staTouch == NULL)
        return false;

    //�������������ķ��齻����ȫͼ����
    if (staTouch->getIsSpecial() && endTouch->getIsSpecial() && staTouch->getSpecialType() == 0 && endTouch->getSpecialType() == 0)
    {
        for (int i = 0; i < MAX_ROW; i++)
        {
            for (int j = 0; j < MAX_COL; j++)
            {
                if (!myMap[i][j])
                    continue;
                myMap[i][j]->setIsNeedRemove(true);
            }
        }
        staTouch->setIsNeedRemove(true);
        endTouch->setIsNeedRemove(true);
        return true;
    }
    //�������������������ⷽ�齻��������
    else if (staTouch->getIsSpecial() && endTouch->getIsSpecial() && staTouch->getSpecialType() != 0 && endTouch->getSpecialType() != 0)
    {
        myMap[staTouch->getRow()][staTouch->getCol()]->setIsNeedRemove(true);
        myMap[endTouch->getRow()][endTouch->getCol()]->setIsNeedRemove(true);
        return true;
    }
    else if (staTouch->getIsSpecial() && staTouch->getSpecialType() == 0)
    {
        if (!endTouch->getIsSpecial())//������������ͨ���齻����ȫͼ�������ͷ�������
        {
            for (int i = 0; i < MAX_ROW; i++)
            {
                for (int j = 0; j < MAX_COL; j++)
                {
                    if (!myMap[i][j])
                        continue;

                    if (myMap[i][j]->getType() == endTouch->getType())
                        myMap[i][j]->setIsNeedRemove(true);
                }
            }
            staTouch->setIsNeedRemove(true);
            return true;
        }
        else if (endTouch->getSpecialType() != 0)//������������������ⷽ�齻����ȫͼ�������ͷ����Ը������ⷽ�����Ч����
        {
            for (int i = 0; i < MAX_ROW; i++)
            {
                for (int j = 0; j < MAX_COL; j++)
                {
                    if (!myMap[i][j])
                        continue;
                    if (myMap[i][j]->getType() == endTouch->getType())
                    {
                        if (endTouch->getSpecialType() != 3)
                        {
                            int randSpecialType = rand() % 2 + 1;
                            myMap[i][j]->setSpecial(randSpecialType);
                        }
                        else 
                            myMap[i][j]->setSpecial(endTouch->getSpecialType());
                        myMap[i][j]->setIsNeedRemove(true);
                    }
                }
            }
            staTouch->setIsNeedRemove(true);
            return true;
        }
    }
    else if (endTouch->getIsSpecial() && endTouch->getSpecialType() == 0)//ͬ��
    {
        if (!staTouch->getIsSpecial())
        {
            for (int i = 0; i < MAX_ROW; i++)
            {
                for (int j = 0; j < MAX_COL; j++)
                {
                    if (!myMap[i][j])
                        continue;

                    if (myMap[i][j]->getType() == staTouch->getType())
                        myMap[i][j]->setIsNeedRemove(true);
                }
            }
            endTouch->setIsNeedRemove(true);
            return true;
        }
        else if (staTouch->getSpecialType() != 0)
        {
            for (int i = 0; i < MAX_ROW; i++)
            {
                for (int j = 0; j < MAX_COL; j++)
                {
                    if (!myMap[i][j])
                        continue;
                    if (myMap[i][j]->getType() == staTouch->getType())
                    {
                        if (staTouch->getSpecialType() != 3)
                        {
                            int randSpecialType = rand() % 2 + 1;
                            myMap[i][j]->setSpecial(randSpecialType);
                        }
                        else
                            myMap[i][j]->setSpecial(staTouch->getSpecialType());
                        myMap[i][j]->setIsNeedRemove(true);
                    }
                }
            }
            endTouch->setIsNeedRemove(true);
            return true;
        }
    }
    return false;
}