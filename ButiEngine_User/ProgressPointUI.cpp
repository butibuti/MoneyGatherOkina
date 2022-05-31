#include "stdafx_u.h"
#include "ProgressPointUI.h"

float ButiEngine::ProgressPointUI::m_startZShift = 10.0f;
float ButiEngine::ProgressPointUI::m_ZShift = 10.0f;

void ButiEngine::ProgressPointUI::OnUpdate()
{
	Vector3 screenPosition = GetCamera("main")->WorldToScreen(m_worldPos);
	screenPosition.z = m_positionZ;

	gameObject.lock()->transform->SetLocalPosition(screenPosition);

	if (!m_isDead)
	{
		float progress = m_vlp_animatoinTimer->GetPercent();
		Vector3 scale = MathHelper::LerpPosition(m_startScale, m_targetScale, Easing::EaseOutBack(progress));
		gameObject.lock()->transform->SetLocalScale(scale);

		if (m_vlp_animatoinTimer->Update())
		{
			m_vlp_animatoinTimer->Stop();

			gameObject.lock()->transform->SetLocalScale(m_targetScale);
			m_isDead = true;

			m_vlp_deadTimer->Start();
		}
	}
	else if(m_vlp_deadTimer->Update())
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::ProgressPointUI::OnSet()
{
}

void ButiEngine::ProgressPointUI::OnRemove()
{
}

void ButiEngine::ProgressPointUI::OnShowUI()
{
}

void ButiEngine::ProgressPointUI::Start()
{
	m_vlp_animatoinTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_animatoinTimer->Start();
	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_ZShift -= 0.0001f;

	m_worldPos = gameObject.lock()->transform->GetLocalPosition();

	Vector3 screenPosition = GetCamera("main")->WorldToScreen(m_worldPos);
	screenPosition.z = m_ZShift;

	gameObject.lock()->transform->SetLocalPosition(screenPosition);
	m_positionZ = m_ZShift;

	m_startScale = Vector3Const::Zero;
	m_targetScale = gameObject.lock()->transform->GetLocalScale();

	m_isDead = false;

	gameObject.lock()->transform->SetLocalScale(m_startScale);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ProgressPointUI::Clone()
{
	return ObjectFactory::Create<ProgressPointUI>();
}
