#pragma once
#include <Windows.h>
#include <chrono>
#include "Vector2.h"

const Vec2 TOP_LEFT_CORNER = Vec2(484, 197);
const Vec2 BOTTOM_RIGHT_CORNER = Vec2(1435, 910);
const Vec2 OSU_SREEN_SIZE = Vec2(512, 384);

void leftClick()
{
	INPUT Input = { 0 };
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

void rightClick()
{
	INPUT Input = { 0 };
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

void hold()
{
	INPUT Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
}

void release()
{
	INPUT Input = { 0 };
	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void moveMouse(Vec2 pos)
{
	// Resolution change TO-DO
	// pos.y = OSU_SREEN_SIZE.y - pos.y; // some mods invert map positions
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

void moveMouse(const Vec2& fromPos, const Vec2& toPos, int delay)
{
	if (delay < 0)
	{
		delay = 1;
	}
	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point temp = start;

	float alpha = 0.0f;
	while (alpha < 1.0f)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - temp).count() > 1)
		{
			temp = std::chrono::high_resolution_clock::now();
			auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
			alpha = (float)timePassed / (float)delay;
			moveMouse(fromPos + (toPos - fromPos) * alpha);
		}
	}

	// Correct to the end position just in case
	moveMouse(toPos);
}