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
	Create_Door();
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
	for (int i = 0; i < 5; i++)
	{
		walls[i]->Render();
	}
	
	Auxbody->GetTransform(&right_gate->transform);
	Auxbody2->GetTransform(&right_joint->transform);
	p.Render();
	right_joint->Render();
	left_joint->Render();
	right_gate->Render();
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::Create_walls()
{
	//Creating walls
	for (int i = 0; i < 5; i++)
	{
		walls[i] = new Cube(70, 30, 2);
		walls[i]->color = Blue;
	}

	//Position of walls
	walls[1]->SetRotation(90, vec3(0, 1, 0));
	walls[2]->SetRotation(90, vec3(0, 1, 0));
	walls[0]->SetPos(0, 15, -20);
	walls[1]->SetPos(36, 15, 15);
	walls[2]->SetPos(-36, 15, 15);
	walls[3]->SetPos(50, 15, 50);
	walls[4]->SetPos(-50, 15, 50);

	//Give physics to walls
	for (int i = 0; i < 5; i++)
	{
		App->physics->AddBody(*walls[i], MASS);
	}
}
void ModuleSceneIntro::Create_Door()
{
	right_joint = new Cylinder(2, 30);
	right_joint->color = Red;
	right_joint->SetPos(-10, 15, 50);
	right_joint->SetRotation(90, vec3(0, 0, 1));

	left_joint = new Cylinder(2, 30);
	left_joint->color = Blue;
	left_joint->SetPos(13, 15, 50);
	left_joint->SetRotation(90, vec3(0, 0, 1));
	//App->physics->AddBody(*left_joint, MASS);

	right_gate = new Cube(5, 30, 2);
	right_gate->SetPos(0, 15, 50);
	right_gate->color = Red;
	//App->physics->AddBody(*right_gate, MASS);

	Auxbody = App->physics->AddBody(*right_gate, 1.0f);
	Auxbody2 = App->physics->AddBody(*right_joint, 1.0f);
	btHingeConstraint* right_hinge = App->physics->AddConstraintHinge(*Auxbody, *Auxbody2, vec3{ 0,0,0 }, vec3{ 0, 0,0 }, vec3{ 1,0,0 }, vec3{ 0,0,1 }, true);

}

