#include "stdafx_u.h"
#include "AbsoluteAnimationRemoverComponent.h"
#include "Suicide.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::AbsoluteAnimationRemoverComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::AbsoluteAnimationRemoverComponent::OnSet()
{
}

void ButiEngine::AbsoluteAnimationRemoverComponent::OnShowUI()
{
}

void ButiEngine::AbsoluteAnimationRemoverComponent::Start()
{
	m_vlp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	std::uint8_t animationInterval = 5;
	m_vlp_animationIntervalTimer = ObjectFactory::Create<AbsoluteTimer>(animationInterval);
	m_vlp_animationIntervalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::AbsoluteAnimationRemoverComponent::Clone()
{
	return ObjectFactory::Create<AbsoluteAnimationRemoverComponent>();
}

void ButiEngine::AbsoluteAnimationRemoverComponent::Animation()
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
