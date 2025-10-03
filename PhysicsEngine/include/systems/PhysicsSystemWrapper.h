#ifndef PHYSICS_SYSTEM_INITIALISER_H
#define PHYSICS_SYSTEM_INITIALISER_H

#include <Jolt/Physics/PhysicsSystem.h>

#include "PhysicsSystemParameters.h"

template
<
	class BroadPhaseLayerInterface,
	class ObjectVsBroadPhaseLayerFilter,
	class ObjectLayerPairFilter
>
class PhysicsSystemWrapper
{
public:
	using PhysicsParams = PhysicsSystemParameters
	<
		BroadPhaseLayerInterface,
		ObjectVsBroadPhaseLayerFilter,
		ObjectLayerPairFilter
	>;

	PhysicsSystemWrapper() = delete;
	PhysicsSystemWrapper(const PhysicsSystemWrapper&) = delete;
	PhysicsSystemWrapper& operator=(const PhysicsSystemWrapper&) = delete;
	PhysicsSystemWrapper(PhysicsSystemWrapper&&) = delete;
	PhysicsSystemWrapper& operator=(PhysicsSystemWrapper&&) = delete;
	~PhysicsSystemWrapper() = default;

	explicit PhysicsSystemWrapper(const PhysicsParams& parmeters)
	{
		physicsSystem.Init
		(
			parmeters.maxBodies,
			parmeters.numBodyMutexes,
			parmeters.maxBodies,
			parmeters.maxContactConstraints,
			parmeters.broadPhaseLayerInterface,
			parmeters.objectVsBroadphaseLayerFilter,
			parmeters.objectVsObjectLayerFilter
		);
	}

	JPH::PhysicsSystem& GetPhysicsSystem()
	{
		return physicsSystem;
	}

private:
	JPH::PhysicsSystem physicsSystem{};
};

#endif
