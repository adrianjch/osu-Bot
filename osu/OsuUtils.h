#pragma once
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

const double PI = 3.14159265;

Vec2 Bezier(const std::vector<Vec2>& points, const float& t) {
	std::vector<Vec2> tmp(points);
	Vec2 answer;
	for (int i = points.size() - 1; i > 0; i--) {
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
	}
	return tmp[0];
}

Vec2 FindCircle(int x1, int y1, int x2, int y2, int x3, int y3, float& radius)
{
	int x12 = x1 - x2;
	int x13 = x1 - x3;

	int y12 = y1 - y2;
	int y13 = y1 - y3;

	int y31 = y3 - y1;
	int y21 = y2 - y1;

	int x31 = x3 - x1;
	int x21 = x2 - x1;

	// x1^2 - x3^2
	int sx13 = pow(x1, 2) - pow(x3, 2);

	// y1^2 - y3^2
	int sy13 = pow(y1, 2) - pow(y3, 2);

	int sx21 = pow(x2, 2) - pow(x1, 2);
	int sy21 = pow(y2, 2) - pow(y1, 2);

	int f = ((sx13) * (x12)
		+(sy13) * (x12)
		+(sx21) * (x13)
		+(sy21) * (x13))
		/ (2 * ((y31) * (x12)-(y21) * (x13)));
	int g = ((sx13) * (y12)
		+(sy13) * (y12)
		+(sx21) * (y13)
		+(sy21) * (y13))
		/ (2 * ((x31) * (y12)-(x21) * (y13)));

	int c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

	// eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
	// where centre is (h = -g, k = -f) and radius r
	// as r^2 = h^2 + k^2 - c
	int h = -g;
	int k = -f;
	int sqr_of_r = h * h + k * k - c;

	// r is the radius
	float r = sqrt(sqr_of_r);
	radius = r;
	return Vec2(h, k);
}

Vec2 PassThrough(const std::vector<Vec2>& points, const float& t)
{
	float radius;
	Vec2 center = FindCircle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, radius);
	Vec2 a = points[0] - center;
	Vec2 b = points[1] - center;
	Vec2 c = points[2] - center;
	float dot = a.x * c.x + a.y * c.y;
	float calc = dot / pow(radius, 2);
	float maxAngle = acos(max(-1.0f, min(calc, 1.0f))); // In radians
	// Rotate towards angle
	// Hack to check if it's the right angle or its the inverted angle (360-angle)
	float x = (a.x * cos(maxAngle)) - (a.y * sin(maxAngle));
	float y = (a.y * cos(maxAngle)) + (a.x * sin(maxAngle));
	if (sqrt(pow(x - c.x, 2) + pow(y - c.y, 2)) < 5.0f)
	{
		float x2 = (a.x * cos(maxAngle*0.98f)) - (a.y * sin(maxAngle*0.98f));
		float y2 = (a.y * cos(maxAngle*0.98f)) + (a.x * sin(maxAngle*0.98f));
		float x3 = (a.x * cos((-2.0f * PI + maxAngle)*0.98f)) - (a.y * sin((-2.0f * PI + maxAngle)*0.98f));
		float y3 = (a.y * cos((-2.0f * PI + maxAngle)*0.98f)) + (a.x * sin((-2.0f * PI + maxAngle)*0.98f));
		// Calculate both distances to point B
		float dist2 = sqrt(pow(x2 - b.x, 2) + pow(y2 - b.y, 2));
		float dist3 = sqrt(pow(x3 - b.x, 2) + pow(y3 - b.y, 2));
		if (dist2 < dist3)
		{
			float angle = maxAngle * t;
			float x4 = (a.x * cos(angle)) - (a.y * sin(angle));
			float y4 = (a.y * cos(angle)) + (a.x * sin(angle));
			return Vec2(x4, y4) + center;
		}
		float angle = (-2.0f*PI + maxAngle) * t;
		float x4 = (a.x * cos(angle)) - (a.y * sin(angle));
		float y4 = (a.y * cos(angle)) + (a.x * sin(angle));
		return Vec2(x4, y4) + center;
	}
	float x2 = (a.x * cos(-maxAngle*0.98f)) - (a.y * sin(-maxAngle*0.98f));
	float y2 = (a.y * cos(-maxAngle*0.98f)) + (a.x * sin(-maxAngle*0.98f));
	float x3 = (a.x * cos((2.0f * PI - maxAngle)*0.98f)) - (a.y * sin((2.0f * PI - maxAngle)*0.98f));
	float y3 = (a.y * cos((2.0f * PI - maxAngle)*0.98f)) + (a.x * sin((2.0f * PI - maxAngle)*0.98f));
	// Calculate both distances to point B
	float dist2 = sqrt(pow(x2 - b.x, 2) + pow(y2 - b.y, 2));
	float dist3 = sqrt(pow(x3 - b.x, 2) + pow(y3 - b.y, 2));
	if (dist2 < dist3)
	{
		float angle = -maxAngle * t;
		float x4 = (a.x * cos(angle)) - (a.y * sin(angle));
		float y4 = (a.y * cos(angle)) + (a.x * sin(angle));
		return Vec2(x4, y4) + center;
	}
	float angle = (2.0f * PI - maxAngle) * t;
	float x4 = (a.x * cos(angle)) - (a.y * sin(angle));
	float y4 = (a.y * cos(angle)) + (a.x * sin(angle));
	return Vec2(x4, y4) + center;
}

Vec2 Linear(const std::vector<Vec2>& points, const float& t)
{
	return points.front() + (points.back() - points.front()) * t;
}

std::string GetSong() {
	std::string song;
	std::ifstream songFile("E:/StreamCompanion/Files/song_path.txt");
	std::getline(songFile, song);
	for (int i = 0; i < song.size(); i++) {
		if (song[i] == '\\')
			song[i] = '/';
	}
	songFile.close();
	return song;
}