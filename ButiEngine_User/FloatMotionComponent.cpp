#include "stdafx_u.h"
#include "FloatMotionComponent.h"

void ButiEngine::FloatMotionComponent::OnUpdate()
{
}

void ButiEngine::FloatMotionComponent::OnSet()
{
}

void ButiEngine::FloatMotionComponent::OnShowUI()
{
}

void ButiEngine::FloatMotionComponent::Start()
{
	auto parentTransform = GetManager().lock()->GetGameObject("FloatMotionParent").lock()->transform;
	gameObject.lock()->transform->SetBaseTransform(parentTransform, true);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FloatMotionComponent::Clone()
{
	return ObjectFactory::Create<FloatMotionComponent>();
}
