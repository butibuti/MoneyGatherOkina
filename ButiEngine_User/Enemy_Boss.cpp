#include "stdafx_u.h"
#include "Enemy_Boss.h"
#include "Enemy.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"
#include "WorningMark.h"
#include "BossState_Fire.h"
#include "BossState_Crystal.h"
#include "BossState_Jump.h"

void ButiEngine::Enemy_Boss::OnUpdate()
{
	if (m_isDead)
	{
		return;
	}

	if (m_state != BossState::Flirting && !IsInAir() && m_vwp_enemyComponent.lock()->IsCapaOver())
	{
		FlirtingStart();
	}

	if (m_state == BossState::Wait)
	{
		Wait();
	}
	else if (m_state == BossState::Flirting)
	{
		Flirting();
	}
	else if (m_state == BossState::StrenghenWait)
	{
		StrengthenWait();
	}
	else if (!gameObject.lock()->HasGameObjectTag("Attack"))
	{
		WaitStart();
	}
}

void ButiEngine::Enemy_Boss::OnSet()
{
}

void ButiEngine::Enemy_Boss::OnShowUI()
{
	std::string stateName = "Wait";
	if (m_state == BossState::StrenghenWait) { stateName = "StrengthenWait"; }
	else if (m_state == BossState::Fire) { stateName = "Fire"; }
	else if (m_state == BossState::Crystal) { stateName = "Crystal"; }
	else if (m_state == BossState::Jump) { stateName = "Jump"; }
	GUI::Text("CurrentState : " + stateName);
	GUI::Text("FirePercent : %06.3f", m_firePercent);
	GUI::Text("CrystalPercent : %06.3f", m_crystalPercent);
	GUI::Text("JumpPercent : %06.3f", m_jumpPercent);
}

void ButiEngine::Enemy_Boss::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Boss");
	SetEnemyParameter();
	SetStateParameter();

	m_life = 100;
	m_maxLife = m_life;
	m_strengthenBorder = m_maxLife * 0.5f;
	m_isStrengthened = false;
	m_isDead = false;

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Boss::Clone()
{
	return ObjectFactory::Create<Enemy_Boss>();
}

bool ButiEngine::Enemy_Boss::IsInAir()
{
	if (m_state != BossState::Jump) { return false; }

	auto jump = gameObject.lock()->GetGameComponent<BossState_Jump>();
	if (!jump) { return false; }

	if (jump->GetJumpPhase() != JumpPhase::Wait) { return true; }

	return false;
}

void ButiEngine::Enemy_Boss::Dead()
{
	m_isDead = true;
	m_vwp_enemyComponent.lock()->Dead();
}

void ButiEngine::Enemy_Boss::Wait()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		SetState();
	}
}

