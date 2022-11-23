#pragma once
#include "Circle.h"
#include "Slider.h"
#include "Spinner.h"

#include <fstream>
#include <sstream>

std::shared_ptr<Object> parseObject(const std::string& content)
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


// Constants
const double PI = 3.14159265;

std::ostream& operator<<(std::ostream& os, const Object::Type& ob) {
	if (ob == Object::CIRCLE)
		os << "circle";
	else if (ob == Object::SLIDER)
		os << "slider";
	else if (ob == Object::SPINNER)
		os << "spinner";
	else if (ob == Object::NOTHING)
		os << "nothing";
	os << '\n';
	return os;
}

Vec2 Bezier(const std::vector<Vec2>& points, const float& t) {
	std::vector<Vec2> tmp(points);
	Vec2 answer;
	for (int i = points.size() - 1; i > 0; i--) {
		for (int k = 0; k < i; k++)
			tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
	}
	return tmp[0];
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