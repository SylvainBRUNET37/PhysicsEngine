#include "pch.h"
#include "listeners/ContactListenerImpl.h"

#include "layers/Layers.h"

void ContactListenerImpl::HandleSlowGhost(const JPH::Body& sensorBody, const JPH::Body& other)
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

void ContactListenerImpl::HandleIcarus(const JPH::Body& body)
{
	if (body.GetObjectLayer() == Layers::ICARUS)
	{
		JoltSystem::GetPostStepCallbacks().emplace_back([&]
			{
				JoltSystem::GetBodyInterface().SetLinearVelocity(body.GetID(), JPH::Vec3(0, 0.5f, 0));
			});
	}
}
