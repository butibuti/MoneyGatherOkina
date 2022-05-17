#include "stdafx_u.h"
#include "NumberComponent.h"

void ButiEngine::NumberComponent::OnUpdate()
{
}

void ButiEngine::NumberComponent::OnSet()
{
}

void ButiEngine::NumberComponent::OnShowUI()
{
}

void ButiEngine::NumberComponent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberComponent::Clone()
{
	return ObjectFactory::Create<NumberComponent>();
}
