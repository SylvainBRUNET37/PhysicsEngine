#include "pch.h"
#include "systems/JoltSystem.h"

#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>

#include "utils/Utils.h"

using namespace JPH;

void JoltSystem::Init()
{
	RegisterDefaultAllocator();

	Trace = Utils::TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = Utils::AssertFailedImpl;)

	Factory::sInstance = new Factory();

	RegisterTypes();

	physicsEngine = std::make_shared<PhysicsEngine::element_type>();
}

JoltSystem::~JoltSystem()
{
	UnregisterTypes();
	delete Factory::sInstance;
	Factory::sInstance = nullptr;
}
