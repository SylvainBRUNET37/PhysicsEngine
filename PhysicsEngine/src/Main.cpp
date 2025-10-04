#include "pch.h"

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <cstdarg>
#include <thread>
#include <utility>

#include "layers/BroadPhaseLayerInterfaceImpl.h"
#include "layers/BroadPhaseLayers.h"
#include "layers/ObjectLayerPairFilterImpl.h"
#include "layers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "SphereFactory.h"
#include "listeners/BodyActivationListenerLogger.h"
#include "utils/Utils.h"
#include "listeners/ContactListenerLogger.h"
#include "scenes/ConsoleBallScene.h"

JPH_SUPPRESS_WARNINGS // Disable common warnings triggered by Jolt

using namespace JPH;
using namespace JPH::literals;
using namespace std;

int main()
{
	const shared_ptr<PhysicsSystem> physicsSystem{new PhysicsSystem{}};
	physicsSystem->Init(1024, 0, 1024, 1024,
	                    BroadPhaseLayerInterfaceImpl{}, ObjectVsBroadPhaseLayerFilterImpl{},
	                    ObjectLayerPairFilterImpl{});

	PhysicsSettings settings = physicsSystem->GetPhysicsSettings();
	settings.mMinVelocityForRestitution = 0.01f;
	physicsSystem->SetPhysicsSettings(settings);
	physicsSystem->SetGravity(Vec3::sZero());

	BodyInterface& bodyInterface = physicsSystem->GetBodyInterface();

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem->SetBodyActivationListener(&bodyActivationListener);

	const shared_ptr<vector<function<void()>>> postStepTasks{new vector<function<void()>>{}};

	ContactListenerLogger contactListener{bodyInterface, postStepTasks};
	physicsSystem->SetContactListener(&contactListener);

	const vector<ConsoleBallScene::BallInfos> balls
	{
		{"Normal Ball:", SphereFactory::GetNormalBallSettings()},
		{"Ghost Ball :", SphereFactory::GetGoalBallSettings()},
		{"Icarus Ball:", SphereFactory::GetIcarusBallSettings()},
		{"Slow Ball  :", SphereFactory::GetSlowGhostBallSettings()},
	};

	ConsoleBallScene consoleScene{physicsSystem, balls, postStepTasks};
	consoleScene.Run();

	return EXIT_SUCCESS;
}
