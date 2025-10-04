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
	auto& bodyInterface = JoltSystem::GetBodyInterface();

	for (auto staticObject : staticObjects)
	{
		bodyInterface.RemoveBody(staticObject);
		bodyInterface.DestroyBody(staticObject);
	}

	if (currentBallIndex != 0)
	{
		bodyInterface.RemoveBody(currentBallId);
		bodyInterface.DestroyBody(currentBallId);
	}
}

void ConsoleBallScene::Run()
{
	uint frameCount = 0;
	bool wasSpacePressedLastFrame = false; // used to prevent ball skipping

	while (true)
	{
		const bool isSpacePressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
		const DWORD frameStartTime = GetTickCount();

		++frameCount;

		if (isSpacePressed && not wasSpacePressedLastFrame)
		{
			++currentBallIndex;

			DeletePreviousBall();
			SpawnNextBall();
		}

		wasSpacePressedLastFrame = isSpacePressed;

		if (frameCount % 30 == 0 && currentBallIndex >= 0) // 0.5s/frame if a ball has already been invoked
			DrawObject();

		UpdatePhysics();
		WaitBeforeNextFrame(frameStartTime);
	}
}

void ConsoleBallScene::DeletePreviousBall() const
{
	if (currentBallIndex != 0) [[likely]]
	{
		auto& bodyInterface = JoltSystem::GetBodyInterface();

		bodyInterface.RemoveBody(currentBallId);
		bodyInterface.DestroyBody(currentBallId);
	}
}

void ConsoleBallScene::SpawnNextBall()
{
	auto& bodyInterface = JoltSystem::GetBodyInterface();
	const BallInfos& currentBallInfo = ballInfos[currentBallIndex % ballInfos.size()];

	currentBallId = bodyInterface.CreateAndAddBody(currentBallInfo.settings, EActivation::Activate);
	bodyInterface.SetLinearVelocity(currentBallId, Vec3(0.5f, 0.0f, 0.0f));
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

	BodyID wallId = JoltSystem::GetBodyInterface().CreateAndAddBody(wallSettings, EActivation::DontActivate);
	BodyID wallSensorId = JoltSystem::GetBodyInterface().CreateAndAddBody(sensorSettings, EActivation::DontActivate);

	staticObjects.emplace_back(wallId);
	staticObjects.emplace_back(wallSensorId);
}

void ConsoleBallScene::DrawObject() const
{
	const RVec3 position = JoltSystem::GetBodyInterface().GetCenterOfMassPosition(currentBallId);
	const Vec3 velocity = JoltSystem::GetBodyInterface().GetLinearVelocity(currentBallId);
	const string_view objectName = ballInfos[currentBallIndex % ballInfos.size()].name;

	std::cout << objectName << ": Position = (" << position.GetX() << ", " << position.GetY() <<
		", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", "
		<< velocity.GetY() << ", " << velocity.GetZ() << ")" << '\n';
}

void ConsoleBallScene::UpdatePhysics()
{
	constexpr int collisionSteps = 1;
	JoltSystem::GetPhysicSystem().Update(PHYSICS_UPDATE_RATE, collisionSteps,
		&JoltSystem::GetTempAllocator(),
		&JoltSystem::GetJobSystem());

	for (auto& task : JoltSystem::GetPostStepCallbacks())
		task();

	JoltSystem::GetPostStepCallbacks().clear();
}

void ConsoleBallScene::WaitBeforeNextFrame(const DWORD frameStartTime)
{
	const DWORD frameEnd = GetTickCount();
	const DWORD frameDuration = frameEnd - frameStartTime;

	if (frameDuration < FRAME_TIME)
		Sleep(static_cast<DWORD>(FRAME_TIME - frameDuration));
}
