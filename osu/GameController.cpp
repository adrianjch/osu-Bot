#include "GameController.h"
#include "WindowsInput.h"
#include "OsuUtils.h"
#include "Circle.h"
#include "Slider.h"
#include "Spinner.h"
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>

GameController::GameController()
{
	timeMultiplier = 1.0f;
}

void GameController::ChangeSong()
{
	song = GetSong();
}

void GameController::StartSong()
{
	firstTime = true;
	// Starts reading the file
	std::ifstream file(song);

	while (content.find("SliderMultiplier:") != 0)
		std::getline(file, content);
	sliderMultiplier = std::stof(content.substr(content.find(":") +1));

	ReadTimingPoints(file);

	while (content != "[HitObjects]")
		std::getline(file, content);
	std::noskipws(file);
	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

	///starts reading the objects
	while (std::getline(file, content))
	{
		currentObject = ParseObject(content);
		std::cout << currentObject->timer << std::endl;

		if (firstTime)
		{
			// ignore the first hit since rn it's done by the player
			mousePos = currentObject->pos;
			previousTimer = currentObject->timer;
			firstGameTimer = previousTimer;
			firstRealTimer = std::chrono::high_resolution_clock::now();
			std::getline(file, content);
			currentObject = ParseObject(content);
			firstTime = false;
		}

		///CIRCLE
		if (currentObject->type == Object::CIRCLE)
		{
			moveMouse(mousePos, currentObject->pos, (currentObject->timer - firstGameTimer) / timeMultiplier - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());
			currentObject->Behavior(this);
		}
		///SLIDER
		else if (currentObject->type == Object::SLIDER)
		{
			moveMouse(mousePos, currentObject->pos, (currentObject->timer - firstGameTimer) / timeMultiplier - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());
			currentObject->Behavior(this);
		}
		///SPINNER
		else if (currentObject->type == Object::SPINNER)
		{
			moveMouse(mousePos, Vec2(256, 142), (currentObject->timer - firstGameTimer) / timeMultiplier - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());
			currentObject->Behavior(this);
		}
		///press ESCAPE to stop
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			std::cout << "Escaped" << std::endl;
			break;
		}
	}
	file.close();
}

void GameController::SetMousePos(Vec2 newPos)
{
	mousePos = newPos;
}

void GameController::SetPreviousTimer(float newTimer)
{
	previousTimer = newTimer;
}

float GameController::GetTimeMultiplier()
{
	return timeMultiplier;
}

float GameController::GetSliderMultiplier()
{
	return sliderMultiplier;
}

float GameController::GetFirstGameTimer()
{
	return firstGameTimer;
}

std::chrono::steady_clock::time_point GameController::GetFirstRealTimer()
{
	return firstRealTimer;
}

float GameController::GetBeatLength(float timer)
{
	for (int i = 0; i < timingPoints.size(); i++)
	{
		if (timingPoints[i].timer > timer)
			return timingPoints[i-1].beatLength;
	}
	return timingPoints.back().beatLength;
}

std::shared_ptr<Object> GameController::ParseObject(const std::string& content)
{
	// Get segments separated by comma
	std::stringstream stream(content);
	std::string segment;
	std::vector<std::string> seglist;
	while (std::getline(stream, segment, ','))
	{
		seglist.push_back(segment);
	}

	std::shared_ptr<Object> object;
	// https://www.reddit.com/r/osugame/comments/36jyth/need_help_understanding_osu_file_format_syntax/
	// 1 = circle
	// 2 = slider
	// 5 = circle (starts new combo)
	// 6 = slider (starts new combo)
	// 12 = spinner
	if (seglist[3] == "12")
	{
		object = std::make_shared<Spinner>();
	}
	else if (seglist[3] == "2" || seglist[3] == "6")
	{
		object = std::make_shared<Slider>();
	}
	else
	{
		object = std::make_shared<Circle>();
	}
	object->Parse(seglist);

	return object;
}

void GameController::ReadTimingPoints(std::ifstream& file)
{
	bool original = true;
	while (content != "[TimingPoints]")
		std::getline(file, content);

	std::getline(file, content);
	while (!content.empty())
	{
		// Get segments separated by comma
		std::stringstream stream(content);
		std::string segment;
		std::vector<std::string> seglist;
		while (std::getline(stream, segment, ','))
		{
			seglist.push_back(segment);
		}
		int timer = std::stoi(seglist[0]);
		float beatLength = std::stof(seglist[1]);
		if (original)
		{
			original = false;
			originalBeatLength = beatLength;
		}
		else if (beatLength < 0) // If it's an inherited point, parse it
		{
			beatLength = -100.0f / beatLength;
			beatLength = originalBeatLength / beatLength;
		}
		timingPoints.push_back(TimingPoint(timer, beatLength));

		std::getline(file, content);
	}
}