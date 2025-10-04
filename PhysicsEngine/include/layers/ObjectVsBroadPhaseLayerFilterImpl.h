#ifndef OBJECT_VS_BROAD_PHASE_LAYER_FILTER_IMPL_H
#define OBJECT_VS_BROAD_PHASE_LAYER_FILTER_IMPL_H

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "BroadPhaseLayers.h"
#include "Layers.h"

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	[[nodiscard]] bool
	ShouldCollide(const JPH::ObjectLayer inLayer1, const JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return inLayer2 != BroadPhaseLayers::GHOST && inLayer2 != BroadPhaseLayers::SLOW_GHOST;
		case Layers::GHOST:
			return false;
		case Layers::SLOW_GHOST:
			return true;
		case Layers::ICARUS:
			return true;
		default:
			return false;
		}
	}
};

#endif
