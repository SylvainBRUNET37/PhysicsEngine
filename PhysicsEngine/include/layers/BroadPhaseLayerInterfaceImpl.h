#ifndef BROAD_PHASE_LAYER_INTERFACE_IMPL_H
#define BROAD_PHASE_LAYER_INTERFACE_IMPL_H

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "BroadPhaseLayers.h"
#include "Layers.h"

class BroadPhaseLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
	BroadPhaseLayerInterfaceImpl()
	{
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
		mObjectToBroadPhase[Layers::GHOST] = BroadPhaseLayers::GHOST;
		mObjectToBroadPhase[Layers::ICARUS] = BroadPhaseLayers::ICARUS;
		mObjectToBroadPhase[Layers::SLOW_GHOST] = BroadPhaseLayers::SLOW_GHOST;
		mObjectToBroadPhase[Layers::SENSOR] = BroadPhaseLayers::SENSOR;
	}

	[[nodiscard]] JPH::uint GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}
		
	[[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(const JPH::ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	[[nodiscard]] const char* GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const override
	{
		switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer))
		{
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING): return "NON_MOVING";
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING): return "MOVING";
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::GHOST): return "GHOST";
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::ICARUS): return "ICARUS";
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::SLOW_GHOST): return "SLOW_GHOST";
		case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::SENSOR): return "SENSOR";
		default: JPH_ASSERT(false);
			return "INVALID";
		}
	}
#endif

private:
	JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

#endif
