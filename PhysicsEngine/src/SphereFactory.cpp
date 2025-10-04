#include "pch.h"
#include "SphereFactory.h"

#include <Jolt/Physics/EActivation.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include "layers/Layers.h"

using namespace JPH;
using namespace JPH::literals;

BodyID SphereFactory::CreateNormalBall(BodyInterface& bodyInterface)
{
	BodyCreationSettings sphereSettings(
		new SphereShape(0.5f), 
		RVec3(0.0_r, 0.0_r, 0.0_r), 
		Quat::sIdentity(),
		EMotionType::Dynamic, 
		Layers::MOVING
	);

	sphereSettings.mRestitution = 0.6f;
	sphereSettings.mFriction = 0.0f;

	return bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);
}

BodyID SphereFactory::CreateGhostBall(BodyInterface& bodyInterface)
{
	BodyCreationSettings sphereSettings(
		new SphereShape(0.5f),
		RVec3(0.0_r, 0.0_r, 0.0_r),
		Quat::sIdentity(),
		EMotionType::Dynamic,
		Layers::GHOST
	);

	sphereSettings.mRestitution = 0.0f;
	sphereSettings.mFriction = 0.0f;

	return bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);
}

BodyID SphereFactory::CreateIcarusBall(BodyInterface& bodyInterface)
{
	BodyCreationSettings sphereSettings(
		new SphereShape(0.5f),
		RVec3(0.0_r, 0.0_r, 0.0_r),
		Quat::sIdentity(),
		EMotionType::Dynamic,
		Layers::ICARUS
	);

	return bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);
}

BodyID SphereFactory::CreateSlowGhostBall(BodyInterface& bodyInterface)
{
	BodyCreationSettings sphereSettings(
		new SphereShape(0.5f),
		RVec3(0.0_r, 0.0_r, 0.0_r),
		Quat::sIdentity(),
		EMotionType::Dynamic,
		Layers::SLOW_GHOST
	);

	return bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);
}
