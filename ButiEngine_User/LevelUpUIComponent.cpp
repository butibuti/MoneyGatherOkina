#include "stdafx_u.h"
#include "LevelUpUIComponent.h"

void ButiEngine::LevelUpUIComponent::OnUpdate()
{
	ScaleAnimation();

	if (m_life <= 60)
	{
		m_isChange = true;
	}

	if (m_isChange)
	{
		m_currentScale = Vector3(0, 0, 0);
	}
	else
	{
		m_currentScale = Vector3(260, 190, 0);
	}

	if (m_life > 0)
	{
		m_life--;
	}
	else
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::LevelUpUIComponent::OnSet()
{
}

void ButiEngine::LevelUpUIComponent::OnShowUI()
{
}

void ButiEngine::LevelUpUIComponent::Start()
{
	m_currentScale = Vector3(0, 0, 0);
	m_previousScale = m_currentScale;
	m_life = 120;
	m_isChange = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LevelUpUIComponent::Clone()
{
	return ObjectFactory::Create<LevelUpUIComponent>();
}

void ButiEngine::LevelUpUIComponent::ScaleAnimation()
{
	float lerpScale = 0.2f;
	m_previousScale.x = m_previousScale.x * (1.0f - lerpScale) + m_currentScale.x * lerpScale;
	m_previousScale.y = m_previousScale.y * (1.0f - lerpScale) + m_currentScale.y * lerpScale;

	auto transform = gameObject.lock()->transform;
	transform->SetLocalScale(m_previousScale);
}
