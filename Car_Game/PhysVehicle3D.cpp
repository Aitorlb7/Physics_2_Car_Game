#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "ModulePhysics3D.h"
#include "Application.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color.Set(0.57f,0.61f,0.66f);

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z); 
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	//----------------Axis for paddles--------------------------
	Cube rPivot(info.pivot.rPivot_size.x, info.pivot.rPivot_size.y, info.pivot.rPivot_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rPivot.transform);
	Cube lPivot(info.pivot.lPivot_size.x, info.pivot.lPivot_size.y, info.pivot.lPivot_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&lPivot.transform);
	//--------------------Paddles--------------------------
	Cube rPaddle(info.paddle.rPaddle_size.x, info.paddle.rPaddle_size.y, info.paddle.rPaddle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rPaddle.transform);
	Cube lPaddle(info.paddle.lPaddle_size.x, info.paddle.lPaddle_size.y, info.paddle.lPaddle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&lPaddle.transform);

	//-------------------Slider Constraint--------------------------
	

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	btVector3 offset_Chassis(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset_Chassis = offset_Chassis.rotate(q.getAxis(), q.getAngle());
	btVector3 offset_Cabin(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset_Cabin = offset_Cabin.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_rPivot(info.pivot.rPivot_offset.x, info.pivot.rPivot_offset.y, info.pivot.rPivot_offset.z);
	offset_rPivot = offset_rPivot.rotate(q.getAxis(), q.getAngle());
	btVector3 offset_lPivot(info.pivot.lPivot_offset.x, info.pivot.lPivot_offset.y, info.pivot.lPivot_offset.z);
	offset_lPivot = offset_lPivot.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_rPaddle(info.paddle.rPaddle_offset.x, info.paddle.rPaddle_offset.y, info.paddle.rPaddle_offset.z);
	offset_rPaddle = offset_rPaddle.rotate(q.getAxis(), q.getAngle());
	btVector3 offset_lPaddle(info.paddle.lPaddle_offset.x, info.paddle.lPaddle_offset.y, info.paddle.lPaddle_offset.z);
	offset_lPaddle = offset_lPaddle.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset_Chassis.getX();
	chassis.transform.M[13] += offset_Chassis.getY();
	chassis.transform.M[14] += offset_Chassis.getZ();

	cabin.transform.M[12] += offset_Cabin.getX();
	cabin.transform.M[13] += offset_Cabin.getY();
	cabin.transform.M[14] += offset_Cabin.getZ();

	rPivot.transform.M[12] += offset_rPivot.getX();
	rPivot.transform.M[13] += offset_rPivot.getY();
	rPivot.transform.M[14] += offset_rPivot.getZ();

	lPivot.transform.M[12] += offset_lPivot.getX();
	lPivot.transform.M[13] += offset_lPivot.getY();
	lPivot.transform.M[14] += offset_lPivot.getZ();

	rPaddle.transform.M[12] += offset_rPaddle.getX();
	rPaddle.transform.M[13] += offset_rPaddle.getY();
	rPaddle.transform.M[14] += offset_rPaddle.getZ();

	lPaddle.transform.M[12] += offset_lPaddle.getX();
	lPaddle.transform.M[13] += offset_lPaddle.getY();
	lPaddle.transform.M[14] += offset_lPaddle.getZ();

	chassis.color.Set(0.355f, 0.315f, 0.110f);
	cabin.color.Set(0.355f, 0.315f, 0.110f);
	rPivot.color.Set(0.f, 0.f, 0.f);
	lPivot.color.Set(0.f, 0.f, 0.f);
	rPaddle.color.Set(0.f, 0.f, 0.f);
	lPaddle.color.Set(0.f, 0.f, 0.f);


	rPaddle.Render();
	lPaddle.Render();
	rPivot.Render();
	lPivot.Render();
	chassis.Render();
	cabin.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}