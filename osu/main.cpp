#include "WindowsInput.h"
#include "OsuUtils.h"
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <math.h>

// TODO LIST
// - Add reverse sliders
// - Add pass-through sliders
// - Start using timing points (BPM and slider speed) for sliders
// - Try to make the bot hit the first object instead of the user
// - Try to fix bezier speeds
// - Try to fix bezier red dots
int main() {
	Vec2 mousePos = Vec2(400, 300);
	float timeMultiplier = 1; //0.75 for HT and 1.5 for DT
	int previousTimer;
	float firstGameTimer;
	std::chrono::steady_clock::time_point firstRealTimer;
	bool firstTime;

	std::string song;
	std::string content = "";
	std::shared_ptr<Object> object;

	while (true) {
		///press enter to change to current song
		if (GetAsyncKeyState(VK_RETURN)) {
			song = GetSong();
		}
		///press 'Z' to start
		if (GetAsyncKeyState(0x5A)) {
			firstTime = true;
			///starts reading the file
			std::ifstream file(song);
			while (content != "[HitObjects]")
				std::getline(file, content);
			std::noskipws(file);
			std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

			///starts reading the objects
			while (std::getline(file, content)) {
				object = parseObject(content);
				//std::cout << content << std::endl;

				if (firstTime) {
					// ignore the first hit since rn it's done by the player
					mousePos = object->pos;
					previousTimer = object->timer;
					firstRealTimer = std::chrono::high_resolution_clock::now();
					firstGameTimer = previousTimer;
					std::getline(file, content);
					object = parseObject(content);
					firstTime = false;
				}

				///CIRCLE
				if (object->type == Object::CIRCLE || 
					(object->type == Object::SLIDER && std::dynamic_pointer_cast<Slider>(object)->curveType == Slider::BEZIER)) {
					moveMouse(mousePos, object->pos, (object->timer - firstGameTimer) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());

					mousePos = object->pos;
					previousTimer = object->timer;
					rightClick();
				}
				///SLIDER
				else if (object->type == Object::SLIDER) {
					moveMouse(mousePos, object->pos, (object->timer - firstGameTimer) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());
					
					hold();
					std::vector<Vec2> points = std::dynamic_pointer_cast<Slider>(object)->points;
					float timer = (float)std::dynamic_pointer_cast<Slider>(object)->speed / 100.0f;
					timer *= 187.0f;
					Slider::CurveType type = std::dynamic_pointer_cast<Slider>(object)->curveType;

					std::chrono::steady_clock::time_point start2 = std::chrono::high_resolution_clock::now();
					std::chrono::steady_clock::time_point temp = start2;
					float alpha = 0.0f;
					while (alpha < 1.0f)
					{
						if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 1)
						{
							temp = std::chrono::high_resolution_clock::now();
							auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start2).count();
							alpha = (float)timePassed / (float)timer;
							if (type == Slider::LINEAR)
							{
								moveMouse(Linear(points, alpha));
							}
							else
							{
								moveMouse(Bezier(points, alpha));
							}
						}
					}
					mousePos = points.back();// end of slider
					previousTimer = object->timer+timer;// time when slider ends idk
					Sleep(20);
					release();
				}
				///SPINNER
				else if (object->type == Object::SPINNER) {
					moveMouse(mousePos, Vec2(256, 142), (object->timer - firstGameTimer) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - firstRealTimer).count());
					
					std::chrono::steady_clock::time_point temp = std::chrono::high_resolution_clock::now();
					int spinnerEnd = std::dynamic_pointer_cast<Spinner>(object)->end;
					int angle = 0;
					hold();
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < (spinnerEnd - previousTimer) / timeMultiplier) {
						// Every 2ms, move by X angle
						if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 2)
						{
							moveMouse(Vec2(256 + 50 * cos(angle * PI / 180), 192 + 50 * sin(angle * PI / 180)));
							temp = std::chrono::high_resolution_clock::now();
							angle += 15;
							angle = angle % 360;
						}
					}
					release();
					mousePos = object->pos;
					previousTimer = spinnerEnd;
				}
				///press ESCAPE to stop
				if (GetAsyncKeyState(VK_ESCAPE))
					break;
			}
			file.close();
		}
	}
	return 0;
}