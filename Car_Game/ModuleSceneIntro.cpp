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
	Cube chasis;
	chasis.size = { vec3(2.5f,2.5f,2.5f) };
	App->physics->AddBody(chasis, 1.0f);

	Cylinder front_R_wheel;
	front_R_wheel.SetPos(3, 0, 0);
	front_R_wheel.height = 0.5f;
	front_R_wheel.radius = 0.7f;
	front_R_wheel.SetRotation(90.0f, vec3(3,3,3));

	App->physics->AddBody(front_R_wheel, 1.0f);


	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

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
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

