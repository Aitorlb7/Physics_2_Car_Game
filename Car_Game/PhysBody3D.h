#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;

enum SensorType
{
	None = 0,
	End,
	Obstacle,
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);

	void CreateSensor();

private:
	

public:
	btRigidBody* body = nullptr;
	p2List<Module*> collision_listeners;
	SensorType type = None;
};

#endif // __PhysBody3D_H__