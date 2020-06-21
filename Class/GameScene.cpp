
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

    //背景
    Sprite* background1 = Sprite::create("onepiece.jpg");
    background1->setContentSize(winSize);
    background1->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(background1);

    //背景音乐及部分按钮
    backGroundLayer* player_bg = backGroundLayer::create();
    addChild(player_bg);
    
    //目标分数
    auto goal = LabelTTF::create("Goal: 10000", "Arial", 30);
    goal->setColor(ccc3(255, 0, 0));
    goal->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.75));
    this->addChild(goal);

    //当前分数
    auto scoreLabel = LabelTTF::create(CCString::createWithFormat("Score: %d", gameScore)->getCString(), "Arial", 30);
    scoreLabel->setColor(ccc3(255,0,0));
    scoreLabel->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.65));
    this->addChild(scoreLabel, 0, 1);

    //剩余步数
    gameSteps = 25;
    auto stepsLabel = LabelTTF::create(CCString::createWithFormat("Steps: %d", gameSteps)->getCString(), "Arial", 30);
    stepsLabel->setColor(ccc3(255, 0, 0));
    stepsLabel->setPosition(Vec2(winSize.width * 0.11, winSize.height * 0.55));
    this->addChild(stepsLabel, 0, 2);

    
    srand(unsigned(time(0)));

    //加载地图
    initMap();

    //更新
    schedule(schedule_selector(Game::update),0.5);

    //添加鼠标触摸事件
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
}

//加载地图
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

//产生方块
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

//实时更新
void Game::update(float time)
{
    //如果仍有方块在动，则不进行其他操作
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
        if (gameScore >= 10000 && gameSteps > 0)//完成目标且还有剩余步数
        {
            vector<int>finalSpecial;//用来储存剩余步数对应的特殊方块位置
            
            int i = 0;
            while (i < gameSteps)
            {
                bool same = false;
                int temp = rand() % (MAX_ROW * MAX_COL);

                for (int j = 0; j < i; j++)//新方块位置之前已存在
                    if (temp == finalSpecial[j])
                        same = true;

                if (same)//如果新方块位置之前已存在，寻找下一块
                    continue;

                if (myMap[temp / 8][temp % 8] == NULL)
                    continue;

                i++;
                finalSpecial.push_back(temp);

                myMap[temp / 8][temp % 8]->setSpecial(rand() % 3 + 1);//新方块的特殊类型
                myMap[temp / 8][temp % 8]->setIsNeedRemove(true);//需要被移除
                SpriteType* spr = myMap[temp / 8][temp % 8];

                //更换新方块图片
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
        else if (gameSteps <= 0 && gameScore >= 10000)//没有步数但是达到了目标分数，下一关
        {
            Director::getInstance()->replaceScene(Second::createScene());
        }
        else if (gameSteps <= 0 && gameScore < 10000)//没有步数了且未达到目标分数，结束游戏
        {
            Director::getInstance()->replaceScene(Gameover::createScene());
        }
    }

}

//填补空缺
void Game::FillSprite()
{
    isAtion = true;

    //用来存每列被消除的方块数
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

                //将下方有空缺的方块向下移
                Point start = myMap[newRow][j]->getPosition();
                Point end = Point(WINDOWS_WIDTH * (0.25 + 0.07 * j), WINDOWS_HEIGHT * (0.14 + 0.105 * newRow));
                myMap[newRow][j]->stopAllActions();
                myMap[newRow][j]->runAction(MoveTo::create(0.5, end));
            }
        }
        colEmptyNumber[j] = colEmpty;
    }

    //在每列的上方产生新的方块
    for (int j = 0; j < MAX_COL; j++) 
    {
        for (int i = MAX_ROW - colEmptyNumber[j]; i < MAX_ROW; i++) 
        {
            createSprite(i, j);
        }
    }

    free(colEmptyNumber);
}

//查找并消除
void Game::checkAndRemove()
{
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            SpriteType* spr = myMap[i][j];
            if(spr == NULL)
                continue;

            //仅考虑消除过程中未产生特殊方块的方块以及不用消除的方块
            if (spr->getIsNeedRemove() && spr->getIsNextToSpecial())
                continue;
            
            list<SpriteType*> rowList;
            checkRow(spr, &rowList);
            list<SpriteType*> colList;
            checkCol(spr, &colList);

            list<SpriteType*>::iterator count;
            
            if (rowList.size() >= 5 && colList.size() < 3)//一行五个及以上相同的
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//新产生的特殊方块保留不消除
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(0); 
            }           
            else if(colList.size() >= 5 && rowList.size() < 3)//一列五个及以上相同的
            {
                for (count = colList.begin(); count != colList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//新产生的特殊方块保留不消除
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(0);
            }
            else if (rowList.size() == 4 && colList.size() < 3)//一行四个相同的
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//新产生的特殊方块保留不消除
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(1);
            }            
            else if (colList.size() == 4 && rowList.size() < 3)//一列四个相同的
            {
                for (count = colList.begin(); count != colList.end(); count++)
                {
                    markRemove((SpriteType*)*count);
                    markNextToSpecial((SpriteType*)*count);
                }
                if (!spr->getIsSpecial())//新产生的特殊方块保留不消除
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(2);
            }           
            else if (colList.size() >= 3 && colList.size() < 5 && rowList.size() >= 3 && rowList.size() < 5)//   T \ L型
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
                if (!spr->getIsSpecial())//新产生的特殊方块保留不消除
                {
                    spr->runAction(Blink::create(0.5, 2));
                    spr->setIsNeedRemove(false);
                }
                spr->setSpecial(3);
            }           
            else if (rowList.size() == 3 && colList.size() < 3)//一行三个
            {
                for (count = rowList.begin(); count != rowList.end(); count++)
                    markRemove((SpriteType*)*count);
            }           
            else if (colList.size() == 3 && rowList.size() < 3)//一列三个
            {
                for (count = colList.begin(); count != colList.end(); count++)
                    markRemove((SpriteType*)*count);
            }
        }
    }

    SpriteType* spr;
    //新产生的特殊方块更换图片
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

