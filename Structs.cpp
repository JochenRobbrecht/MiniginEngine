#include "MiniginPCH.h"
#include "Structs.h"

bool operator==(const Rectf& rect1, const Rectf& rect2)
{
	return (rect1.leftBottom.x == rect2.leftBottom.x && rect1.leftBottom.y == rect2.leftBottom.y 
		&& rect1.width == rect2.width && rect1.height == rect2.height);
}

Color4f::Color4f()
	:Color4f{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Color4f::Color4f(float r, float g, float b, float a)
	: r{ r }
	, g{ g }
	, b{ b }
	, a{ a }
{
}