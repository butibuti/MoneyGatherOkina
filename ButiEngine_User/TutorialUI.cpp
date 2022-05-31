#include "stdafx_u.h"
#include "TutorialUI.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::TutorialUI::OnUpdate()
{
	m_theta += 0.05f * GameDevice::WorldSpeed;
	if (!m_isAppear) { return; }

	if (m_isLoopAnim)
	{
		float progress = std::sin(m_theta);
		Vector3 scale = MathHelper::LerpPosition(m_targetScale, m_targetScale * 1.05f, progress);
		gameObject.lock()->transform->SetLocalScale(scale);

		return;
	}

	float progress = m_vlp_appearTimer->GetPercent();
	Vector3 scale = MathHelper::LerpPosition(m_startScale, m_targetScale, Easing::EaseOutExpo(progress));
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_vlp_appearTimer->Update())
	{
		m_vlp_appearTimer->Stop();

		gameObject.lock()->transform->SetLocalScale(m_targetScale);

		m_isLoopAnim = true;
	}
}

void ButiEngine::TutorialUI::OnSet()
{
}

void ButiEngine::TutorialUI::OnRemove()
{
}

void ButiEngine::TutorialUI::OnShowUI()
{
}

void ButiEngine::TutorialUI::Start()
{
	m_isAppear = false;
	m_isLoopAnim = false;
	m_startScale = Vector3Const::Zero;
	m_targetScale = gameObject.lock()->transform->GetLocalScale();
	gameObject.lock()->transform->SetLocalScale(0.0f);
	m_theta = 0.0f;

	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_appearTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TutorialUI::Clone()
{
	return ObjectFactory::Create<TutorialUI>();
}

void ButiEngine::TutorialUI::Appear()
{
	if (m_isAppear) { return; }

	m_vlp_appearTimer->Start();

	m_isAppear = true;
}
