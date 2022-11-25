#pragma once
#include "Object.h"
#include <chrono>
#include <memory>

class GameController
{
public:
	struct TimingPoint
	{
		int timer;
		float beatLength;

		TimingPoint(int _timer, float _beatLength) : timer(_timer), beatLength(_beatLength) {}
	};

	GameController();
	void ChangeSong();
	void StartSong();

	void SetMousePos(Vec2 newPos);
	void SetPreviousTimer(float newTimer);
	float GetTimeMultiplier();
	float GetSliderMultiplier();
	float GetFirstGameTimer();
	std::chrono::steady_clock::time_point GetFirstRealTimer();
	float GetBeatLength(float timer);
private:
	std::shared_ptr<Object> ParseObject(const std::string& content);
	void ReadTimingPoints(std::ifstream& file);

	Vec2 mousePos;
	float timeMultiplier; //0.75 for HT and 1.5 for DT
	float sliderMultiplier; // constant for each beatmap
	float originalBeatLength; // can change with TimingPoints
	std::vector<TimingPoint> timingPoints;
	int previousTimer;
	float firstGameTimer;
	std::chrono::steady_clock::time_point firstRealTimer;
	bool firstTime;

	std::string song;
	std::string content;
	std::shared_ptr<Object> currentObject;
};