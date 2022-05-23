#include "stdafx_u.h"
#include "AppealRBUI.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::AppealRBUI::OnUpdate()
{
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	if (scale != m_targetScale) { return; }
	SpriteAnimation();
}

void ButiEngine::AppealRBUI::OnSet()
{
}

void ButiEngine::AppealRBUI::OnRemove()
{
}

void ButiEngine::AppealRBUI::OnShowUI()
{
}

void ButiEngine::AppealRBUI::Start()
{
	m_vlp_spriteAnimationTimer = ObjectFactory::Create<RelativeTimer>(5);
	m_vlp_spriteAnimationTimer->Start();

	m_vwp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	m_targetScale = gameObject.lock()->transform->GetLocalScale();
	gameObject.lock()->transform->SetLocalScale(Vector3(0.0f, 0.0f, 1.0f));
	m_isAppear = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::AppealRBUI::Clone()
{
	return ObjectFactory::Create<AppealRBUI>();
}

void ButiEngine::AppealRBUI::Appear()
{
	if (m_isAppear) { return; }

	AddScaleAnimation(m_targetScale);

	m_isAppear = true;
}

void ButiEngine::AppealRBUI::Disappear()
{
	if (!m_isAppear) { return; }

	AddScaleAnimation(Vector3(0.0f, 0.0f, 1.0f));

	m_isAppear = false;
}

void ButiEngine::AppealRBUI::SpriteAnimation()
{
	if (m_vlp_spriteAnimationTimer->Update())
	{
		m_vwp_spriteAnimation.lock()->UpdateHorizontalAnim(1);
	}
}

void ButiEngine::AppealRBUI::AddScaleAnimation(const Vector3& arg_targetScale)
{
	m_vwp_scaleAnimation = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (m_vwp_scaleAnimation.lock())
	{
		m_vwp_scaleAnimation.lock()->SetIsRemove(true);
	}
	m_vwp_scaleAnimation = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	m_vwp_scaleAnimation.lock()->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	m_vwp_scaleAnimation.lock()->SetTargetScale(arg_targetScale);
	m_vwp_scaleAnimation.lock()->SetSpeed(1.0f / 30);
	m_vwp_scaleAnimation.lock()->SetEaseType(Easing::EasingType::EaseOutElastic);
}
