#pragma once
#include "Object.h"
#include "OsuUtils.h"

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
	void Behavior(GameController* game) override
	{
		std::chrono::steady_clock::time_point temp = std::chrono::high_resolution_clock::now();
		int spinnerEnd = end / game->timeMultiplier;
		int angle = 0;
		hold();
		while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - game->firstRealTimer).count() < spinnerEnd - game->firstGameTimer)
		{
			// Every 1ms, move by X angle
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 1)
			{
				moveMouse(Vec2(256 + 50 * cos(angle * 3.14f / 180), 192 + 50 * sin(angle * 3.14f / 180)));
				temp = std::chrono::high_resolution_clock::now();
				angle += 15;
				angle = angle % 360;
			}
		}
		release();

		game->mousePos = pos + Vec2(0, -50);
		game->previousTimer = spinnerEnd;
	}

	int duration;
	int end;
};