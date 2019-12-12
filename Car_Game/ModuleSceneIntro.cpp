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

	


	for (int i = 0; i < 9; i++)
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
	
	for (int i = 0; i < 9; i++)
	{
		walls[i] = new Cube(50, 2, 2);
		App->physics->AddBody(*walls[i], mass);
		walls[i]->SetPos(0, 1, -10);
		walls[i]->color = Blue;
	}

	walls[1]->SetRotation(90, vec3(0, 1, 0));
	walls[2]->SetRotation(90, vec3(0, 1, 0));
	walls[1]->SetPos(27, 1, -10);
	


}

