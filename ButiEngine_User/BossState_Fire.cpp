#include "stdafx_u.h"
#include "BossState_Fire.h"
#include "Enemy_Boss.h"

void ButiEngine::BossState_Fire::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		SetIsRemove(true);
	}
}

void ButiEngine::BossState_Fire::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_isStrengthened = gameObject.lock()->GetGameComponent<Enemy_Boss>()->IsStrengthened();

	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_waitTimer->Start();
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
