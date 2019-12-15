#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	enabled = true;
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;


	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1.5f, 3.5f);
	car.chassis_offset.Set(0, 1.5, 0);
	car.cabin_size.Set(2, 2.5f, 1.5f);
	car.cabin_offset.Set(0, 2.5f, 1);
	car.pivot.rPivot_size.Set(0.3f, 4, 0.15f);
	car.pivot.rPivot_offset.Set(-0.5f, 2.2f, 1.83f);
	car.pivot.lPivot_size.Set(0.3f, 4, 0.15f);
	car.pivot.lPivot_offset.Set(0.5f, 2.2f, 1.83f);
	car.paddle.rPaddle_size.Set(0.3f, 0.15f, 1.8f);
	car.paddle.rPaddle_offset.Set(-0.5f,0.25f,2.65f);
	car.paddle.lPaddle_size.Set(0.3f, 0.15f, 1.8f);
	car.paddle.lPaddle_offset.Set(0.5f, 0.25f, 2.65f);


	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.6f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 0);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->camera->free_camera == false)
	{
		if (App->camera->rotate_camera == false)
		{
			App->camera->Position.x = vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 15.0f * vehicle->vehicle->getForwardVector().getX();
			App->camera->Position.y = vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 10.5f * vehicle->vehicle->getUpAxis();
			App->camera->Position.z = vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 12.5f * vehicle->vehicle->getForwardVector().getZ();
			float x = vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 20 * vehicle->vehicle->getForwardVector().getX();
			float z = vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 20 * vehicle->vehicle->getForwardVector().getZ();
			App->camera->LookAt(vec3(x, 0, z));
		}	
	}	

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
	{
		vehicle->Elevate_paddle(-2.0f);
	}

	if (can_sprint == true)
	{
		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
		{
			sprinting = true;
			sprint_timer.Start();
		}
	}

// Doesn't work------------------
	vehicle->info.paddle.right_paddle->GetTransform(&vehicle->info.rPaddle->transform);
	vehicle->info.pivot.right_pivot->GetTransform(&vehicle->info.rPivot->transform);
//--------------------------------

	PowerUpSystem();
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::PowerUpSystem()
{

		//SPRINT:
	if (sprinting == true)
	{
		if (sprint_timer.ReadSec() < 1.0f)
		{
			acceleration += 9999;
		}
		else
		{
			sprint_timer.Reset();
			sprinting = false;
		/*	can_sprint = false;*/
		}
	}


}



