#include "stdafx_u.h"
#include "VibrationEffectComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::VibrationEffectComponent::OnUpdate()
{
	Animation();

	if (m_animationCount >= m_maxAnimationCount)
	{
		m_animationCount = 0;
	}
}

void ButiEngine::VibrationEffectComponent::OnSet()
{
}

void ButiEngine::VibrationEffectComponent::OnRemove()
{
}

void ButiEngine::VibrationEffectComponent::OnShowUI()
{
	GUI::BulletText("AnimationRate");
	GUI::InputInt("##animationRate", m_animationRate);
}

void ButiEngine::VibrationEffectComponent::Start()
{
	m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_calcScale = m_defaultScale;
	m_randomPosY = ButiRandom::GetInt(1, 500) * 0.0001f;
	m_animationFrame = 0;
	m_animationRate = 4;
	m_animationCount = 0;
	m_maxAnimationCount = 3;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VibrationEffectComponent::Clone()
{
	return ObjectFactory::Create<VibrationEffectComponent>();
}

void ButiEngine::VibrationEffectComponent::SetVibration()
{
	float vibPower = 0.5f;
	std::int32_t animationRate = vibPower * 4;
	m_animationRate = 4 - animationRate;
	float scaleRate = 1.0f + vibPower;
	m_calcScale = m_defaultScale * scaleRate;
}

void ButiEngine::VibrationEffectComponent::SetVibrationViolent(const float arg_vibrationPower, const bool arg_isPlayer)
{
	float vibPower = arg_vibrationPower;
	std::int32_t animationRate = vibPower * 4;
	m_animationRate = 4 - animationRate;
	float scaleRate;
	if (arg_isPlayer)
	{
		scaleRate = 1.0f + vibPower * 6.0f;
	}
	else
	{
		scaleRate = 1.0f + vibPower * 0.3f;
	}
	m_calcScale = m_defaultScale * scaleRate;
}

void ButiEngine::VibrationEffectComponent::SetEffectPosition(const Vector3& arg_position)
{
	auto position = arg_position;
	position.y += m_randomPosY;
	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::VibrationEffectComponent::Animation()
{
	m_animationFrame++;

	if (m_animationFrame < m_animationRate) { return; }

	m_animationFrame = 0;

	m_animationCount++;

	if (m_animationCount % 2)
	{
		gameObject.lock()->transform->SetLocalScale(m_calcScale * 0.9f);
	}
	else
	{
		gameObject.lock()->transform->SetLocalScale(m_calcScale);
	}

	///////////////////////////////////////////////////
	gameObject.lock()->transform->SetLocalScale(0.0f);
	///////////////////////////////////////////////////
	m_vwp_spriteAnimationComponent.lock()->UpdateHorizontalAnim(1);
}