void ButiEngine::Enemy_Boss::WaitStart()
{
	m_state = BossState::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::Enemy_Boss::Flirting()
{
	if (m_vlp_flirtingTimer->Update())
	{
		m_vlp_flirtingTimer->Stop();

		//ポケット再生成
		m_vwp_enemyComponent.lock()->CreatePocket(m_pocketCount);

		//ライフが一定以下で覚醒
		if (!m_isStrengthened)
		{
			CheckStrengthen();
		}

		m_vwp_enemyComponent.lock()->SetIsCapaOver(false);

		if (m_state == BossState::StrenghenWait) { return; }
		WaitStart();
	}
}

void ButiEngine::Enemy_Boss::FlirtingStart()
{
	RemoveAttackComponent();

	m_state = BossState::Flirting;
	m_vlp_flirtingTimer->Start();

	m_life -= 30;
	if (m_life <= 0)
	{
		m_life = 0;
		Dead();
		return;
	}

	//モブハチ振り払う
	m_vwp_enemyComponent.lock()->RemoveAllPocket();
	m_vwp_enemyComponent.lock()->SetVibration(0.0f);
}

void ButiEngine::Enemy_Boss::StrengthenWait()
{
	if (m_vlp_strengthenWaitTimer->Update())
	{
		m_vlp_strengthenWaitTimer->Stop();
		SetState();
	}
}

void ButiEngine::Enemy_Boss::StrengthenWaitStart()
{
	m_isStrengthened = true;
	m_state = BossState::StrenghenWait;
	m_vlp_strengthenWaitTimer->Start();
}

void ButiEngine::Enemy_Boss::SetState()
{
	float random = ButiRandom::GetRandom(1.0f, 100.0f, 1000);
	if (random <= m_firePercent)
	{
		m_state = BossState::Fire;
		DecreaseStatePercent(m_firePercent, m_crystalPercent, m_jumpPercent);
		gameObject.lock()->AddGameComponent<BossState_Fire>();
	}
	else if (random <= m_firePercent + m_crystalPercent)
	{
		m_state = BossState::Crystal;
		DecreaseStatePercent(m_crystalPercent, m_jumpPercent, m_firePercent);
		gameObject.lock()->AddGameComponent<BossState_Crystal>();
	}
	else
	{
		m_state = BossState::Jump;
		DecreaseStatePercent(m_jumpPercent, m_firePercent, m_crystalPercent);
		gameObject.lock()->AddGameComponent<BossState_Jump>();
	}
}

void ButiEngine::Enemy_Boss::DecreaseStatePercent(float& arg_percent, float& arg_otherPercent1, float& arg_otherPercent2)
{
	float decreasePercent = 15.0f;
	arg_percent -= decreasePercent;
	if (arg_percent < 0.0f)
	{
		decreasePercent += arg_percent;
		arg_percent = 0.0f;
	}

	float oneThird = decreasePercent / 3.0f;


	//減った確率の3分の2を少ない方に渡す
	if (arg_otherPercent1 <= arg_otherPercent2)
	{
		arg_otherPercent1 += oneThird * 2;
		arg_otherPercent2 = 100.0f - arg_percent - arg_otherPercent1;
	}
	else
	{
		arg_otherPercent2 += oneThird * 2;
		arg_otherPercent1 = 100.0f - arg_percent - arg_otherPercent2;
	}
}

void ButiEngine::Enemy_Boss::RemoveAttackComponent()
{
	if (m_state == BossState::Fire)
	{
		auto fire = gameObject.lock()->GetGameComponent<BossState_Fire>();
		if (fire)
		{
			fire->Dead();
		}
	}
	else if (m_state == BossState::Crystal)
	{
		auto crystal = gameObject.lock()->GetGameComponent<BossState_Crystal>();
		if (crystal)
		{
			crystal->Dead();
		}
	}
	else if (m_state == BossState::Jump)
	{
		auto jump = gameObject.lock()->GetGameComponent<BossState_Jump>();
		if (jump)
		{
			jump->Dead();
		}
	}
}

void ButiEngine::Enemy_Boss::SetEnemyParameter()
{
	m_pocketCount = 16;

	m_vwp_enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	m_vwp_enemyComponent.lock()->CreatePocket(m_pocketCount);
	m_vwp_enemyComponent.lock()->SetVibrationCapacity(10000.0f);
	m_vwp_enemyComponent.lock()->SetVibrationResistance(5.0f);
	m_vwp_enemyComponent.lock()->SetExplosionScale(8.0f);
	m_vwp_enemyComponent.lock()->SetWeight(1000.0f);
}

void ButiEngine::Enemy_Boss::SetStateParameter()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_flirtingTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_strengthenWaitTimer = ObjectFactory::Create<RelativeTimer>(90);

	m_crystalPercent = 30.0f;
	m_jumpPercent = 30.0f;
	m_firePercent = 100.0f - m_crystalPercent - m_jumpPercent;

	WaitStart();
}

void ButiEngine::Enemy_Boss::CheckStrengthen()
{
	if (m_life < m_strengthenBorder)
	{
		StrengthenWaitStart();
	}
}
