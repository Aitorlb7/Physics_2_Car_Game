#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	Create_walls();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;

	

	//Render:
	for (int i = 0; i < 4; i++)
	{
		walls[i]->Render();
	}
	
	p.Render();
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::Create_walls()
{
	float mass = 999999;
	
	//Creating walls
	for (int i = 0; i < 4; i++)
	{
		walls[i] = new Cube(70, 6, 2);
		walls[i]->color = Blue;
	}

	//Position of walls
	walls[1]->SetRotation(90, vec3(0, 1, 0));
	walls[2]->SetRotation(90, vec3(0, 1, 0));
	walls[0]->SetPos(0, 3, -20);
	walls[1]->SetPos(36, 3, 15);
	walls[2]->SetPos(-36, 3, 15);
	walls[3]->SetPos(0, 3, 50);

	//Give physics to walls
	for (int i = 0; i < 4; i++)
	{
		App->physics->AddBody(*walls[i], mass);	
	}
}

