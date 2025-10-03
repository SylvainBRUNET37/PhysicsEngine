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

#include "BodyActivationListenerLogger.h"
#include "layers/BroadPhaseLayerInterfaceImpl.h"
#include "layers/BroadPhaseLayers.h"
#include "layers/Layers.h"
#include "layers/ObjectLayerPairFilterImpl.h"
#include "layers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "ContactListenerLogger.h"
#include "systems/JoltSystem.h"
#include "systems/PhysicsSystemWrapper.h"
#include "systems/PhysicsSystemParameters.h"
#include "utils/Utils.h"

JPH_SUPPRESS_WARNINGS // Disable common warnings triggered by Jolt

using namespace JPH;
using namespace JPH::literals;
using namespace std;

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

	// Now create a dynamic body to bounce on the floor
	// Note that this uses the shorthand version of creating and adding a body to the world
	BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(),
	                                     EMotionType::Dynamic, Layers::MOVING);
	BodyID sphere_id = bodyInterface.CreateAndAddBody(sphere_settings, EActivation::Activate);

	// Create cube
	BodyCreationSettings cube_settings(new BoxShape(Vec3Arg(0.5f, 0.5f, 0.5f)), RVec3(0.0_r, 5.0_r, 0.0_r),
	                                   Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
	BodyID cube_id = bodyInterface.CreateAndAddBody(cube_settings, EActivation::Activate);
	bodyInterface.SetLinearVelocity(cube_id, Vec3(0.0f, -5.0f, 0.0f));

	// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
	// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
	bodyInterface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
	constexpr float cDeltaTime = 1.0f / 60.0f;

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	physicsSystem.OptimizeBroadPhase();

	// Now we're ready to simulate the body, keep simulating until it goes to sleep
	uint step = 0;
	while (bodyInterface.IsActive(sphere_id))
	{
		// Next step
		++step;

		// Output current position and velocity of the sphere
		RVec3 position = bodyInterface.GetCenterOfMassPosition(sphere_id);
		Vec3 velocity = bodyInterface.GetLinearVelocity(sphere_id);
		cout << "Sphere: Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " <<
			position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.
			GetZ() << ")" << endl;

		RVec3 box_pos = bodyInterface.GetCenterOfMassPosition(cube_id);
		Vec3 box_velocity = bodyInterface.GetLinearVelocity(cube_id);
		cout << "Box   : Step " << step << ": Position = (" << box_pos.GetX() << ", " << box_pos.GetY() << ", " <<
			box_pos.GetZ() << "), Velocity = (" << box_velocity.GetX() << ", " << box_velocity.GetY() << ", " <<
			box_velocity.GetZ() << ")" << endl;

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		constexpr int cCollisionSteps = 1;

		// Step the world
		physicsSystem.Update(cDeltaTime, cCollisionSteps, &JoltSystem::GetTempAllocator(),
		                      &JoltSystem::GetJobSystem());
	}

	// Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
	bodyInterface.RemoveBody(sphere_id);

	// Destroy the sphere. After this the sphere ID is no longer valid.
	bodyInterface.DestroyBody(sphere_id);

	// Remove and destroy the floor
	bodyInterface.RemoveBody(floor->GetID());
	bodyInterface.DestroyBody(floor->GetID());

	return 0;
}
