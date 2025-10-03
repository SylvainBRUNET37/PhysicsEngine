#ifndef PHYSICS_SYSTEM_PARAMETERS_H
#define PHYSICS_SYSTEM_PARAMETERS_H

#include <type_traits>

template<typename Derived, typename BaseClass>
concept DerivedFrom = std::is_base_of_v<BaseClass, Derived>;

template
<
	DerivedFrom<JPH::BroadPhaseLayerInterface> BroadPhaseLayerInterface,
	DerivedFrom<JPH::ObjectVsBroadPhaseLayerFilter> ObjectVsBroadPhaseLayerFilter,
	DerivedFrom<JPH::ObjectLayerPairFilter> ObjectLayerPairFilter
>
struct PhysicsSystemParameters
{
	JPH::uint maxBodies;
	JPH::uint numBodyMutexes;
	JPH::uint maxBodyPairs;
	JPH::uint maxContactConstraints;

	BroadPhaseLayerInterface broadPhaseLayerInterface;
	ObjectVsBroadPhaseLayerFilter objectVsBroadphaseLayerFilter;
	ObjectLayerPairFilter objectVsObjectLayerFilter;
};

#endif
