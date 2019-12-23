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


	vehicle->getChassisWorldTransform().getOpenGLMatrix(matrix);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	btVector3 offset_Chassis(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset_Chassis = offset_Chassis.rotate(q.getAxis(), q.getAngle());
	btVector3 offset_Cabin(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset_Cabin = offset_Cabin.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_right_pivot(info.chassis_offset.x + info.pivot.rPivot_offset.x, info.chassis_offset.y
		+ 1.5f, info.chassis_offset.z + info.pivot.rPivot_offset.z + 0.1f);
	offset_right_pivot = offset_right_pivot.rotate(q.getAxis(), q.getAngle());
	//btVector3 offset_lPivot(info.pivot.lPivot_offset.x, info.pivot.lPivot_offset.y, info.pivot.lPivot_offset.z);
	//offset_lPivot = offset_lPivot.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_right_paddle(info.chassis_offset.x , info.chassis_offset.y - 3.8f
		, info.chassis_offset.z + 1.0f );
	offset_right_paddle = offset_right_paddle.rotate(q.getAxis(), q.getAngle());
	//btVector3 offset_lPaddle(info.paddle.lPaddle_offset.x, info.paddle.lPaddle_offset.y, info.paddle.lPaddle_offset.z);
	//offset_lPaddle = offset_lPaddle.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset_Chassis.getX();
	chassis.transform.M[13] += offset_Chassis.getY();
	chassis.transform.M[14] += offset_Chassis.getZ();

	cabin.transform.M[12] += offset_Cabin.getX();
	cabin.transform.M[13] += offset_Cabin.getY();
	cabin.transform.M[14] += offset_Cabin.getZ();

	matrix[12] += offset_right_pivot.getX();
	matrix[13] += offset_right_pivot.getY();
	matrix[14] += offset_right_pivot.getZ();

	info.pivot.right_pivot->SetTransform(matrix);
	
	matrix[12] += offset_right_paddle.getX();
	matrix[13] += offset_right_paddle.getY();
	matrix[14] += offset_right_paddle.getZ();
	
	info.paddle.right_paddle->SetTransform(matrix);

	
	info.paddle.right_paddle->GetTransform(&info.rPaddle.transform);
	info.pivot.right_pivot->GetTransform(&info.rPivot.transform);


	chassis.color.Set(0.355f, 0.315f, 0.110f);
	cabin.color.Set(0.355f, 0.315f, 0.110f);
	info.rPivot.color.Set(0.10f, 0.10f, 0.11f);
	info.rPaddle.color.Set(0.10f, 0.10f, 0.11f);

	//lPivot.color.Set(0.10f, 0.10f, 0.11f);
	//lPaddle.color.Set(0.10f, 0.10f, 0.11f);


	info.rPaddle.Render();
	info.rPivot.Render();
	//lPaddle.Render();
	//lPivot.Render();
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

// ----------------------------------------------------------------------------

void PhysVehicle3D::Elevate_paddle(float velocity)
{
	info.paddle.sliderConstraint->setPoweredLinMotor(true);
	info.paddle.sliderConstraint->setMaxLinMotorForce(10000);
	info.paddle.sliderConstraint->setTargetLinMotorVelocity(velocity);
}