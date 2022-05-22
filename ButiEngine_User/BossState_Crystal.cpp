#include "stdafx_u.h"
#include "BossState_Crystal.h"
#include "Enemy_Boss.h"

void ButiEngine::BossState_Crystal::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		SetIsRemove(true);
	}
}

void ButiEngine::BossState_Crystal::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_isStrengthened = gameObject.lock()->GetGameComponent<Enemy_Boss>()->IsStrengthened();

	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_waitTimer->Start();
}

void ButiEngine::BossState_Crystal::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Attack"));
}

void ButiEngine::BossState_Crystal::OnShowUI()
{
}

void ButiEngine::BossState_Crystal::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossState_Crystal::Clone()
{
	return ObjectFactory::Create<BossState_Crystal>();
}

void ButiEngine::BossState_Crystal::Dead()
{
	SetIsRemove(true);
}
