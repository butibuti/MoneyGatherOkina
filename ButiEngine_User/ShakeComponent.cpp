#include "stdafx_u.h"
#include "ShakeComponent.h"

void ButiEngine::ShakeComponent::OnUpdate()
{
}

void ButiEngine::ShakeComponent::OnSet()
{
}

void ButiEngine::ShakeComponent::Start()
{
}

void ButiEngine::ShakeComponent::ShakeStart(const float arg_amplitude)
{
}

void ButiEngine::ShakeComponent::ShakeStop()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}
