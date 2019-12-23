#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

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
	App->audio->PlayMusic("music/MK8_RB.wav");
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));






	Create_Sensors();
	Create_walls();
	Create_Door();
	Create_Obstacles();
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



	rightGate_body->GetTransform(&rightGate->transform);
	leftGate_body->GetTransform(&leftGate->transform);


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{
		right_hinge->enableAngularMotor(true, -1.0f, 100.0f);
		left_hinge->enableAngularMotor(true, 1.0f, 100.0f);
	}


	//Render:
	for (int i = 0; i < 8; i++)
	{
		walls[i]->Render();
		if (i < 6)
		{
			obstacle[i]->Render();
		}
	}

	p.Render();
	//Aux->Render();
	rightGate->Render();
	leftGate->Render();
	Floor->Render();
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->type == End) 
	{
		right_hinge->enableAngularMotor(true, -1.0f, 100.0f);
		left_hinge->enableAngularMotor(true, 1.0f, 100.0f);
	}
	if (body1->type == Win) {
		App->player->vehicle->SetPos(0, 0, 0);
		App->player->brake = BRAKE_POWER;
	}
}

void ModuleSceneIntro::Create_walls()
{
	// Create floor
	Floor = new Cube(500, 0.1f, 500);
	Floor->color.Set(0.635f, 0.635f, 0.635f);;

	//Creating walls
	for (int i = 0; i < 8; i++)
	{		
		if ((i == 3) || (i == 4))
		{
			walls[i] = new Cube(35, 30, 2);
		}
		if(i < 3)
		{
			walls[i] = new Cube(200, 30, 2);
		}
		if (i >= 5)
		{
			walls[i] = new Cube(70, 30, 2);
		}
		
		walls[i]->color = Blue;
	}

	//Position of walls
	walls[1]->SetRotation(90, vec3(0, 1, 0));
	walls[2]->SetRotation(90, vec3(0, 1, 0));
	walls[0]->SetPos(0, 15, -20);
	walls[1]->SetPos(50, 15, 80);
	walls[2]->SetPos(-50, 15, 80);
	walls[3]->SetPos(32, 15, 180);
	walls[4]->SetPos(-32, 15, 180);
	walls[5]->SetPos(15, 15, 30);
	walls[6]->SetPos(-15, 15, 60);
	walls[7]->SetPos(15, 15, 90);


	//Give physics to walls
	for (int i = 0; i < 8; i++)
	{
		App->physics->AddBody(*walls[i], MASS * 100);
	}
}
void ModuleSceneIntro::Create_Door()
{
	//---------------Right Gate-----------------
	rightGate = new Cube(14.f, 30.f, 2.f);
	rightGate->SetPos(-7.0f, 15.f, 180.0f);
	rightGate->color = Red;

	rightGate_body = App->physics->AddBody(*rightGate, 10.f);
	rightGate_body->body->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 btPivotA(-11.0f, 15.f, 0.f);
	const btVector3 btAxisA(0.0f, 1.0f, 0.0f);
	right_hinge = new btHingeConstraint(*rightGate_body->body, btPivotA, btAxisA);
	right_hinge->setLimit(-SIMD_PI * 0.35f, SIMD_PI * 0.35f);
	App->physics->world->addConstraint(right_hinge);
	right_hinge->setDbgDrawSize(btScalar(2.f));

	//---------------Left Gate-----------------
	leftGate = new Cube(14.f, 30.f, 2.f);
	leftGate->SetPos(7.0f, 15.f, 180.0f);
	leftGate->color = Red;

	leftGate_body = App->physics->AddBody(*leftGate, 10.f);
	leftGate_body->body->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 btPivotB(11.0f, 15.f, 0.f);
	const btVector3 btAxisB(0.0f, 1.0f, 0.0f);
	left_hinge = new btHingeConstraint(*leftGate_body->body, btPivotB, btAxisB);
	left_hinge->setLimit(-SIMD_PI * 0.35f, SIMD_PI * 0.35f);
	App->physics->world->addConstraint(left_hinge); 
	left_hinge->setDbgDrawSize(btScalar(2.f));
}

void ModuleSceneIntro::Create_Obstacles()
{

	//Create obstacles
	for (int i = 0; i < 6; i++)
	{
		obstacle[i] = new Cube(5,4,5);
		obstacle[i]->color = Red;
	}

	//Position

	obstacle[0]->SetPos(-20, 2, 40);
	obstacle[1]->SetPos(5, 2, 45);
	obstacle[2]->SetPos(25, 2, 50);
	obstacle[3]->SetPos(-20, 2, 70);
	obstacle[4]->SetPos(5, 2, 75);
	obstacle[5]->SetPos(25, 2, 80);

	//Physic

	for (int i = 0; i < 6; i++)
	{
		App->physics->AddBody(*obstacle[i], MASS * 100);
	}

}

void ModuleSceneIntro::Create_Sensors()
{
	// End sensor------------------
	Cube sensor_win(80.f, 0.1f, 5.f);
	sensor_win.SetPos(0, 1.6f, 200.f);
	PhysBody3D* auxbody2 = App->physics->AddBody(sensor_win, 0, Win);
	auxbody2->CreateSensor();
	auxbody2->collision_listeners.add(this);
	
	Cube sensor_end (80.f, 0.1f, 5.f);
	sensor_end.SetPos(0,1.6f, 150.f);
	PhysBody3D* auxbody = App->physics->AddBody(sensor_end, 0,End);
	auxbody->CreateSensor();
	auxbody->collision_listeners.add(this);


}

