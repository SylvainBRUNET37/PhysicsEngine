#ifndef JOLT_INITIALISER_H
#define JOLT_INITIALISER_H

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>

#include <thread>
#include <Jolt/Physics/PhysicsSettings.h>

#include "utils/Utils.h"

class JoltSystem
{
public:
	JoltSystem(const JoltSystem&) = delete;
	JoltSystem& operator=(const JoltSystem&) = delete;
	JoltSystem(JoltSystem&&) = delete;
	JoltSystem& operator=(JoltSystem&&) = delete;

	static JPH::TempAllocatorImpl& GetTempAllocator()
	{
		static JPH::TempAllocatorImpl tempAllocator{TEMP_ALLOCATOR_SIZE};

		return tempAllocator;
	}

	static JPH::JobSystemThreadPool& GetJobSystem()
	{
		static JPH::JobSystemThreadPool jobSystem
		{
			JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int>(std::thread::hardware_concurrency() - 1)
		};

		return jobSystem;
	}

private:
	static constexpr size_t TEMP_ALLOCATOR_SIZE_MB = 10;
	static constexpr size_t TEMP_ALLOCATOR_SIZE = TEMP_ALLOCATOR_SIZE_MB * 1024 * 1024;

	static JoltSystem joltSystem;

	JoltSystem()
	{
		JPH::RegisterDefaultAllocator();

		JPH::Trace = Utils::TraceImpl;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = Utils::AssertFailedImpl;)

		JPH::Factory::sInstance = new JPH::Factory();

		JPH::RegisterTypes();
	}

	~JoltSystem()
	{
		JPH::UnregisterTypes();
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}
};

inline JoltSystem JoltSystem::joltSystem{};

#endif
