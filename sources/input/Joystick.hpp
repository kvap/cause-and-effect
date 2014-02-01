#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

class Joystick
{
private:
	Joystick() { }
	~Joystick() { }
public:
	static void updateEvents();
};

#endif // JOYSTICK_HPP
