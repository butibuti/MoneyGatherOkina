#include "stdafx_u.h"
#include "EnemyScaleAnimationComponent.h"

void ButiEngine::EnemyScaleAnimationComponent::OnUpdate()
{
	m_previousScale = m_defaultScale + m_maxPlusScale * std::abs(Easing::EaseOutExpo(m_lerpScale));
	m_previousScale *= m_anotherScale;

	gameObject.lock()->transform->SetLocalScale(m_previousScale);
}

void ButiEngine::EnemyScaleAnimationComponent::OnSet()
{
	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_previousScale = m_defaultScale;
	m_maxPlusScale = m_defaultScale * 0.2f;
	m_anotherScale = Vector3(1, 1, 1);

	m_lerpScale = 0;
}

void ButiEngine::EnemyScaleAnimationComponent::OnShowUI()
{
}

void ButiEngine::EnemyScaleAnimationComponent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemyScaleAnimationComponent::Clone()
{
	return ObjectFactory::Create<EnemyScaleAnimationComponent>();
}
