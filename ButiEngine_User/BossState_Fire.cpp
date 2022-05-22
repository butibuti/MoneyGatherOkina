#include "stdafx_u.h"
#include "BossState_Fire.h"
#include "Enemy_Boss.h"
#include "FireBall.h"

void ButiEngine::BossState_Fire::OnUpdate()
{
	if (m_vlp_stateTimer->Update())
	{
		SetIsRemove(true);
	}
}

void ButiEngine::BossState_Fire::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_isStrengthened = gameObject.lock()->GetGameComponent<Enemy_Boss>()->IsStrengthened();

	m_vlp_stateTimer = ObjectFactory::Create<RelativeTimer>(700);
	m_vlp_stateTimer->Start();

	CreateFireBall();
}

void ButiEngine::BossState_Fire::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Attack"));
}

void ButiEngine::BossState_Fire::OnShowUI()
{
}

void ButiEngine::BossState_Fire::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossState_Fire::Clone()
{
	return ObjectFactory::Create<BossState_Fire>();
}

void ButiEngine::BossState_Fire::Dead()
{
	auto end = m_vec_fireBalls.end();
	for (auto itr = m_vec_fireBalls.begin(); itr != end; ++itr)
	{
		auto fireBall = (*itr).lock()->GetGameComponent<FireBall>();
		if (fireBall)
		{
			fireBall->DisappeaerStart();
		}
	}

	SetIsRemove(true);
}

void ButiEngine::BossState_Fire::CreateFireBall()
{
	std::uint8_t fireBallCount = 2;
	if (m_isStrengthened)
	{
		fireBallCount = 3;
	}

	//自身の周りに等間隔でファイアボールを作成する
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;

	auto fireBallCenter = gameObject.lock()->transform->Clone();
	auto fireBallTransform = ObjectFactory::Create<Transform>();
	fireBallTransform->SetBaseTransform(fireBallCenter);
	fireBallTransform->SetLocalPosition(Vector3(-(radius + 4.0f) / gameObject.lock()->transform->GetLocalScale().x, 0.0f, 0.0f));

	float rollAngle = 360.0f / fireBallCount;

	for (std::uint8_t i = 0; i < fireBallCount; i++)
	{
		auto fireBall = GetManager().lock()->AddObjectFromCereal("FireBall", ObjectFactory::Create<Transform>());

		Vector3 pos = fireBallTransform->GetWorldPosition();
		fireBall.lock()->transform->SetLocalPosition(pos);
		fireBall.lock()->transform->SetLocalScale(0.0f);

		auto vlp_fireBallComponent = fireBall.lock()->GetGameComponent<FireBall>();
		if (i % 2 == 0)
		{
			vlp_fireBallComponent->SetIsClockwise(true);
		}

		vlp_fireBallComponent->SetIsStrengthened(m_isStrengthened);
		vlp_fireBallComponent->SetDefaultScale(2.5f);

		m_vec_fireBalls.push_back(fireBall);

		fireBallCenter->RollLocalRotationY_Degrees(rollAngle);
	}
}
