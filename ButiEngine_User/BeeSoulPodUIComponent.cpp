#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"
#include "AppealRBUI.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	if (m_soulCount == m_maxSoulCount)
	{
		m_vwp_appealRBUI.lock()->Appear();
	}
	else
	{
		m_vwp_appealRBUI.lock()->Disappear();
	}

	Animation();
	ScaleAnimation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
	m_rate = 0.0f;
	m_previousRate = 0.0f;
	m_soulCount = 0;
}

void ButiEngine::BeeSoulPodUIComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulPodUIComponent::Start()
{
	m_vwp_appealRBUI = GetManager().lock()->GetGameObject("AppealRBUI").lock()->GetGameComponent<AppealRBUI>();

	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
	m_maxSoulCount = 50;

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_currentScale = m_defaultScale;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulPodUIComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulPodUIComponent>();
}

void ButiEngine::BeeSoulPodUIComponent::AddSoulCount()
{
	m_soulCount++;
	m_currentScale = Vector3(100, 100, 1);
	if (m_soulCount > m_maxSoulCount)
	{
		m_soulCount = m_maxSoulCount;
	}
}

void ButiEngine::BeeSoulPodUIComponent::Animation()
{
	m_rate = (float)m_soulCount / (float)m_maxSoulCount;

	auto lerpScale = 0.1f * GameDevice::WorldSpeed;
	m_previousRate = MathHelper::Lerp(m_previousRate, m_rate, lerpScale);

	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.y = 1.0f - m_previousRate;
}

void ButiEngine::BeeSoulPodUIComponent::ScaleAnimation()
{
	m_currentScale.x = MathHelper::Lerp(m_currentScale.x, m_defaultScale.x, 0.2f);
	m_currentScale.y = MathHelper::Lerp(m_currentScale.y, m_defaultScale.y, 0.2f);
	gameObject.lock()->transform->SetLocalScale(m_currentScale);
}
