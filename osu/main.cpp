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
	char aux;
	std::string song;
	std::string aux2 = "";
	Object object;
	//moveMouse(308,101);
	//moveMouse(1229,791);
	while (true) {
		///press numpad 0 to change to current song
		if (GetAsyncKeyState(VK_NUMPAD0)) {
			song = changeSong();
		}
		///press c to start
		if (GetAsyncKeyState(0x43)) {
			//starts reading the file
			std::ifstream file(song);
			while (aux2 != "[HitObjects]")
				std::getline(file, aux2);
			aux2 = "";
			std::noskipws(file);
			//starts reading the objects
			std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
			file >> x;
			file.ignore();
			file >> y;
			file.ignore();
			file >> firstTimer;
			aux = ' ';
			while (aux != '\n')
				file >> aux;
			while (file >> x || !GetAsyncKeyState(VK_ESCAPE)) {
				file.ignore();
				file >> y;
				file.ignore();
				file >> timer;

				while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < timer - firstTimer) {
					
				}
				//moveMouse((x*1.8) + 308, (y*1.8) + 101);
				moveMouse((x*0.86) + 546, (y*0.86) + 273);
				///press numpad 5 to stop
				if (GetAsyncKeyState(VK_NUMPAD5))
					break;
				aux = ' ';
				while (aux != '\n')
					file >> aux;
			}
			file.close();
		}
	}
	return 0;
}