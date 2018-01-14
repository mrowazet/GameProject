#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class DevTestClass
{
public:
	virtual void sayHello() const;
	virtual int getInt() const;

	static constexpr int VALUE = 1;

private:
	sf::Vector2u m_point;
	b2PolygonShape m_box;
};