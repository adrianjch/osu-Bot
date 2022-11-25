#pragma once
#include "Vector2.h"
#include <vector>
#include <string>
#include <memory>

class GameController;

class Object
{
public:
	enum Type : short { CIRCLE, SLIDER, SPINNER, NOTHING };

protected:
	void Init(const std::vector<std::string>& segments)
	{
		pos.x = std::stoi(segments[0]);
		pos.y = std::stoi(segments[1]);
		timer = std::stoi(segments[2]);
	}

public:
	virtual void Parse(const std::vector<std::string>& segments) = 0;
	virtual void Behavior(GameController* game) = 0;

	Vec2 pos;
	int timer;
	Type type;
};