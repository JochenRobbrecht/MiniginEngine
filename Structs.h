#pragma once

struct Point2f
{
	float x;
	float y;
};
struct Rectf
{
	Point2f leftBottom;
	float width;
	float height;
};
bool operator==(const Rectf& rect1, const Rectf& rect2);
struct Color4f
{
	Color4f();
	explicit Color4f(float r, float g, float b, float a);

	float r;
	float g;
	float b;
	float a;
};