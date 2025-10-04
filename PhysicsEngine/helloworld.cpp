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
	PhysicsSettings settings = physicsSystem.GetPhysicsSettings();
	settings.mMinVelocityForRestitution = 0.01f;
	physicsSystem.SetPhysicsSettings(settings);
	physicsSystem.SetGravity(Vec3::sZero());

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	ContactListenerLogger contactListener;
	physicsSystem.SetContactListener(&contactListener);

	BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();

	///
	///
	///

	BoxShapeSettings wallShapeSettings(Vec3(0.2f, 5.0f, 3.0f));

	ShapeSettings::ShapeResult wallShapeResult = wallShapeSettings.Create();
	ShapeRefC wallShape = wallShapeResult.Get();

	BodyCreationSettings wallSettings(wallShape, RVec3(2.0_r, 0.0_r, 0.0_r), Quat::sRotation(Vec3::sAxisX(), JPH_PI / 2.0f),
	                                    EMotionType::Static, Layers::NON_MOVING);
	wallSettings.mFriction = 0.0f;
	wallSettings.mRestitution = 1.0f;

	Body* floor = bodyInterface.CreateBody(wallSettings);

	bodyInterface.AddBody(floor->GetID(), EActivation::DontActivate);

	///
	///
	///

	BodyCreationSettings sphereSettings(new SphereShape(0.5f), RVec3(0.0_r, 0.0_r, 0.0_r), Quat::sIdentity(),
	                                     EMotionType::Dynamic, Layers::MOVING);

	sphereSettings.mRestitution = 0.6f;
	sphereSettings.mFriction = 0.0f;

	BodyID sphere_id = bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);

	bodyInterface.SetLinearVelocity(sphere_id, Vec3(0.5f, 0.0f, 0.0f));
	const auto shape = bodyInterface.GetTransformedShape(sphere_id);
	

	///
	///
	///
	
	constexpr float cDeltaTime = 1.0f / 60.0f;

	physicsSystem.OptimizeBroadPhase();

	uint step = 0;
	while (bodyInterface.IsActive(sphere_id))
	{
		++step;

		if (step % 30 == 0)
		{
			// Output current position and velocity of the sphere
			RVec3 position = bodyInterface.GetCenterOfMassPosition(sphere_id);
			Vec3 velocity = bodyInterface.GetLinearVelocity(sphere_id);
			cout << "Sphere: Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " <<
				position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.
				GetZ() << ")" << '\n';
		}

		constexpr int cCollisionSteps = 1;

		physicsSystem.Update(cDeltaTime, cCollisionSteps, &JoltSystem::GetTempAllocator(),
		                      &JoltSystem::GetJobSystem());
	}

	bodyInterface.RemoveBody(sphere_id);
	bodyInterface.DestroyBody(sphere_id);

	bodyInterface.RemoveBody(floor->GetID());
	bodyInterface.DestroyBody(floor->GetID());

	return 0;
}
