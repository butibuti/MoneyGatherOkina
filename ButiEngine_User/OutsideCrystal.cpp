#include "stdafx_u.h"
#include "OutsideCrystal.h"
#include "Enemy.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"

std::int32_t ButiEngine::OutsideCrystal::m_progressPoint = 1000;
std::int32_t ButiEngine::OutsideCrystal::m_pocketCount = 16;
float ButiEngine::OutsideCrystal::m_createPocketRadius = 3.0f;
float ButiEngine::OutsideCrystal::m_vibrationCapacity = 3000.0f;
float ButiEngine::OutsideCrystal::m_vibrationResistance = 0.0f;
std::int32_t ButiEngine::OutsideCrystal::m_appearIntervalFrame = 1800;

void ButiEngine::OutsideCrystal::OnUpdate()
{
	if (!m_isAppear)
	{
		Wait();
	}
}

void ButiEngine::OutsideCrystal::OnSet()
{
}

void ButiEngine::OutsideCrystal::OnRemove()
{
}

void ButiEngine::OutsideCrystal::OnShowUI()
{
	GUI::BulletText(u8"ポイント");
	GUI::DragInt("##point", &m_progressPoint, 1, 0, 1000);

	GUI::BulletText(u8"ポケットの数");
	GUI::DragInt("##pocketCount", &m_pocketCount, 1, 0, 64);

	GUI::BulletText(u8"ポケット生成時の半径");
	GUI::DragFloat("##radius", &m_createPocketRadius, 0.1f, 0.0f, 100.0f);

	GUI::BulletText(u8"振動値の上限");
	GUI::DragFloat("##capacity", &m_vibrationCapacity, 0.1f, 0.0f, 1000.0f);

	GUI::BulletText(u8"振動の抵抗");
	GUI::DragFloat("##resistance", &m_vibrationResistance, 0.1f, 0.0f, 1000.0f);

	GUI::BulletText(u8"再生成までの時間");
	GUI::DragInt("##appearInterval", &m_appearIntervalFrame, 1, 0, 18000);
}

void ButiEngine::OutsideCrystal::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("OutsideCrystal");
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1000.0f);

	m_vlp_appearIntervalTimer = ObjectFactory::Create<RelativeTimer>(0);
	SetEnemyParameter();

	gameObject.lock()->transform->SetLocalScale(0.0f);
	m_isAppear = true;
	m_isInvincible = true;

	Disappear();

	isActive = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::OutsideCrystal::Clone()
{
	return ObjectFactory::Create<OutsideCrystal>();
}

void ButiEngine::OutsideCrystal::Appeaer()
{
	if (m_isAppear) { return; }

	gameObject.lock()->transform->SetLocalScale(m_defaultScale);

	m_isAppear = true;
	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	//collider->SetIsActive(true);
	auto enemy = gameObject.lock()->GetGameComponent<Enemy>()->isCollision = true;
}

void ButiEngine::OutsideCrystal::Disappear()
{
	if (!m_isAppear) { return; }

	gameObject.lock()->transform->SetLocalScale(0.0f);

	m_isAppear = false;
	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	auto enemy = gameObject.lock()->GetGameComponent<Enemy>()->isCollision = false;
	//collider->SetIsActive(false);


	m_vlp_appearIntervalTimer->Start();
}

void ButiEngine::OutsideCrystal::Wait()
{
	if (m_vlp_appearIntervalTimer->Update())
	{
		m_vlp_appearIntervalTimer->Stop();
		m_vlp_appearIntervalTimer->Reset();
		m_vlp_appearIntervalTimer->ChangeCountFrame(m_appearIntervalFrame);

		Appeaer();
	}
}

void ButiEngine::OutsideCrystal::SetEnemyParameter()
{
	auto enemy = gameObject.lock()->GetGameComponent<Enemy>();
	enemy->SetProgressPoint(m_progressPoint);
	enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	enemy->SetVibrationCapacity(m_vibrationCapacity);
	enemy->SetVibrationResistance(m_vibrationResistance);
	enemy->SetWeight(10000.0f);
}
