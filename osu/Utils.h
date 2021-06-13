#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <deque>
//limits -> 0, 0 = 546, 273
//limits - > 512, 384 = 989, 605

enum Type { CIRCLE, SLIDER, SPINNER, NOTHING };
struct Object {
	Type type;
	float x;
	float y;
	int timer;
	int spinnerLength;


	void setObjectType(const std::string& content) {
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

std::ostream& operator<<(std::ostream& os, const Type& ob) {
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

struct vec2 {
	float x, y;
	vec2(float _x, float _y) {
		x = _x;
		y = _y;
	}
	vec2() {
		x = 0;
		y = 0;
	}
};

vec2 operator+(vec2 a, vec2 b) {
	return vec2(a.x + b.x, a.y + b.y);
}

vec2 operator-(vec2 a, vec2 b) {
	return vec2(a.x - b.x, a.y - b.y);
}

vec2 operator*(float s, vec2 a) {
	return vec2(s * a.x, s * a.y);
}


vec2 getBezierPoint(vec2* points, int numPoints, float t) {
	vec2* tmp = new vec2[numPoints];
	memcpy(tmp, points, numPoints * sizeof(vec2));
	int i = numPoints - 1;
	while (i > 0) {
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
		i--;
	}
	vec2 answer = tmp[0];
	delete[] tmp;
	return answer;
}
vec2 getBezierPoint2(const std::deque<vec2>& points, const float& t) {
	std::deque<vec2> tmp(points);
	vec2 answer;
	for (int i = points.size() - 1; i > 0; i--) {
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
	}
	return tmp[0];
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
void hold() {
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

}
void release() {
	INPUT    Input = { 0 };
	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}
void moveMouse(int x, int y) {
	// Resolution change TO-DO
	x = (x * 0.86f) + 546;
	y = (y * 0.86f) + 273;

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
	std::ifstream songFile("D:/ppCounter/StreamCompanion/Files/osuFileLocation.txt");
	std::getline(songFile, song);
	for (int i = 0; i < song.size(); i++) {
		if (song[i] == '\\')
			song[i] = '/';
	}
	songFile.close();
	return song;
}

