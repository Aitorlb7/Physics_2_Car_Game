#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModulePhysics3D.h"

#define MAX_SNAKE 2
#define MASS 10000

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void Create_walls();
	void Create_Door();
	void Create_Obstacles();
	void Create_Sensors();
public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Cube* obstacle[6];

	Cube* walls[8];
	Cube* Floor;
	Cube* Aux;
	Cube* Aux2;

	Cube* rightGate;
	Cube* leftGate;

	PhysBody3D* rightGate_body;
	PhysBody3D* leftGate_body;

	btHingeConstraint* right_hinge;
	btHingeConstraint* left_hinge;
};
