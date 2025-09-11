#ifndef SAMPLE_APP
#define SAMPLE_APP

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

class SampleApp : public JPH::DebugRendererSimple
{
public:
	void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override
	{
		// Implement
	}

	void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor,
	                  ECastShadow inCastShadow) override
	{
		// Implement
	}

	void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor,
	                float inHeight) override
	{
		// Implement
	}
};

#endif
