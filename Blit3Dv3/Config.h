#pragma once

//enumerate the variables the config can set
enum class ConfigTags { DEADZONE, MOVE, STRAFE, YAW, PITCH };

//somewhere to read our data into
class Configuration
{
public:
	float deadzone = 0.50f;
	float axis_strafe = 1.5f;
	float axis_move = 1.5f;
	float axis_yaw = 1.5f;
	float axis_pitch = 1.5f;
};

bool LoadConfig(Configuration& config);
void DisplayConfig(Configuration& config);
