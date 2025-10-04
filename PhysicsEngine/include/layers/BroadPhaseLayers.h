#ifndef BROAD_PHASE_LAYERS_H
#define BROAD_PHASE_LAYERS_H

#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::BroadPhaseLayer GHOST(2);
	static constexpr JPH::BroadPhaseLayer ICARUS(3);
	static constexpr JPH::BroadPhaseLayer SLOW_GHOST(4);
	static constexpr JPH::BroadPhaseLayer SENSOR(5);
	static constexpr JPH::uint NUM_LAYERS(6);
};

#endif
