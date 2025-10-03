#include "pch.h"

#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <cstdarg>
#include <thread>

#include "physics/utils/BodyActivationListenerLogger.h"
#include "physics/layers/BroadPhaseLayerInterfaceImpl.h"
#include "physics/layers/BroadPhaseLayers.h"
#include "physics/layers/Layers.h"
#include "physics/layers/ObjectLayerPairFilterImpl.h"
#include "physics/layers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "physics/utils/ContactListenerLogger.h"
#include "physics/systems/JoltSystem.h"
#include "physics/systems/PhysicsSystemWrapper.h"
#include "physics/systems/PhysicsSystemParameters.h"
#include "physics/utils/JoltUtils.h"

JPH_SUPPRESS_WARNINGS // Disable common warnings triggered by Jolt

using namespace JPH;
using namespace JPH::literals;
using namespace std;

/*
int main()
{
	const PhysicsSystemParameters<BroadPhaseLayerInterfaceImpl, ObjectVsBroadPhaseLayerFilterImpl,
	                              ObjectLayerPairFilterImpl>
		parameters
		{
			.maxBodies = 1024,
			.numBodyMutexes = 0,
			.maxBodyPairs = 1024,
			.maxContactConstraints = 1024,

			.broadPhaseLayerInterface{},
			.objectVsBroadphaseLayerFilter{},
			.objectVsObjectLayerFilter{}
		};

	PhysicsSystemWrapper physicsSystemInitialiser{parameters};
	PhysicsSystem& physicsSystem = physicsSystemInitialiser.GetPhysicsSystem();

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	ContactListenerLogger contactListener;
	physicsSystem.SetContactListener(&contactListener);

	BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();

	BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded();

	// Create the shape
	ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	ShapeRefC floor_shape = floor_shape_result.Get();

	// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
	BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(),
	                                    EMotionType::Static, Layers::NON_MOVING);

	// Create the actual rigid body
	Body* floor = bodyInterface.CreateBody(floor_settings);
	// Note that if we run out of bodies this can return nullptr

	// Add it to the world
	bodyInterface.AddBody(floor->GetID(), EActivation::DontActivate);

	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
	constexpr float cDeltaTime = 1.0f / 60.0f;

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	physicsSystem.OptimizeBroadPhase();

	// Now we're ready to simulate the body, keep simulating until it goes to sleep
	uint step = 0;
	while (true)
	{
		
	}

	return EXIT_SUCCESS;
}
*/