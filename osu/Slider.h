#pragma once
#include "Object.h"
#include <sstream>

class Slider : public Object
{
public:
	enum CurveType : short { LINEAR, BEZIER, PASS_THROUGH, CATMULL };

	void Parse(const std::vector<std::string>& segments) override
	{
		Init(segments);

		type = SLIDER;

		std::string curve = segments[5];
		repetitions = std::stoi(segments[6]);
		speed = std::stoi(segments[7]);
		switch (curve[0])
		{
			case 'L':
				curveType = LINEAR;
				break;
			case 'B':
				curveType = BEZIER;
				break;
			case 'P':
				curveType = PASS_THROUGH;
				break;
			case 'C':
				curveType = CATMULL;
				break;
			default:
				curveType = BEZIER;
				break;
		}

		// Get segments separated by comma
		std::stringstream stream(curve);
		std::string segment;
		std::vector<std::string> seglist;
		while (std::getline(stream, segment, '|'))
		{
			seglist.push_back(segment);
		}

		// Save points
		points.push_back(pos);
		for (int i = 1; i < seglist.size(); i++)
		{
			int aux = seglist[i].find(":");
			int x = std::stoi(seglist[i].substr(0, aux));
			int y = std::stoi(seglist[i].substr(aux+1));
			points.push_back(Vec2(x, y));
		}
	}

	CurveType curveType;
	std::vector<Vec2> points;
	int repetitions;
	int speed;
};