#pragma once
struct Vec2
{
	float x, y;
	Vec2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	Vec2()
	{
		x = 0;
		y = 0;
	}

	Vec2 operator+(const Vec2 rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2 operator-(const Vec2 rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2 operator*(const Vec2 rhs) const
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}
	Vec2 operator/(const Vec2 rhs) const
	{
		return Vec2(x / rhs.x, y / rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	bool operator==(const Vec2 rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vec2 rhs) const
	{
		return !(x == rhs.x && y == rhs.y);
	}

	friend Vec2 operator*(const float lhs, const Vec2 rhs)
	{
		return Vec2(rhs.x * lhs, rhs.y * lhs);
	}
	friend Vec2 operator*(const Vec2 lhs, const float rhs)
	{
		return rhs * lhs;
	}
	friend Vec2 operator/(const Vec2 lhs, const float rhs)
	{
		return Vec2(lhs.x / rhs, lhs.y / rhs);
	}
};