#include "pch.h"

#include <tchar.h>

#include "rendering/application/WindowsApplication.h"
#include "systems/MainEngine.h"

#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <cstdarg>
#include <thread>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

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


int APIENTRY _tWinMain(const HINSTANCE hInstance,
                       HINSTANCE,
                       LPTSTR,
                       int)
{
	WindowsApplication application{hInstance};

	if (!application.Init())
		return EXIT_FAILURE;

	const auto dispositif = new PM3D::CDispositifD3D11{PM3D::CDS_FENETRE, application.GetMainWindow()};

	MainEngine mainEngine{dispositif, {WindowsApplication::ProcessWindowMessages}};

	mainEngine.Run();

	// Jolt

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

	PhysicsSystemWrapper physicsSystemInitialiser{ parameters };
	PhysicsSystem& physicsSystem = physicsSystemInitialiser.GetPhysicsSystem();

	BodyActivationListenerLogger bodyActivationListener;
	physicsSystem.SetBodyActivationListener(&bodyActivationListener);

	ContactListenerLogger contactListener;
	physicsSystem.SetContactListener(&contactListener);

	BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();

	BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded();

	ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	ShapeRefC floor_shape = floor_shape_result.Get();

	BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(),
		EMotionType::Static, Layers::NON_MOVING);

	Body* floor = bodyInterface.CreateBody(floor_settings);
	bodyInterface.AddBody(floor->GetID(), EActivation::DontActivate);

	constexpr float cDeltaTime = 1.0f / 60.0f;

	physicsSystem.OptimizeBroadPhase();

	while (true)
	{

	}

	return EXIT_SUCCESS;
}
