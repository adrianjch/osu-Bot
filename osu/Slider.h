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

	void Behavior(GameController* game) override
	{
		// Pass-through specific
		Vec2 center;
		float maxAngle;
		if (curveType == Slider::PASS_THROUGH)
			PreCalculatePassThrough(points, center, maxAngle);

		// Bezier specific
		std::vector<BezierStruct> beziers;
		if (curveType == Slider::BEZIER)
			beziers = PreCalculateBezier(points);

		hold();
		float sliderLength = (float)speed / 100.0f; // this is the amount of beats the slider has
		sliderLength *= game->GetBeatLength(timer) / (game->GetSliderMultiplier() * game->GetTimeMultiplier()); // this its length in seconds
		int repsDone = 0;
		do
		{
			std::chrono::steady_clock::time_point start2 = std::chrono::high_resolution_clock::now();
			std::chrono::steady_clock::time_point temp = start2;
			float alpha = 0.0f;
			while (alpha < 1.0f)
			{
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 1)
				{
					temp = std::chrono::high_resolution_clock::now();
					auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start2).count();
					alpha = (float)timePassed / (float)sliderLength;

					if (curveType == Slider::LINEAR)
					{
						moveMouse(Linear(points, (repsDone % 2 == 0) ? alpha : 1.0f - alpha));
					}
					else if (curveType == Slider::PASS_THROUGH)
					{
						moveMouse(PassThrough(points[0], (repsDone % 2 == 0) ? alpha : 1.0f - alpha, center, maxAngle));
					}
					else
					{
						if (beziers.size() == 1)
							moveMouse(Bezier(points, (repsDone % 2 == 0) ? alpha : 1.0f - alpha));
						else
							moveMouse(ConcatenatedBezier(beziers, (repsDone % 2 == 0) ? alpha : 1.0f - alpha));
					}
				}
			}
			repsDone++;
		} while (repsDone < repetitions);
		game->SetMousePos((repetitions % 2 != 0) ? points.back() : points.front());// end of slider
		game->SetPreviousTimer(timer + sliderLength * repetitions);// time when slider ends
		//Sleep(20);
		release();
	}

	CurveType curveType;
	std::vector<Vec2> points;
	int repetitions;
	int speed;
};