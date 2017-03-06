#pragma once

#include "Transform.h"
#include "Collider.h"

namespace base
{

class Rigidbody
{
public:
	float drag, mass;
	vec2 impulse, force, acceleration, velocity; // defaults to 0's
	
	float angularDrag;
	float spin, torque, angularAcceleration, angularVelocity;

	Rigidbody(float a_mass = 1)
				: spin(0), torque(0), angularAcceleration(0), angularVelocity(0),
				  drag(0), mass(a_mass), angularDrag(0) {}


	void addForce(const vec2 &a_force)	 { force += a_force; }
	void addImpulse(const vec2 &a_impulse) { impulse += a_impulse; }

	void addTorque(float a_torque) { torque += a_torque; }
	void addSpin(float a_spin)	   { spin += a_spin; }

	void integrate(Transform *T, float dt)
	{
		acceleration += force / mass;
		velocity += acceleration * dt + impulse / mass;
		T->setGlobalPosition(T->getGlobalPosition() + velocity * dt);
		acceleration = -velocity * drag;
		impulse = force = vec2{ 0,0 };

		angularAcceleration += torque / mass;
		angularVelocity += angularAcceleration * dt + spin / mass;
		T->setGlobalAngle(T->getGlobalAngle() + angularVelocity * dt);
		angularAcceleration = -angularVelocity * angularDrag;
		spin = torque = 0;


	}

	void draw(const Transform *T, const mat3 &cam)
	{
		auto glob = cam * T->getGlobalTransform();
		vec2 tvel = (cam * vec3(velocity, 0)).xy;
		vec2 tacc = (cam * vec3(acceleration, 0)).xy;

		debugDrawLine(glob.c3.xy, glob.c3.xy+tvel, 0xff00ffff);
		debugDrawLine(glob.c3.xy, glob.c3.xy+tacc, 0x00ffffff);
	}
};

	
inline void DynamicResolution(const collision &cd, Transform *AT, Rigidbody *AR, Transform *BT, Rigidbody *BR, float bounciness = 1)
{
	vec2 MTV = cd.penetration * cd.normal;
	
	float am = (AR->mass * AR->velocity).magnitude();
	float bm = (BR->mass * BR->velocity).magnitude();
	float cm = am + bm;							 

	AT->setGlobalPosition(AT->getGlobalPosition() + MTV * (1 - am / cm));
	BT->setGlobalPosition(BT->getGlobalPosition() - MTV * (1 - bm / cm));

	vec2  A = AR->velocity;	
	float P = AR->mass;		
	vec2  X;				

	vec2  B = BR->velocity;
	float Q = BR->mass;	
	vec2  Y;			

	float E = bounciness;

	X = (A*P + B*Q + -E*(A - B)*Q) / (Q + P);
	Y = E*(A - B) + X;

	AR->velocity = X;
	BR->velocity = Y;
}


inline void StaticResolution(const collision &cd, Transform *AT, Rigidbody *AR, float bounciness = 1)
{
	vec2 MTV = cd.penetration * cd.normal;
	AT->setGlobalPosition(AT->getGlobalPosition() - MTV);

	AR->velocity = reflect(AR->velocity, cd.normal) * bounciness;
}


}