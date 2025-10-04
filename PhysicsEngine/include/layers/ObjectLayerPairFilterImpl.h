#ifndef OBJECT_LAYER_PAIR_FILTER_IMPL_H
#define OBJECT_LAYER_PAIR_FILTER_IMPL_H

#include <Jolt/Core/IssueReporting.h>

#include "Layers.h"

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	[[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer inLayer1, const JPH::ObjectLayer inLayer2) const override
	{
		if (inLayer1 == Layers::GHOST || inLayer2 == Layers::GHOST)
			return false;

		if (inLayer1 == Layers::MOVING && inLayer2 == Layers::NON_MOVING)
			return true;

		if (inLayer1 == Layers::NON_MOVING && inLayer2 == Layers::MOVING)
			return true;

		JPH_ASSERT(false);
		return false;
	}
};

#endif
