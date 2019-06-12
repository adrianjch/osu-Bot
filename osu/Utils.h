#pragma once
#include<Windows.h>
#include <string>
#include <fstream>
//limits -> 0, 0 = 546, 273
//limits - > 512, 384 = 989, 605

enum Type{CIRCLE, SLIDER, SPINNER, NOTHING};
struct Object {
	Type type;
	float x;
	float y;
	int timer;
	int timer2;
	

	void setObjectType(const std::string &content) {
		type = NOTHING;
		int counter = 0;
		for (int i = 0; i < content.size(); i++) {
			if (content[i] == '|') {
				type = SLIDER;
				break;
			}
			if (content[i] == ',') {
				counter++;
				//break;
			}
		}
		if (type == NOTHING) {
			if (counter == 2)
				type = CIRCLE;
			else if (counter == 1)
				type = SPINNER;

			/*if (osuversion == 3) {
				if (counter == 2)
					type = CIRCLE;
				else if (counter == 0)
					type = SPINNER;
			}*/

			/*if (osuversion == 5 || osuversion == 6 || osuversion == 7 || osuversion == 8 || osuversion == 9) {
				if (counter == 1)
					type = CIRCLE;
				else if (counter == 0)
					type = SPINNER;
			}*/

			/*if (osuversion == 10 || osuversion == 11 || osuversion == 12 || osuversion == 13 || osuversion == 14) {
				if (counter == 2)
					type = CIRCLE;
				else if (counter == 1)
					type = SPINNER;
			}*/
		}
	}
};

std::ostream& operator<<(std::ostream &os, const Type &ob) {
	if (ob == CIRCLE)
		os << "circle";
	else if (ob == SLIDER)
		os << "slider";
	else if (ob == SPINNER)
		os << "spinner";
	else if (ob == NOTHING)
		os << "nothing";
	os << '\n';
	return os;
}

void leftClick() {
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}
void rightClick() {
	INPUT    Input = { 0 };
	// right down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}
void moveMouse(int x, int y) {
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = x * (65535.0f / fScreenWidth);
	double fy = y * (65535.0f / fScreenHeight);
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}

std::string changeSong() {
	std::string song;
	std::ifstream songFile("D:/Juegos/StreamCompanion/Files/osuFileLocation.txt");
	std::getline(songFile, song);
	for (int i = 0; i < song.size(); i++) {
		if (song[i] == 92)
			song[i] = '/';
	}
	songFile.close();
	return song;
}