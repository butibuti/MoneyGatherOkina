#include "stdafx_u.h"
#include "ProgressPointUI.h"

float ButiEngine::ProgressPointUI::m_startPosition = 10.0f;
float ButiEngine::ProgressPointUI::m_position = 10.0f;

void ButiEngine::ProgressPointUI::OnUpdate()
{
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
	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_position -= 0.0001f;
	gameObject.lock()->transform->SetLocalPositionZ(m_position);

	m_startScale = Vector3Const::Zero;
	m_targetScale = gameObject.lock()->transform->GetLocalScale();

	m_isDead = false;

	gameObject.lock()->transform->SetLocalScale(m_startScale);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ProgressPointUI::Clone()
{
	return ObjectFactory::Create<ProgressPointUI>();
}
