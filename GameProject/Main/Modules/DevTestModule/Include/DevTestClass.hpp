#pragma once
#include <SFML\Graphics.hpp>

class DevTestClass
{
public:
	virtual void sayHello() const;
	virtual int getInt() const;

	static constexpr int VALUE = 1;

private:
	sf::Vector2u point;
};