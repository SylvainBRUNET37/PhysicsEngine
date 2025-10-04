#ifndef CONTACT_LISTENER_LOGGER_H
#define CONTACT_LISTENER_LOGGER_H

#include <Jolt/Physics/Collision/ContactListener.h>

#include "systems/JoltSystem.h"

class ContactListenerLogger : public JPH::ContactListener
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

		HandleSensor(inBody1, inBody2);
		HandleSensor(inBody2, inBody1);

		if (inBody1.GetObjectLayer() == Layers::ICARUS)
			JoltSystem::GetPostStepCallbacks().emplace_back([&]
			{
				JoltSystem::GetBodyInterface().SetLinearVelocity(inBody1.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
		if (inBody2.GetObjectLayer() == Layers::ICARUS)
			JoltSystem::GetPostStepCallbacks().emplace_back([&]
			{
				JoltSystem::GetBodyInterface().SetLinearVelocity(inBody2.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
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
	static void HandleSensor(const JPH::Body& sensorBody, const JPH::Body& other)
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
};

#endif
