#include "stdafx_u.h"
#include "LoopAnimation.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::LoopAnimation::OnUpdate()
{
	if (!isAnimation) { return; }
	
	if (m_vlp_animationIntervalTimer->Update())
	{
		m_vwp_spriteAnimation.lock()->UpdateHorizontalAnim(1);
	}
}

void ButiEngine::LoopAnimation::OnSet()
{
	m_vwp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
	m_vlp_animationIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_animationInterval);
}

void ButiEngine::LoopAnimation::OnRemove()
{
}

void ButiEngine::LoopAnimation::OnShowUI()
{
	GUI::BulletText("AnimationInterval");
	if (GUI::DragInt("##interval", &m_animationInterval, 1, 0, 128))
	{
		SetAnimationInterval(m_animationInterval);
	}
}

void ButiEngine::LoopAnimation::Start()
{
	m_vlp_animationIntervalTimer->Start();

	isAnimation = true;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LoopAnimation::Clone()
{
	auto output = ObjectFactory::Create<LoopAnimation>();
	output->m_animationInterval = m_animationInterval;
	return output;
}

void ButiEngine::LoopAnimation::AnimationStart()
{
	isAnimation = true;
}

void ButiEngine::LoopAnimation::AnimationStop()
{
	isAnimation = false;
}

void ButiEngine::LoopAnimation::AnimationReset()
{
	m_vwp_spriteAnimation.lock()->SetHorizontalAnim(0);
}
