#include "stdafx_u.h"
#include "HexagonAnimationComponent.h"

void ButiEngine::HexagonAnimationComponent::OnUpdate()
{
	AnimationTime();
	ScaleAnimation();
}

void ButiEngine::HexagonAnimationComponent::OnSet()
{
}

void ButiEngine::HexagonAnimationComponent::Start()
{
	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_scale = Vector3(0, 0, 0);
	m_isShow = true;
	m_isAnimation = false;
	m_animationTimer = 0;
}

void ButiEngine::HexagonAnimationComponent::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::HexagonAnimationComponent::Clone()
{
	return ObjectFactory::Create<HexagonAnimationComponent>();
}

void ButiEngine::HexagonAnimationComponent::StartAnimation(const bool arg_isChange)
{
	bool isMistake = (m_isShow && arg_isChange) || (!m_isShow && !arg_isChange);
	if (isMistake) { return; }
	m_animationTimer = m_myNumber * 3;	
	m_isAnimation = true;
}

void ButiEngine::HexagonAnimationComponent::ScaleAnimation()
{
	if (m_isShow)
	{
		m_scale = m_defaultScale;
	}
	else
	{
		m_scale = Vector3(0, 0, 0);
	}
	gameObject.lock()->transform->SetLocalScale(m_scale);
}

void ButiEngine::HexagonAnimationComponent::AnimationTime()
{
	if (!m_isAnimation) { return; }
	if (m_animationTimer > 0)
	{
		m_animationTimer--;
	}
	else
	{
		m_isAnimation = false;
		m_isShow = !m_isShow;
	}
}
