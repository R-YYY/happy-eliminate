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
	//��������
	int type;
	//���ⷽ������
	int specialType;
	//�Ƿ������ⷽ��
	bool isSpecial;
	//�Ƿ�����Ҫ�����������ⷽ��
	bool isNextToSpecial;
	//�Ƿ���Ҫ������
	bool isNeedRemove;

public:

	SpriteType(int r = 0, int c = 0, int t = 0) :row(r), col(c), type(t), isSpecial(false), isNextToSpecial(false), isNeedRemove(false) { }
	~SpriteType() { }

	static SpriteType* setSprite(int r, int c);		
	void setSpecialSprite();

	//������
	void setRow(int r);
	//������
	void setCol(int c);
	//���þ�������
	void setType(int t);
	//������Ҫ������
	void setIsNeedRemove(bool k);
	//�������ⷽ�������
	void setSpecial(int t);
	//�����Ƿ��Ž������������ⷽ��
	void setIsNextToSpecial(bool k);

	//��ȡ��
	int getRow();
	//��ȡ��
	int getCol();
	//��ȡ����
	int getType();
	//��ȡ��������
	int getSpecialType();
	//�Ƿ���Ҫ������
	bool getIsNeedRemove();
	//�Ƿ�����
	bool getIsSpecial();
	//�Ƿ��Ž������������ⷽ��
	bool getIsNextToSpecial();
};

#endif // __SPRITE_TYPE_H__