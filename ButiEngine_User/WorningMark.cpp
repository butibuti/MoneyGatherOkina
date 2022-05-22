#include "stdafx_u.h"
#include "WorningMark.h"
#include "SeparateDrawObject.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::WorningMark::OnUpdate()
{
}

void ButiEngine::WorningMark::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::WorningMark::OnRemove()
{
}

void ButiEngine::WorningMark::OnShowUI()
{
}

void ButiEngine::WorningMark::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("WorningMark");

	AddScaleAnimation();

	m_addObjectName = "Crystal";
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorningMark::Clone()
{
	return ObjectFactory::Create<WorningMark>();
}

void ButiEngine::WorningMark::AddScaleAnimation()
{
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(Vector3(1.0f, 1.0f, 1.0f));
	anim->SetSpeed(1.0f / 20);
	anim->SetEaseType(Easing::EasingType::EaseOutElastic);
}
