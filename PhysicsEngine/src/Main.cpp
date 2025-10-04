#include "pch.h"

#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <cstdarg>
#include <thread>
#include <utility>

#include "BodyActivationListenerLogger.h"
#include "layers/BroadPhaseLayerInterfaceImpl.h"
#include "layers/BroadPhaseLayers.h"
#include "layers/Layers.h"
#include "layers/ObjectLayerPairFilterImpl.h"
#include "layers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "ContactListenerLogger.h"
#include "SphereFactory.h"
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

	BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	vector<function<void()>> postStepTasks;

	ContactListenerLogger contactListener{bodyInterface, postStepTasks};
	physicsSystem.SetContactListener(&contactListener);

	///
	///
	///

	BoxShapeSettings wallShapeSettings(Vec3(0.2f, 5.0f, 3.0f));

	ShapeSettings::ShapeResult wallShapeResult = wallShapeSettings.Create();
	ShapeRefC wallShape = wallShapeResult.Get();

	BodyCreationSettings wallSettings(wallShape, RVec3(2.0_r, 0.0_r, 0.0_r),
	                                  Quat::sRotation(Vec3::sAxisX(), JPH_PI / 2.0f),
	                                  EMotionType::Static, Layers::NON_MOVING);

	BodyCreationSettings sensorSettings(wallShape, RVec3(2.0_r, 0.0_r, 0.0_r),
	                                    Quat::sRotation(Vec3::sAxisX(), JPH_PI / 2.0f),
	                                    EMotionType::Static, Layers::SENSOR);

	sensorSettings.mIsSensor = true;

	wallSettings.mFriction = 0.0f;
	wallSettings.mRestitution = 1.0f;

	BodyID wallId = bodyInterface.CreateAndAddBody(wallSettings, EActivation::DontActivate);
	BodyID wallSensorId = bodyInterface.CreateAndAddBody(sensorSettings, EActivation::DontActivate);

	///
	///
	///

	using BallCreator = function<BodyID(BodyInterface&)>;

	struct ObjectDebug
	{
		string name;
		BallCreator creator;
	};

	vector<ObjectDebug> objects
	{
		{.name = "Ghost  Ball", .creator = SphereFactory::CreateGhostBall},
		{.name = "Normal Ball", .creator = SphereFactory::CreateNormalBall},
		{.name = "Icarus Ball", .creator = SphereFactory::CreateIcarusBall},
		{.name = "Slow   Ball", .creator = SphereFactory::CreateSlowGhostBall},
	};


	///
	///
	///

	constexpr double PHYSICS_UPDATE_RATE = 1.0f / 60.0f;
	constexpr double TARGET_FPS = 60.0;
	constexpr double FRAME_TIME = 1000.0 / TARGET_FPS;

	physicsSystem.OptimizeBroadPhase();

	uint step = 0;

	const auto displayObject = [&](const string_view sphereName, const BodyID bodyId)
	{
		const RVec3 position = bodyInterface.GetCenterOfMassPosition(bodyId);
		const Vec3 velocity = bodyInterface.GetLinearVelocity(bodyId);
		cout << sphereName << ": Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() <<
			", " <<
			position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.
			GetZ() << ")" << '\n';
	};

	DWORD lastSpaceTrigger = 0;
	bool prevSpacePressed = false;
	vector<pair<string, BodyID>> scene;

	while (true)
	{
		DWORD frameStart = GetTickCount();

		++step;

		bool isSpacePressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
		DWORD now = GetTickCount();

		if (isSpacePressed && !prevSpacePressed && now - lastSpaceTrigger >= 1000)
		{
			if (!scene.empty())
			{
				auto previousBall = scene.back();
				bodyInterface.RemoveBody(previousBall.second);
				bodyInterface.DestroyBody(previousBall.second);
				scene.pop_back();
			}

			ObjectDebug objectDebug = objects.back();
			BodyID object = objectDebug.creator(bodyInterface);
			objects.pop_back();

			scene.emplace_back(objectDebug.name, object);
			bodyInterface.SetLinearVelocity(object, Vec3(0.5f, 0.0f, 0.0f));

			lastSpaceTrigger = now;
		}

		prevSpacePressed = isSpacePressed;

		if (step % 30 == 0)
		{
			for (const auto& object : scene)
				displayObject(object.first, object.second);
		}

		constexpr int cCollisionSteps = 1;
		physicsSystem.Update(PHYSICS_UPDATE_RATE, cCollisionSteps,
		                     &JoltSystem::GetTempAllocator(),
		                     &JoltSystem::GetJobSystem());

		for (auto& task : postStepTasks)
			task();

		postStepTasks.clear();

		DWORD frameEnd = GetTickCount();
		DWORD frameDuration = frameEnd - frameStart;

		if (frameDuration < FRAME_TIME)
			Sleep(static_cast<DWORD>(FRAME_TIME - frameDuration));
	}

	//bodyInterface.RemoveBody(floor->GetID());
	//bodyInterface.DestroyBody(floor->GetID());

	return 0;
}
