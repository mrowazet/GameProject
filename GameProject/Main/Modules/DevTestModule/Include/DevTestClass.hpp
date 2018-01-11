#pragma once

class DevTestClass
{
public:
	virtual void sayHello() const;
	virtual int getInt() const;

	static constexpr int VALUE = 1;
};