#ifndef SPHERE_FACTORY_H
#define SPHERE_FACTORY_H

#include <Jolt/Physics/Body/BodyInterface.h>

namespace SphereFactory
{
	JPH::BodyCreationSettings GetNormalBallSettings();
	JPH::BodyCreationSettings GetGhostBallSettings();
	JPH::BodyCreationSettings GetIcarusBallSettings();
	JPH::BodyCreationSettings GetSlowGhostBallSettings();
}

#endif
