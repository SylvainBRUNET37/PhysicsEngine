#ifndef CONSOLE_SCENE_H
#define CONSOLE_SCENE_H

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyID.h>

#include <memory>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

class ConsoleBallScene
{
public:
	struct BallInfos
	{
		std::string_view name;
		JPH::BodyCreationSettings settings;
	};

	~ConsoleBallScene();

	explicit ConsoleBallScene(const std::shared_ptr<JPH::PhysicsSystem>& physicsSystem,
	                      const std::vector<BallInfos>& ballInfos,
	                      const std::shared_ptr<std::vector<std::function<void()>>>& postStepTasks)
		: physicsSystem{physicsSystem},
		  postStepTasks{postStepTasks},
		  ballInfos{ballInfos}
	{
		CreateStaticScene();
	}

	void Run();

private:
	std::shared_ptr<JPH::PhysicsSystem> physicsSystem;
	std::shared_ptr<std::vector<std::function<void()>>> postStepTasks;

	std::vector<BallInfos> ballInfos;
	std::vector<JPH::BodyID> staticObjects;

	JPH::BodyID currentBallId;
	int currentBallIndex = -1;

	void CreateStaticScene();
	void DrawObject() const;
};

#endif	
