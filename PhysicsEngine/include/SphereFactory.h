#pragma once

#include <Jolt/Physics/Collision/Shape/SphereShape.h>

namespace SphereFactory
{
	JPH::SphereShape CreateNormalBall()
	{
		static JPH::SphereShape settings;

		settings.mDensity = 5;

		return JPH::SphereShape(settings);
	}

	JPH::SphereShape CreateGhostBall()
	{
		static JPH::SphereShape settings;

		settings.mDensity = 5;

		return JPH::SphereShape(settings);
	}
}