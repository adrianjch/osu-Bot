#include "Utils.h"
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>

int main() {
	float x = 400;
	float y = 300;
	int timer = 0;
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
		///press c to start
		if (GetAsyncKeyState(0x43)) {
			firstTime = true;
			//starts reading the file
			std::ifstream file(song);
			while (content != "[HitObjects]")
				std::getline(file, content);
			content = "";
			std::noskipws(file);
			std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
			//starts reading the objects
			while (!file.eof()) {
				//set object coords+timer
				file >> object.x;
				if (file.eof())
					break;
				file.ignore();
				file >> object.y;
				file.ignore();
				file >> object.timer;
				if (firstTime) {
					firstTimer = object.timer;
					firstTime = false;
				}
				file.ignore();
				file >> aux;
				if (aux == 12) {
					file.ignore();
					file >> aux;
					file.ignore();
					file >> object.timer2;
				}
				//set object type
				std::getline(file, content);
				object.setObjectType(content);
				if (object.type == CIRCLE || object.type == SLIDER) {
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < object.timer - firstTimer) {

					}
					moveMouse((object.x*0.86) + 546, (object.y*0.86) + 273);
				}
				else if (object.type == SPINNER) {
					std::chrono::steady_clock::time_point temp = std::chrono::high_resolution_clock::now();
					int phase = 0;
					bool move = true;
					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < object.timer2 - firstTimer) {
						while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < object.timer - firstTimer) {

						}
						if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 30) {
							phase++;
							phase = phase % 4;
							temp = std::chrono::high_resolution_clock::now();
							move = true;
						}

						if (phase == 0 && move) {
							moveMouse((226 * 0.86) + 546, (162 * 0.86) + 273);
							move = false;
						}
						else if (phase == 1 && move) {
							moveMouse((286 * 0.86) + 546, (162 * 0.86) + 273);
							move = false;
						}
						else if (phase == 2 && move) {
							moveMouse((286 * 0.86) + 546, (222 * 0.86) + 273);
							move = false;
						}
						else if (phase == 3 && move) {
							moveMouse((226 * 0.86) + 546, (222 * 0.86) + 273);
							move = false;
						}
					}
				}
				//std::cout << object.type;
				///press numpad 5 to stop
				if (GetAsyncKeyState(VK_NUMPAD5))
					break;
			}
			file.close();
		}
	}
	return 0;
}