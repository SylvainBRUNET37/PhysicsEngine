#include "pch.h"
#include "scenes/ConsoleBallScene.h"

#include <Jolt/Physics/EActivation.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include "layers/Layers.h"
#include "systems/JoltSystem.h"

using namespace JPH;
using namespace JPH::literals;

ConsoleBallScene::~ConsoleBallScene()
{
	auto& bodyInterface = physicsSystem->GetBodyInterface();

	for (auto staticObject : staticObjects)
		bodyInterface.RemoveBody(staticObject);
}

void ConsoleBallScene::Run()
{
	constexpr double PHYSICS_UPDATE_RATE = 1.0f / 60.0f;
	constexpr double TARGET_FPS = 60.0;
	constexpr double FRAME_TIME = 1000.0 / TARGET_FPS;

	DWORD lastSpaceTrigger = 0;
	bool prevSpacePressed = false;
	uint step = 0;

	while (true)
	{
		constexpr int cCollisionSteps = 1;
		const DWORD frameStart = GetTickCount();
		const bool isSpacePressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
		const DWORD now = GetTickCount();

		++step;

		if (isSpacePressed && !prevSpacePressed && now - lastSpaceTrigger >= 1000)
		{
			auto& bodyInterface = physicsSystem->GetBodyInterface();

			++currentBallIndex;

			if (currentBallIndex != 0) [[likely]]
			{
				bodyInterface.RemoveBody(currentBallId);
				bodyInterface.DestroyBody(currentBallId);
			}

			const BallInfos& currentBallInfo = ballInfos[currentBallIndex % ballInfos.size()];
			currentBallId = bodyInterface.CreateAndAddBody(currentBallInfo.settings, EActivation::Activate);
			bodyInterface.SetLinearVelocity(currentBallId, Vec3(0.5f, 0.0f, 0.0f));

			lastSpaceTrigger = now;
		}

		prevSpacePressed = isSpacePressed;

		if (step % 30 == 0 && currentBallIndex >= 0) [[likely]]
		{
			DrawObject();
		}

		physicsSystem->Update(PHYSICS_UPDATE_RATE, cCollisionSteps,
		                      &JoltSystem::GetTempAllocator(),
		                      &JoltSystem::GetJobSystem());

		for (auto& task : *postStepTasks)
			task();

		postStepTasks->clear();

		const DWORD frameEnd = GetTickCount();
		const DWORD frameDuration = frameEnd - frameStart;

		if (frameDuration < FRAME_TIME)
			Sleep(static_cast<DWORD>(FRAME_TIME - frameDuration));
	}
}

void ConsoleBallScene::CreateStaticScene()
{
	const BoxShapeSettings wallShapeSettings(Vec3(0.2f, 5.0f, 3.0f));

	const ShapeSettings::ShapeResult wallShapeResult = wallShapeSettings.Create();
	const ShapeRefC wallShape = wallShapeResult.Get();

	BodyCreationSettings wallSettings(wallShape, RVec3(2.0_r, 0.0_r, 0.0_r),
	                                  Quat::sRotation(Vec3::sAxisX(), JPH_PI / 2.0f),
	                                  EMotionType::Static, Layers::NON_MOVING);

	wallSettings.mFriction = 0.0f;
	wallSettings.mRestitution = 1.0f;

	BodyCreationSettings sensorSettings(wallShape, RVec3(2.0_r, 0.0_r, 0.0_r),
	                                    Quat::sRotation(Vec3::sAxisX(), JPH_PI / 2.0f),
	                                    EMotionType::Static, Layers::SENSOR);

	sensorSettings.mIsSensor = true;

	BodyID wallId = physicsSystem->GetBodyInterface().CreateAndAddBody(wallSettings, EActivation::DontActivate);
	BodyID wallSensorId = physicsSystem->GetBodyInterface().CreateAndAddBody(sensorSettings, EActivation::DontActivate);

	staticObjects.emplace_back(wallId);
	staticObjects.emplace_back(wallSensorId);
}

void ConsoleBallScene::DrawObject() const
{
	const RVec3 position = physicsSystem->GetBodyInterface().GetCenterOfMassPosition(currentBallId);
	const Vec3 velocity = physicsSystem->GetBodyInterface().GetLinearVelocity(currentBallId);
	const string_view objectName = ballInfos[currentBallIndex % ballInfos.size()].name;

	std::cout << objectName << ": Position = (" << position.GetX() << ", " << position.GetY() <<
		", " <<
		position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.
		GetZ() << ")" << '\n';
}
