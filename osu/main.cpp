#include "Utils.h"
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <math.h>

int main() {
	Vec2 mousePos = Vec2(400, 300);
	float timeMultiplier = 1; //0.75 for HT and 1.5 for DT
	int timer = 0;
	int timer2;
	int firstTimer;
	bool firstTime;
	int aux;

	std::string song;
	std::string content = "";
	Object object;
	//moveMouse(308,101);
	//moveMouse(1229,791);
	while (true) {
		///press enter to change to current song
		if (GetAsyncKeyState(VK_RETURN)) {
			song = changeSong();
		}
		///press 'Z' to start
		if (GetAsyncKeyState(0x5A)) {
			firstTime = true;
			///starts reading the file
			std::ifstream file(song);
			while (content != "[HitObjects]")
				std::getline(file, content);
			content = "";
			std::noskipws(file);
			std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
			///starts reading the objects
			while (!file.eof()) {
				///set object coords+timer
				file >> object.pos.x;
				if (file.eof())
					break;
				file.ignore();
				file >> object.pos.y;
				file.ignore();
				file >> object.timer;
				if (firstTime) {
					firstTimer = object.timer;
					timer2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
					firstTime = false;
				}
				file.ignore();
				file >> aux;
				if (aux == 2) {///if is a slider
					/*file.ignore();
					file >> aux;
					file.ignore();
					file.ignore();*/
					object.type = SLIDER;
				}
				else if (aux == 12) {///if is a spinner
					file.ignore();
					file >> aux;
					file.ignore();
					file >> object.spinnerLength;
					object.type = SPINNER;
				}
				else
					object.type = CIRCLE;
				///set object type
				std::getline(file, content);
				/*object.setObjectType(content);*/
				///CIRCLE
				if (object.type == CIRCLE || object.type == SLIDER) {
					Vec2 initialPos = mousePos;
					Vec2 deltaPos = (object.pos - mousePos) / ((object.timer - timer2) / timeMultiplier);
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < (object.timer - firstTimer) / timeMultiplier) {
						Sleep(2);
						//float alpha = (object.timer - firstTimer);
						//mousePos = initialPos + (deltaPos*alpha);

						if (deltaPos.x > 0 && mousePos.x < object.pos.x) {
							mousePos += deltaPos * speed;
							moveMouse(mousePos);
						}
						else if (deltaPos.x < 0 && mousePos.x > object.pos.x) {
							mousePos += deltaPos * speed;
							moveMouse(mousePos);
						}
						else if (deltaPos.x == 0) {
							if (deltaPos.y > 0 && mousePos.y < object.pos.y) {
								mousePos += deltaPos * speed;
								moveMouse(mousePos);
							}
							else if (deltaPos.y < 0 && mousePos.y > object.pos.y) {
								mousePos += deltaPos * speed;
								moveMouse(mousePos);
							}
						}
						else
							moveMouse(object.pos);
					}
					timer2 = object.timer;
					mousePos = object.pos;
					moveMouse(mousePos);
					rightClick();
				}
				///SLIDER
				else if (object.type == SLIDER) {
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < (object.timer - firstTimer) / timeMultiplier) {

					}
					/*hold();
					std::deque<vec2> tmp;
					tmp.push_back({ 144,89 });
					tmp.push_back({ 271,88 });
					tmp.push_back({ 337,147 });
					tmp.push_back({ 215,243 });
					tmp.push_back({ 206,349 });
					tmp.push_back({ 474,228 });
					vec2 aux;
					for (float i = 0; i <= 1; i += 0.02f/((455/100)/1.4f)) {
						Sleep(20);
						aux = getBezierPoint2(tmp, i);
						moveMouse(aux.x*0.86 + 546, aux.y*0.86 + 273);
					}
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < object.timer - firstTimer + 455*10/1.4f + 20) {

					}
					release();*/
				}
				///SPINNER
				else if (object.type == SPINNER) {
					std::chrono::steady_clock::time_point temp = std::chrono::high_resolution_clock::now();
					int angle = 0;
					hold();
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < (object.spinnerLength - firstTimer) / timeMultiplier) {
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < (object.timer - firstTimer) / timeMultiplier) {

						}
						if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 2) {
							moveMouse(Vec2(256 + 50 * cos(angle * PI / 180), 192 + 50 * sin(angle * PI / 180)));
							temp = std::chrono::high_resolution_clock::now();
							angle += 15;
							angle = angle % 360;
						}
					}
					release();
					timer2 = object.spinnerLength;
					mousePos = object.pos;
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