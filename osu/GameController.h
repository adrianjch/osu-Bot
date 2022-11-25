#pragma once
#include "Object.h"
#include <chrono>
#include <memory>

class GameController
{
public:
	GameController();
	void ChangeSong();
	void StartSong();
//private:
	Vec2 mousePos;
	float timeMultiplier; //0.75 for HT and 1.5 for DT
	float sliderMultiplier; // constant for each beatmap
	float beatLength; // can change with TimingPoints
	int previousTimer;
	float firstGameTimer;
	std::chrono::steady_clock::time_point firstRealTimer;
	bool firstTime;

	std::string song;
	std::string content;
	std::shared_ptr<Object> currentObject;
private:
	std::shared_ptr<Object> ParseObject(const std::string& content);
};