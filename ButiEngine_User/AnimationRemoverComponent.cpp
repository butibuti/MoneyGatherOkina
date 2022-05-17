#include "stdafx_u.h"
#include "AnimationRemoverComponent.h"
#include "Suicide.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::AnimationRemoverComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::AnimationRemoverComponent::OnSet()
{
}

void ButiEngine::AnimationRemoverComponent::OnShowUI()
{
}

void ButiEngine::AnimationRemoverComponent::Start()
{
	m_vlp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	std::uint8_t animationInterval = 3;
	m_vlp_animationIntervalTimer = ObjectFactory::Create<RelativeTimer>(animationInterval);
	m_vlp_animationIntervalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::AnimationRemoverComponent::Clone()
{
	return ObjectFactory::Create<AnimationRemoverComponent>();
}

void ButiEngine::AnimationRemoverComponent::Animation()
{
	if (m_vlp_animationIntervalTimer->Update())
	{
		m_vlp_spriteAnimation->UpdateHorizontalAnim(1);

		//アニメーションが一周したら死ぬ
		if (m_vlp_spriteAnimation->GetHorizontalAnim() == m_vlp_spriteAnimation->GetHorizontalSplitScale() - 1)
		{
			gameObject.lock()->SetIsRemove(true);
		}
	}
}
