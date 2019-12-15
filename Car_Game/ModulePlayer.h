#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 800.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 600.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void PowerUpSystem();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	bool sprinting = false;
	bool can_sprint = true;
	Timer sprint_timer;
};