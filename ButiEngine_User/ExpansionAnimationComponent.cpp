#include "stdafx_u.h"
#include "ExpansionAnimationComponent.h"

void ButiEngine::ExpansionAnimationComponent::OnUpdate()
{
	if (!m_isActive) { return; }

	float difference = std::abs(m_defaultScale.x - m_currentScale.x);

	if (difference < m_borderScale)
	{
		gameObject.lock()->transform->SetLocalScale(m_defaultScale);
		m_isActive = false;
	}
	else
	{
		m_currentScale.x = MathHelper::Lerp(m_currentScale.x, m_defaultScale.x, 0.2f);
		m_currentScale.y = MathHelper::Lerp(m_currentScale.y, m_defaultScale.y, 0.2f);
		m_currentScale.z = MathHelper::Lerp(m_currentScale.z, m_defaultScale.z, 0.2f);
		gameObject.lock()->transform->SetLocalScale(m_currentScale);
	}
}

void ButiEngine::ExpansionAnimationComponent::OnSet()
{
	m_isActive = true;
}

void ButiEngine::ExpansionAnimationComponent::OnRemove()
{
}

void ButiEngine::ExpansionAnimationComponent::OnShowUI()
{
}

void ButiEngine::ExpansionAnimationComponent::Start()
{
	m_currentScale = Vector3(0, 0, 0);

	m_borderScale = m_defaultScale.x * 0.05f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ExpansionAnimationComponent::Clone()
{
	return ObjectFactory::Create<ExpansionAnimationComponent>();
}
