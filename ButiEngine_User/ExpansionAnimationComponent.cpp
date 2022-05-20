#include "stdafx_u.h"
#include "ExpansionAnimationComponent.h"

void ButiEngine::ExpansionAnimationComponent::OnUpdate()
{

}

void ButiEngine::ExpansionAnimationComponent::OnSet()
{
	m_timer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::ExpansionAnimationComponent::OnRemove()
{
}

void ButiEngine::ExpansionAnimationComponent::OnShowUI()
{
}

void ButiEngine::ExpansionAnimationComponent::Start()
{
	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_currentScale = Vector3(0, 0, 0);
	m_previousScale = m_currentScale;
	m_timer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ExpansionAnimationComponent::Clone()
{
	return ObjectFactory::Create<ExpansionAnimationComponent>();
}
