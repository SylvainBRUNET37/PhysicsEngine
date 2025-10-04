#ifndef SPHERE_FACTORY_H
#define SPHERE_FACTORY_H

#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

namespace SphereFactory
{
	JPH::BodyID CreateNormalBall(JPH::BodyInterface& bodyInterface);
	JPH::BodyID CreateGhostBall(JPH::BodyInterface& bodyInterface);
	JPH::BodyID CreateIcarusBall(JPH::BodyInterface& bodyInterface);
	JPH::BodyID CreateSlowGhostBall(JPH::BodyInterface& bodyInterface);
}

#endif
