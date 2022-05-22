#include "stdafx_u.h"
#include "BossState_Jump.h"
#include "Enemy_Boss.h"

void ButiEngine::BossState_Jump::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		SetIsRemove(true);
	}
}

void ButiEngine::BossState_Jump::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_isStrengthened = gameObject.lock()->GetGameComponent<Enemy_Boss>()->IsStrengthened();
	SetPhaseParameter();
}

void ButiEngine::BossState_Jump::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Attack"));
}

void ButiEngine::BossState_Jump::OnShowUI()
{
}

void ButiEngine::BossState_Jump::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossState_Jump::Clone()
{
	return ObjectFactory::Create<BossState_Jump>();
}

void ButiEngine::BossState_Jump::Dead()
{
	SetIsRemove(true);
}

void ButiEngine::BossState_Jump::Wait()
{
}

void ButiEngine::BossState_Jump::WaitStart()
{
	m_phase = JumpPhase::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::BossState_Jump::Rise()
{
}

void ButiEngine::BossState_Jump::RiseStart()
{
	m_phase = JumpPhase::Rise;
	m_vlp_riseTimer->Start();
}

void ButiEngine::BossState_Jump::AirWait()
{
}

void ButiEngine::BossState_Jump::AirWaitStart()
{
	m_phase = JumpPhase::AirWait;
	m_vlp_airWaitTimer->Start();
}

void ButiEngine::BossState_Jump::Fall()
{
}

void ButiEngine::BossState_Jump::FallStart()
{
	m_phase = JumpPhase::Fall;
	m_vlp_fallTimer->Start();
}

void ButiEngine::BossState_Jump::SetPhaseParameter()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_riseTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_airWaitTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_fallTimer = ObjectFactory::Create<RelativeTimer>(60);

	WaitStart();
}