#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"

#include "Bullet/include/btBulletCollisionCommon.h"

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
	Cube floor(1000, 0.05, 1000);
	floor.color.Set(0.635f, 0.635f, 0.635f);
	floor.Render();

	//Render:
	for (int i = 0; i < 5; i++)
	{
		walls[i]->Render();
	}
	
	rightGate_body->GetTransform(&rightGate->transform);
	leftGate_body->GetTransform(&leftGate->transform);
	p.Render();
	rightGate->Render();
	leftGate->Render();
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
	//---------------Right Gate-----------------
	rightGate = new Cube(14.f, 30.f, 2.f);
	rightGate->SetPos(-7.0f, 15.f, 50.f);
	rightGate->color = Red;

	rightGate_body = App->physics->AddBody(*rightGate, 10.f);
	const btVector3 btPivotA(-11.0f, 15.f, 0.f);
	const btVector3 btAxisA(0.0f, 1.0f, 0.0f);
	btHingeConstraint* right_hinge = new btHingeConstraint(*rightGate_body->body, btPivotA, btAxisA);
	right_hinge->setLimit(-SIMD_PI * 0.35f, SIMD_PI * 0.35f);
	App->physics->world->addConstraint(right_hinge);
	right_hinge->setDbgDrawSize(btScalar(5.f));


	//---------------Left Gate-----------------
	leftGate = new Cube(14.f, 30.f, 2.f);
	leftGate->SetPos(7.0f, 15.f, 50.f);
	leftGate->color = Red;

	leftGate_body = App->physics->AddBody(*leftGate, 10.f);
	const btVector3 btPivotB(11.0f, 15.f, 0.f);
	const btVector3 btAxisB(0.0f, 1.0f, 0.0f);
	btHingeConstraint* left_hinge = new btHingeConstraint(*leftGate_body->body, btPivotB, btAxisB);
	left_hinge->setLimit(-SIMD_PI * 0.35f, SIMD_PI * 0.35f);
	App->physics->world->addConstraint(left_hinge);
	left_hinge->setDbgDrawSize(btScalar(5.f));
}

