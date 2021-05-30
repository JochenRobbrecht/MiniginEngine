#include "MiniginPCH.h"
#include "Transform.h"

const Point2f Transform::GetPosition() const
{
	return m_Position;
}

void Transform::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void Transform::SetPosition(Point2f pos)
{
	m_Position = pos;
}
