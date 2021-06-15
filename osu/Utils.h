#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <deque>
//limits -> 0, 0 = 546, 273
//limits - > 512, 384 = 989, 605

struct Vec2 {
	float x, y;
	Vec2(float _x, float _y) {
		x = _x;
		y = _y;
	}
	Vec2() {
		x = 0;
		y = 0;
	}

	Vec2 operator+(const Vec2 rhs) const{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2 operator-(const Vec2 rhs) const{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2 operator*(const Vec2 rhs) const {
		return Vec2(x * rhs.x, y * rhs.y);
	}
	Vec2 operator/(const Vec2 rhs) const {
		return Vec2(x / rhs.x, y / rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	friend Vec2 operator*(const float lhs, const Vec2 rhs) {
		return Vec2(rhs.x * lhs, rhs.y * lhs);
	}
	friend Vec2 operator*(const Vec2 lhs, const float rhs) {
		return rhs * lhs;
	}
	friend Vec2 operator/(const Vec2 lhs, const float rhs) {
		return Vec2(lhs.x / rhs, lhs.y / rhs);
	}
};

enum Type : short { CIRCLE, SLIDER, SPINNER, NOTHING };
struct Object {
	Type type;
	Vec2 pos;
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


// Constants
const float speed = 20.0f;//3.2f
const double PI = 3.14159265;
const Vec2 TOP_LEFT_CORNER = Vec2(501, 241);
const Vec2 BOTTOM_RIGHT_CORNER = Vec2(1034, 640);
const Vec2 OSU_SREEN_SIZE = Vec2(512, 384);

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

Vec2 getBezierPoint(Vec2* points, int numPoints, float t) {
	Vec2* tmp = new Vec2[numPoints];
	memcpy(tmp, points, numPoints * sizeof(Vec2));
	int i = numPoints - 1;
	while (i > 0) {
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
		i--;
	}
	Vec2 answer = tmp[0];
	delete[] tmp;
	return answer;
}
Vec2 getBezierPoint2(const std::deque<Vec2>& points, const float& t) {
	std::deque<Vec2> tmp(points);
	Vec2 answer;
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
void moveMouse(Vec2 pos) {
	// Resolution change TO-DO
	pos = (pos * (BOTTOM_RIGHT_CORNER - TOP_LEFT_CORNER) / OSU_SREEN_SIZE) + TOP_LEFT_CORNER;

	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = pos.x * (65535.0f / fScreenWidth);
	double fy = pos.y * (65535.0f / fScreenHeight);
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