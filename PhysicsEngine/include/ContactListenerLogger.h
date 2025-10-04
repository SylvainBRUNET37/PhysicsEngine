#ifndef CONTACT_LISTENER_LOGGER_H
#define CONTACT_LISTENER_LOGGER_H

#include <Jolt/Physics/Collision/ContactListener.h>

class ContactListenerLogger : public JPH::ContactListener
{
public:
	explicit ContactListenerLogger(JPH::BodyInterface& bodyInterface,
	                               std::vector<std::function<void()>>& postStepActions)
		: bodyInterface{bodyInterface}, postStepActions{postStepActions}
	{
	}

	JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2,
	                                      JPH::RVec3Arg inBaseOffset,
	                                      const JPH::CollideShapeResult& inCollisionResult) override
	{
		std::cout << "Contact validate callback" << '\n';

		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold,
	                    JPH::ContactSettings& ioSettings) override
	{
		std::cout << "A contact was added" << '\n';

		if (inBody1.GetObjectLayer() == Layers::ICARUS)
			postStepActions.emplace_back([&]
			{
				bodyInterface.SetLinearVelocity(inBody1.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
		if (inBody2.GetObjectLayer() == Layers::ICARUS)
			postStepActions.emplace_back([&]
			{
				bodyInterface.SetLinearVelocity(inBody2.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
	}

	void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold,
	                        JPH::ContactSettings& ioSettings) override
	{
		std::cout << "A contact was persisted" << '\n';
	}

	void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		std::cout << "A contact was removed" << '\n';
	}

private:
	JPH::BodyInterface& bodyInterface;
	std::vector<std::function<void()>>& postStepActions;
};

#endif
