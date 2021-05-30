#pragma once
#include "Structs.h"


class Transform final
{
public:
	const Point2f GetPosition() const;
	void SetPosition(float x, float y);
	void SetPosition(Point2f pos);
private:
	Point2f m_Position;
};

