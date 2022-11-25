#pragma once
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

const double PI = 3.14159265;
#pragma region BEZIER
struct BezierStruct
{
	std::vector<Vec2> points;
	float t0;
	float t1;
	float length;

	BezierStruct(std::vector<Vec2> _points) : points(_points) {}
};

Vec2 Bezier(const std::vector<Vec2>& points, const float& t)
{
	std::vector<Vec2> tmp(points);
	Vec2 answer;
	for (int i = points.size() - 1; i > 0; i--)
	{
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
	}
	return tmp[0];
}

Vec2 ConcatenatedBezier(const std::vector<BezierStruct>& beziers, const float& t)
{
	for (int i = 0; i < beziers.size(); i++)
	{
		if (beziers[i].t1 > t)
		{
			float customT = (t - beziers[i].t0) / (beziers[i].t1 - beziers[i].t0);
			return Bezier(beziers[i].points, customT);
		}
	}
	float customT = (t - beziers.back().t0) / (beziers.back().t1 - beziers.back().t0);
	return Bezier(beziers.back().points, customT);
}

float CalculateBezierLength(const std::vector<Vec2>& points)
{
	float alpha = 0.0f;
	float distance = 0.0f;
	Vec2 previous = Bezier(points, alpha);
	while (alpha <= 1.0f)
	{
		alpha += 0.02f;
		Vec2 current = Bezier(points, alpha);
		distance += sqrt(pow(previous.x - current.x, 2) + pow(previous.y - current.y, 2));
		previous = current;
	}
	return distance;
}

std::vector<BezierStruct> PreCalculateBezier(const std::vector<Vec2>& points)
{
	// Calculate the different bezier curves
	std::vector<BezierStruct> beziers;
	std::vector<Vec2> aux;
	aux.push_back(points[0]);
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i] != points[i - 1])
			aux.push_back(points[i]);
		else
		{
			beziers.push_back(BezierStruct(aux));
			aux.clear();
			aux.push_back(points[i]);
		}
	}
	if (aux.size() > 1)
		beziers.push_back(BezierStruct(aux));

	// Assign their t0 and t1 based on their length
	float totalLength = 0.0f;
	for (int i = 0; i < beziers.size(); i++)
	{
		beziers[i].length = CalculateBezierLength(beziers[i].points);
		totalLength += beziers[i].length;
	}

	beziers[0].t0 = 0.0f;
	beziers[0].t1 = beziers[0].length / totalLength;
	for (int i = 1; i < beziers.size(); i++)
	{
		beziers[i].t0 = beziers[i-1].t1;
		beziers[i].t1 = beziers[i].t0 + beziers[i].length/totalLength;
	}

	return beziers;
}
#pragma endregion

#pragma region PASS-THROUGH
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

void PreCalculatePassThrough(const std::vector<Vec2>& points, Vec2& center, float& maxAngle)
{
	float radius;
	center = FindCircle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, radius);
	Vec2 a = points[0] - center;
	Vec2 b = points[1] - center;
	Vec2 c = points[2] - center;
	float dot = a.x * c.x + a.y * c.y;
	float calc = dot / pow(radius, 2);
	maxAngle = acos(max(-1.0f, min(calc, 1.0f))); // In radians
	// Rotate towards angle
	// Hack to check if it's the right angle or its the inverted angle (360-angle)
	float x = (a.x * cos(maxAngle)) - (a.y * sin(maxAngle));
	float y = (a.y * cos(maxAngle)) + (a.x * sin(maxAngle));
	float dist = sqrt(pow(x - c.x, 2) + pow(y - c.y, 2));
	if (dist < 5.0f)
	{
		float x2 = (a.x * cos(maxAngle * 0.98f)) - (a.y * sin(maxAngle * 0.98f));
		float y2 = (a.y * cos(maxAngle * 0.98f)) + (a.x * sin(maxAngle * 0.98f));
		float x3 = (a.x * cos((-2.0f * PI + maxAngle) * 0.98f)) - (a.y * sin((-2.0f * PI + maxAngle) * 0.98f));
		float y3 = (a.y * cos((-2.0f * PI + maxAngle) * 0.98f)) + (a.x * sin((-2.0f * PI + maxAngle) * 0.98f));
		// Calculate both distances to point B
		float dist2 = sqrt(pow(x2 - b.x, 2) + pow(y2 - b.y, 2));
		float dist3 = sqrt(pow(x3 - b.x, 2) + pow(y3 - b.y, 2));
		if (dist2 < dist3)
		{
			return;
		}
		maxAngle = -2.0f * PI + maxAngle;
		return;
	}
	float x2 = (a.x * cos(-maxAngle * 0.98f)) - (a.y * sin(-maxAngle * 0.98f));
	float y2 = (a.y * cos(-maxAngle * 0.98f)) + (a.x * sin(-maxAngle * 0.98f));
	float x3 = (a.x * cos((2.0f * PI - maxAngle) * 0.98f)) - (a.y * sin((2.0f * PI - maxAngle) * 0.98f));
	float y3 = (a.y * cos((2.0f * PI - maxAngle) * 0.98f)) + (a.x * sin((2.0f * PI - maxAngle) * 0.98f));
	// Calculate both distances to point B
	float dist2 = sqrt(pow(x2 - b.x, 2) + pow(y2 - b.y, 2));
	float dist3 = sqrt(pow(x3 - b.x, 2) + pow(y3 - b.y, 2));
	if (dist2 < dist3)
	{
		maxAngle = -maxAngle;
		return;
	}
	maxAngle = 2.0f * PI - maxAngle;
	return;
}

Vec2 PassThrough(const Vec2& firstPoint, const float& t, const Vec2& center, const float& maxAngle)
{
	Vec2 a = firstPoint - center;
	float angle = maxAngle * t;
	float x = (a.x * cos(angle)) - (a.y * sin(angle));
	float y = (a.y * cos(angle)) + (a.x * sin(angle));
	return Vec2(x, y) + center;
}
#pragma endregion

#pragma region LINEAR
Vec2 Linear(const std::vector<Vec2>& points, const float& t)
{
	return points.front() + (points.back() - points.front()) * t;
}
#pragma endregion

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