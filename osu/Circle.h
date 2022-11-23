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

	// Parsed position could be wrong due to stacked circles
	Vec2 realPos;
};