#pragma once
#include<mutex>
#include "Config.h"

extern std::mutex joystickMutex;

struct JoystickState
{
	int axisCount; //how many axes there are for this joystick...yes, "axes" is the plural of "axis", I know it looks funny
	const float* axisStates; //array of axisCount floats, between -1 and 1 in value
	int buttonCount; //how many buttons there are for this joystick
	const unsigned char* buttonStates; //array of buttonCount unsigned chars, will either be GLFW_PRESS or GLFW_RELEASE in value
};

class Joystick
{
public:
	bool foundJoystick = false;
	JoystickState joystickState;
	int joystickNumber;
	float leftJoystickX = 0.f; //turn wheels
	float leftJoystickY = 0.f;    //1
	float rightJoystickX = 0.f;//turn turret
	float rightJoystickY = 0.f;   //turn gun
	float rightBumper = 0.f;           //5
	float leftBumper = 0.f;			   //4

	void ProcessJoystickAxis(float& joystickAxis, Configuration configuration);
	bool PollJoystick();
	void DoJoystick(void);
	bool Init();
};