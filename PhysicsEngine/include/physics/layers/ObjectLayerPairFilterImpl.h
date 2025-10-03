#ifndef OBJECT_LAYER_PAIR_FILTER_IMPL_H
#define OBJECT_LAYER_PAIR_FILTER_IMPL_H

#include <Jolt/Core/IssueReporting.h>

#include "Layers.h"

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	[[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer inObject1, const JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

#endif
