#include "stdafx_u.h"
#include "WorkerAttackFlashEffect.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::WorkerAttackFlashEffect::OnUpdate()
{
	Animation();
}

void ButiEngine::WorkerAttackFlashEffect::OnSet()
{
}

void ButiEngine::WorkerAttackFlashEffect::OnShowUI()
{
}

void ButiEngine::WorkerAttackFlashEffect::Start()
{
	m_vlp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	std::uint8_t animationInterval = 2;
	m_vlp_animationIntervalTimer = ObjectFactory::Create<RelativeTimer>(animationInterval);
	m_vlp_animationIntervalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorkerAttackFlashEffect::Clone()
{
	return ObjectFactory::Create<WorkerAttackFlashEffect>();
}

void ButiEngine::WorkerAttackFlashEffect::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::WorkerAttackFlashEffect::Animation()
{
	if (m_vlp_animationIntervalTimer->Update())
	{
		m_vlp_spriteAnimation->UpdateHorizontalAnim(1);

		////アニメーションが一周したら死ぬ
		//if (m_vlp_spriteAnimation->GetHorizontalAnim() == m_vlp_spriteAnimation->GetHorizontalSplitScale() - 1)
		//{
		//	gameObject.lock()->SetIsRemove(true);
		//}
	}
}
