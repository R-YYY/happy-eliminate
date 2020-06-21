#include "SpriteType.h"

SpriteType* SpriteType::setSprite(int r, int c)
{
	SpriteType* spr = new SpriteType();
	spr->row = r;
	spr->col = c;
	spr->type = rand() % TOTAL_KIND;
	spr->initWithFile(sprites[spr->type]);
	spr->setScale(0.7);
	spr->autorelease();
	return spr;
}

void SpriteType::setSpecialSprite()
{
	isNeedRemove = false;
	isNextToSpecial = false;

	if (specialType == 0)
	{
		Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[12]);
		this->setTexture(texture);
	}
	else if (specialType == 3)
	{
		Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[13]);
		this->setTexture(texture);
	}
	else
	{
		Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage(sprites[type + specialType * TOTAL_KIND]);
		this->setTexture(texture);
	}
	this->setScale(0.7);
}

void SpriteType::setRow(int r)
{
	row = r;
}

void SpriteType::setCol(int c)
{
	col = c;
}

void SpriteType::setType(int t)
{
	type = t;
}

void SpriteType::setIsNeedRemove(bool k)
{
	isNeedRemove = k;
}


void SpriteType::setSpecial(int t)
{
	isSpecial = true;
	specialType = t;
}

void SpriteType::setIsNextToSpecial(bool k)
{
	isNextToSpecial = k;
}

int SpriteType::getRow()
{
	return row;
}

int SpriteType::getCol()
{
	return col;
}

int SpriteType::getType()
{
	return type;
}

int SpriteType::getSpecialType()
{
	return specialType;
}

bool SpriteType::getIsNeedRemove()
{
	return isNeedRemove;
}

bool SpriteType::getIsSpecial()
{
	return isSpecial;
}

bool SpriteType::getIsNextToSpecial()
{
	return isNextToSpecial;
}