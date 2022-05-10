#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
}

void ButiEngine::BeeSoulPodUIComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulPodUIComponent::Start()
{
	m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
	m_animationCount = 0;
	m_maxAnimationCount = 5;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulPodUIComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulPodUIComponent>();
}

void ButiEngine::BeeSoulPodUIComponent::SetExpRate(const float arg_expRate)
{
	float expRate = 1.0f / (float)m_maxAnimationCount;
	for (std::int8_t i = 0; i < m_maxAnimationCount; i++)
	{
		if (arg_expRate >= expRate * i)
		{
			m_animationCount = i;
		}
	}
}

void ButiEngine::BeeSoulPodUIComponent::Animation()
{
	m_vwp_spriteAnimationComponent.lock()->SetHorizontalAnim(m_animationCount);	
}
