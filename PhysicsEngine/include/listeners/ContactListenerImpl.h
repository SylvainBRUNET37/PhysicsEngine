#ifndef CONTACT_LISTENER_LOGGER_H
#define CONTACT_LISTENER_LOGGER_H

#include <Jolt/Physics/Collision/ContactListener.h>

#include "layers/Layers.h"
#include "systems/JoltSystem.h"

class ContactListenerImpl : public JPH::ContactListener
{
public:
	JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2,
	                                      JPH::RVec3Arg inBaseOffset,
	                                      const JPH::CollideShapeResult& inCollisionResult) override
	{
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold,
	                    JPH::ContactSettings& ioSettings) override
	{
		std::cout << "A contact was added" << '\n';

		HandleSlowGhost(inBody1, inBody2);
		HandleSlowGhost(inBody2, inBody1);

		HandleIcarus(inBody1);
		HandleIcarus(inBody2);
	}

	void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold,
	                        JPH::ContactSettings& ioSettings) override
	{
	}

	void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		std::cout << "A contact was removed" << '\n';
	}

private:
	static void HandleSlowGhost(const JPH::Body& sensorBody, const JPH::Body& other)
	{
		if (!sensorBody.IsSensor())
			return;

		if (other.GetObjectLayer() != Layers::SLOW_GHOST)
			return;

		JoltSystem::GetPostStepCallbacks().emplace_back
		(
			[&]
			{
				JoltSystem::GetBodyInterface().SetLinearVelocity(
					other.GetID(), other.GetLinearVelocity() * 0.5f);
			}
		);
	}

	static void HandleIcarus(const JPH::Body& body)
	{
		if (body.GetObjectLayer() == Layers::ICARUS)
		{
			JoltSystem::GetPostStepCallbacks().emplace_back([&]
			{
				JoltSystem::GetBodyInterface().SetLinearVelocity(body.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
		}
	}
};

#endif
