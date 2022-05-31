#include "stdafx_u.h"
#include "VibrationEffectComponent.h"
#include "PauseManagerComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::VibrationEffectComponent::OnUpdate()
{
	Animation();
	m_currentScale.x = MathHelper::Lerp(m_currentScale.x, m_calcScale.x, 0.25f * GameDevice::WorldSpeed);
	m_currentScale.y = MathHelper::Lerp(m_currentScale.y, m_calcScale.y, 0.25f * GameDevice::WorldSpeed);
	m_currentScale.z = MathHelper::Lerp(m_currentScale.z, m_calcScale.z, 0.25f * GameDevice::WorldSpeed);
	gameObject.lock()->transform->SetLocalScale(m_currentScale);

	if (m_animationCount >= m_maxAnimationCount)
	{
		m_animationCount = 0;
	}
}

void ButiEngine::VibrationEffectComponent::OnSet()
{
	m_vlp_animationTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::VibrationEffectComponent::OnRemove()
{
}

void ButiEngine::VibrationEffectComponent::OnShowUI()
{
}

void ButiEngine::VibrationEffectComponent::Start()
{
	m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
	m_vlp_animationTimer->Start();
	m_vlp_animationTimer->ChangeCountFrame(4);
	m_calcScale = m_defaultScale;
	m_randomPosY = ButiRandom::GetInt(1, 500) * 0.0001f;
	m_animationCount = 0;
	m_maxAnimationCount = 1;
	m_currentScale = Vector3(0, 0, 0);

	m_vwp_pauseManagerComponent = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManagerComponent>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VibrationEffectComponent::Clone()
{
	return ObjectFactory::Create<VibrationEffectComponent>();
}

void ButiEngine::VibrationEffectComponent::SetVibration()
{
	float vibPower = 0.5f;
	std::int32_t animationRate = vibPower * 4;
	m_vlp_animationTimer->ChangeCountFrame(4 - animationRate);
	float scaleRate = 1.0f + vibPower;
	m_calcScale = m_defaultScale * scaleRate;
}

void ButiEngine::VibrationEffectComponent::SetVibrationViolent(const float arg_vibrationPower, const bool arg_isPlayer)
{
	float vibPower = arg_vibrationPower;
	std::int32_t animationRate = vibPower * 4;
	m_vlp_animationTimer->ChangeCountFrame(4 - animationRate);
	float scaleRate;
	if (arg_isPlayer)
	{
		scaleRate = 1.0f + vibPower * 6.0f;
	}
	else
	{
		scaleRate = 1.0f + vibPower * 6.0f;
	}
	if (vibPower <= 0)
	{
		scaleRate = 0;
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
	if (!m_vlp_animationTimer->Update()) { return; }
	if (m_vwp_pauseManagerComponent.lock()->IsPause()) { return; }
	m_animationCount++;

	/////////////////////////////////////////////////////
	//gameObject.lock()->transform->SetLocalScale(0.0f);
	/////////////////////////////////////////////////////
	m_vwp_spriteAnimationComponent.lock()->UpdateHorizontalAnim(1);
}
