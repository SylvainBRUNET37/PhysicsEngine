#ifndef CONSOLE_SCENE_H
#define CONSOLE_SCENE_H

#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "systems/JoltSystem.h"

class ConsoleBallScene
{
public:
	struct BallInfos
	{
		std::string_view name;
		JPH::BodyCreationSettings settings;
	};

	~ConsoleBallScene();
	ConsoleBallScene(const ConsoleBallScene&) = delete;
	ConsoleBallScene& operator=(const ConsoleBallScene&) = delete;
	ConsoleBallScene(ConsoleBallScene&&) = delete;
	ConsoleBallScene& operator=(ConsoleBallScene&&) = delete;

	explicit ConsoleBallScene(const std::vector<BallInfos>& ballInfos)
		: ballInfos{ballInfos}
	{
		CreateStaticScene();
	}

	void Run();

private:
	static constexpr double PHYSICS_UPDATE_RATE = 1.0f / 60.0f;
	static constexpr double TARGET_FPS = 60.0;
	static constexpr double FRAME_TIME = 1000.0 / TARGET_FPS;

	std::vector<BallInfos> ballInfos;
	std::vector<JPH::BodyID> staticObjects;

	JPH::BodyID currentBallId;
	int currentBallIndex = -1;

	void CreateStaticScene();
	void DeletePreviousBall() const;
	void SpawnNextBall();
	void DrawCurrentBall() const;
	static void UpdatePhysics();

	static void WaitBeforeNextFrame(DWORD frameStartTime);
};

#endif
