#pragma once
#include "pch.h"

class Item : public Sprite
{
public:
	int	   count;
	ItemID itemID;

public:
	// ------------------------------------------ [ 값만 가지는 아이템 ]

	static Item* NORMAL()
	{
		Item* item = new Item();
		if (item)
		{
			item->autorelease();
			item->count = 0;
			item->itemID = ItemID::NORMAL;
		}
		return item;
	}

	// ------------------------------------------ [ 공격 아이템 ]

	static Item* ARROW(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::ARROW)))
		{
			item->autorelease();
			item->count += 1;
			item->itemID = ItemID::ARROW;

			item->setPosition(pos);
		}
		return item;
	}

	static Item* TRAP(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::TRAP)))
		{
			item->autorelease();
			item->count += 1;
			item->itemID = ItemID::TRAP;

			item->setPosition(pos);
		}
		return item;
	}

	static Item* WIRE(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::WIRE)))
		{
			item->autorelease();
			item->count += 1;
			item->itemID = ItemID::WIRE;

			item->setPosition(pos);
		}
		return item;
	}

	// ------------------------------------------ [ 즉시 시전 ]

	static Item* HEALTH(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::HEALTH)))
		{
			item->autorelease();
			item->itemID = ItemID::HEALTH;

			item->setPosition(pos);
		}
		return item;
	}

	static Item* SPEED(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::SPEED)))
		{
			item->autorelease();
			item->itemID = ItemID::SPEED;

			item->setPosition(pos);
		}
		return item;
	}

	static Item* GUARD(const Vec2& pos)
	{
		Item* item = new Item();
		if (item&&item->initWithFile(StringUtils::format("Item/%d.png", ItemID::GUARD)))
		{
			item->autorelease();
			item->itemID = ItemID::GUARD;

			item->setPosition(pos);
		}
		return item;
	}
};