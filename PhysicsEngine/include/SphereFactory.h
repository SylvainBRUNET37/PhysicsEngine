#pragma once

#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

namespace SphereFactory
{
	JPH::BodyID CreateNormalBall(JPH::BodyInterface& bodyInterface);
	JPH::BodyID CreateGhostBall(JPH::BodyInterface& bodyInterface);
}