#pragma once
#include "Object.h"

class Spinner : public Object
{
public:
	void Parse(const std::vector<std::string>& segments) override
	{
		Init(segments);

		type = SPINNER;

		end = std::stoi(segments[5]);
		duration = end - std::stoi(segments[2]);
	}

	int duration;
	int end;
};