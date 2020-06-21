#ifndef __SPRITE_TYPE_H__
#define __SPRITE_TYPE_H__

#include "cocos2d.h"

#define TOTAL_KIND 4

static const char* sprites[TOTAL_KIND + 13] = {"luff.png",
                                               "solo.png",
											   "sanji.png",
											   "xks.png",
											   "luff_row.png",
											   "solo_row.png",
											   "sanji_row.png",
											   "xks_row.png",
											   "luff_col.png",
											   "solo_col.png",
											   "sanji_col.png",
											   "xks_col.png",
											   "luff_around.png",
											   "solo_around.png",
											   "sanji_around.png",
											   "xks_around.png",
	                                           "fruit.png"
};

USING_NS_CC;

class SpriteType: public Sprite
{
private:

	int row;
	int col;
	//精灵类型
	int type;
	//特殊方块类型
	int specialType;
	//是否是特殊方块
	bool isSpecial;
	//是否挨着需要被消除的特殊方块
	bool isNextToSpecial;
	//是否需要被消除
	bool isNeedRemove;

public:

	SpriteType(int r = 0, int c = 0, int t = 0) :row(r), col(c), type(t), isSpecial(false), isNextToSpecial(false), isNeedRemove(false) { }
	~SpriteType() { }

	static SpriteType* setSprite(int r, int c);		
	void setSpecialSprite();

	//设置行
	void setRow(int r);
	//设置列
	void setCol(int c);
	//设置精灵类型
	void setType(int t);
	//设置需要被消除
	void setIsNeedRemove(bool k);
	//设置特殊方块的类型
	void setSpecial(int t);
	//设置是否挨着将被消除的特殊方块
	void setIsNextToSpecial(bool k);

	//获取行
	int getRow();
	//获取列
	int getCol();
	//获取类型
	int getType();
	//获取特殊类型
	int getSpecialType();
	//是否需要被消除
	bool getIsNeedRemove();
	//是否特殊
	bool getIsSpecial();
	//是否挨着将被消除的特殊方块
	bool getIsNextToSpecial();
};

#endif // __SPRITE_TYPE_H__