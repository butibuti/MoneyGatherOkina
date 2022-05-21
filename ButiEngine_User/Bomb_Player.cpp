#include "stdafx_u.h"
#include "Bomb_Player.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::Bomb_Player::OnUpdate()
{
}

void ButiEngine::Bomb_Player::OnSet()
{
}

void ButiEngine::Bomb_Player::Start()
{
	auto playerDrawObject = GetManager().lock()->GetGameObject("DrawObject_Player");
	m_maxScale = playerDrawObject.lock()->transform->GetWorldScale() * 5.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bomb_Player::Clone()
{
	return ObjectFactory::Create<Bomb_Player>();
}

void ButiEngine::Bomb_Player::Appear()
{
	if (m_isAppear) { return; }

	AddScaleAnimation(m_maxScale);

	m_isAppear = true;
}

void ButiEngine::Bomb_Player::Disappear()
{
	if (!m_isAppear) { return; }

	AddScaleAnimation(Vector3Const::Zero);

	m_isAppear = false;
}

void ButiEngine::Bomb_Player::AddScaleAnimation(const Vector3& arg_targetScale)
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (anim)
	{
		anim->SetIsRemove(true);
	}
	anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / 30);
	anim->SetEaseType(Easing::EasingType::EaseOutElastic);
}
