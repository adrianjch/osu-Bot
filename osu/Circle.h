#pragma once
#include "Object.h"

class Circle : public Object
{
public:
	void Parse(const std::vector<std::string>& segments) override
	{
		Init(segments);

		type = CIRCLE;
	}

	void Behavior(GameController* game) override
	{
		game->SetMousePos(pos);
		game->SetPreviousTimer(timer);
		rightClick();
	}

	// Parsed position could be wrong due to stacked circles
	Vec2 realPos;
};