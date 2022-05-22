#include "stdafx_u.h"
#include "WarningMark.h"
#include "SeparateDrawObject.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::WarningMark::OnUpdate()
{
	Vector3 parentPos = m_parent.lock()->transform->GetLocalPosition();
	Vector3 newPos = parentPos;
	newPos.y += 3.0f;

	gameObject.lock()->transform->SetLocalPosition(newPos);
}

void ButiEngine::WarningMark::OnSet()
{
}

void ButiEngine::WarningMark::OnRemove()
{
}

void ButiEngine::WarningMark::OnShowUI()
{
}

void ButiEngine::WarningMark::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("WarningMark");

	m_isAppear = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WarningMark::Clone()
{
	return ObjectFactory::Create<WarningMark>();
}

void ButiEngine::WarningMark::Dead()
{
	gameObject.lock()->SetIsRemove(true);
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->SetIsRemove(true);
}

void ButiEngine::WarningMark::Appear()
{
	if (m_isAppear) { return; }

	AddScaleAnimation(1.0f);

	m_isAppear = true;
}

void ButiEngine::WarningMark::Disappear()
{
	if (!m_isAppear) { return; }

	AddScaleAnimation(Vector3Const::Zero);

	m_isAppear = false;
}

void ButiEngine::WarningMark::AddScaleAnimation(const Vector3& arg_targetScale)
{
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / 20);
	anim->SetEaseType(Easing::EasingType::EaseOutElastic);
}