//标记需要消除的方块
void Game:: markRemove(SpriteType* spr)
{
    spr->setIsNeedRemove(true);
}

//标记消除过程产生特殊方块的方块
void Game::markNextToSpecial(SpriteType* spr)
{
    spr->setIsNextToSpecial(true);
}

//检查行消除
void Game::checkRow(SpriteType* spr,list<SpriteType*>* lineList)
{
    lineList->push_back(spr);

    //向左查
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

    //向右查
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

//检查列消除
void Game::checkCol(SpriteType* spr, list<SpriteType*>* lineList)
{
    lineList->push_back(spr);

    //向上查
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

    //向下查
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

//消除方块
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
                else//特殊方块特殊消除方法
                    specialRemove(myMap[i][j]);
            }
        }
    }
}

//特殊方块的消除
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

    if (specialT == 0)//五消产生的方块
    {

    }
    else if (specialT == 1)//一行四个相同产生的特殊方块可以消除当前行所有的方块
    {
        //行消除特效
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
    else if (specialT == 2)//一列四个相同产生的特殊方块可以消除当前列所有的方块
    {
        for (int k = 0; k < MAX_ROW; k++)
        {
            //列消除特效
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
    else if (specialT == 3)//类似交叉路口产生的特殊方块可以消除周围八个
    {
        //一圈消除特效
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

//触摸事件开始
bool Game::onTouchBegan(Touch* touch, Event* event)
{
    //方块还在动则不能触摸
    if (isAtion)
        return false;

    Vec2 staPosition = touch->getLocation();

    staTouch = getTouchedSprite(staPosition);

    return true;
}

//获取触摸位置的方块
SpriteType* Game::getTouchedSprite(Vec2& position)
{
    Rect rect ;

    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (!myMap[i][j])
                continue;
            rect = myMap[i][j]->getBoundingBox();//判断触摸点是否在以某一方块产生的矩形内
            if (rect.containsPoint(position))
                return myMap[i][j];
        }
    }

    return NULL;
}

//触摸中
bool Game::onTouchMoved(Touch* touch, Event* event)
{
    return true;
}

//触摸结束
bool Game::onTouchEnded(Touch* touch, Event* event)
{
    SpriteType* spr;
    Vec2 endPosition = touch->getLocation();

    if (!staTouch)
        return false;

    int row = staTouch->getRow();
    int col = staTouch->getCol();

    //判断触摸结束点是否在触摸开始点的上方
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

    //判断触摸结束点是否在触摸开始点的下方
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

    //判断触摸结束点是否在触摸开始点的右方
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

    //判断触摸结束点是否在触摸开始点的左方
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

//交换方块并进行检查
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

    //若交换特殊方块 或 交换方块后可消除
    if (swapSpecialSperite() || staRowList.size() >= 3 || staColList.size() >= 3 || endRowList.size() >= 3 || endColList.size() >= 3)
    {
        staTouch->runAction(MoveTo::create(0.25, end));
        endTouch->runAction(MoveTo::create(0.25, start));

        --gameSteps;
        CCLabelTTF* stepsLable = (CCLabelTTF*)this->getChildByTag(2);
        stepsLable->setString(CCString::createWithFormat("Steps: %d", gameSteps)->getCString());
        return;
    }

    //交换后无特效且不能消除则换回来
    myMap[endTouch->getRow()][endTouch->getCol()] = staTouch;
    myMap[staTouch->getRow()][staTouch->getCol()] = endTouch;

    tempRow = staTouch->getRow();
    tempCol = staTouch->getCol();

    staTouch->setRow(endTouch->getRow());
    staTouch->setCol(endTouch->getCol());
    endTouch->setRow(tempRow);
    endTouch->setCol(tempCol);

    //来回交换的动画
    staTouch->runAction(Sequence::create(MoveTo::create(0.25, end), MoveTo::create(0.25, start),NULL));
    endTouch->runAction(Sequence::create(MoveTo::create(0.25, start), MoveTo::create(0.25, end), NULL));
}

//若交换的方块含特殊方块，可能产生特殊效果
bool Game::swapSpecialSperite()
{
    if (endTouch == NULL || staTouch == NULL)
        return false;

    //两个五消产生的方块交换则全图消除
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
    //两个非五消产生的特殊方块交换后都消除
    else if (staTouch->getIsSpecial() && endTouch->getIsSpecial() && staTouch->getSpecialType() != 0 && endTouch->getSpecialType() != 0)
    {
        myMap[staTouch->getRow()][staTouch->getCol()]->setIsNeedRemove(true);
        myMap[endTouch->getRow()][endTouch->getCol()]->setIsNeedRemove(true);
        return true;
    }
    else if (staTouch->getIsSpecial() && staTouch->getSpecialType() == 0)
    {
        if (!endTouch->getIsSpecial())//五消方块与普通方块交换则全图该种类型方块消除
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
        else if (endTouch->getSpecialType() != 0)//五消方块与非五消特殊方块交换则全图该种类型方块以该种特殊方块的特效消除
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
    else if (endTouch->getIsSpecial() && endTouch->getSpecialType() == 0)//同上
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