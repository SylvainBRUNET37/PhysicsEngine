#include "pch.h"

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <cstdarg>
#include <utility>

#include "layers/BroadPhaseLayerInterfaceImpl.h"
#include "layers/BroadPhaseLayers.h"
#include "layers/ObjectLayerPairFilterImpl.h"
#include "layers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "SphereFactory.h"
#include "listeners/BodyActivationListenerLogger.h"
#include "utils/Utils.h"
#include "listeners/ContactListenerImpl.h"
#include "scenes/ConsoleBallScene.h"
#include "systems/JoltSystem.h"

JPH_SUPPRESS_WARNINGS // Disable common warnings triggered by Jolt

using namespace JPH;
using namespace JPH::literals;
using namespace std;

int main()
{
	JoltSystem::Init();
	auto& physicsSystem = JoltSystem::GetPhysicSystem();

	const BroadPhaseLayerInterfaceImpl broadPhaseLayerInterface;
	const ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
	const ObjectLayerPairFilterImpl objectLayerPairFilter;
	physicsSystem.Init(1024, 0, 1024, 1024,
		broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);

	PhysicsSettings settings = physicsSystem.GetPhysicsSettings();
	settings.mMinVelocityForRestitution = 0.01f;
	physicsSystem.SetPhysicsSettings(settings);
	physicsSystem.SetGravity(Vec3::sZero());

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	ContactListenerImpl contactListener;
	physicsSystem.SetContactListener(&contactListener);

	const vector<ConsoleBallScene::BallInfos> balls
	{
		{"Normal Ball", SphereFactory::GetNormalBallSettings()},
		{"Ghost Ball ", SphereFactory::GetGhostBallSettings()},
		{"Icarus Ball", SphereFactory::GetIcarusBallSettings()},
		{"Slow Ball  ", SphereFactory::GetSlowGhostBallSettings()},
	};

	cout << "Appuyez sur [ESPACE] pour lancer une balle" << '\n';
	ConsoleBallScene consoleScene{balls};
	consoleScene.Run();

	return EXIT_SUCCESS;
}
