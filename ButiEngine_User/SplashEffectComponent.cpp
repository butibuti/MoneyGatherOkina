#include "stdafx_u.h"
#include "SplashEffectComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::SplashEffectComponent::OnUpdate()
{
	Animation();
	
	if (m_animationCount >= m_maxAnimationCount)
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::SplashEffectComponent::OnSet()
{
}

void ButiEngine::SplashEffectComponent::OnRemove()
{
}

void ButiEngine::SplashEffectComponent::OnShowUI()
{
}

void ButiEngine::SplashEffectComponent::Start()
{
	m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
	m_animationFrame = 0;
	m_animationRate = 10;
	m_animationCount = 0;
	m_maxAnimationCount = 8;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SplashEffectComponent::Clone()
{
	return ObjectFactory::Create<SplashEffectComponent>();
}

void ButiEngine::SplashEffectComponent::Animation()
{
	m_animationFrame++;

	if (m_animationFrame < m_animationRate) { return; }

	m_animationFrame = 0;

	m_animationCount++;

	m_vwp_spriteAnimationComponent.lock()->UpdateHorizontalAnim(1);
}
