#include "stdafx_u.h"
#include "OutsideCrystal.h"
#include "Enemy.h"

std::int32_t ButiEngine::OutsideCrystal::m_progressPoint = 1000;
std::int32_t ButiEngine::OutsideCrystal::m_pocketCount = 16;
float ButiEngine::OutsideCrystal::m_createPocketRadius = 1.0f;
float ButiEngine::OutsideCrystal::m_vibrationCapacity = 3000.0f;
float ButiEngine::OutsideCrystal::m_vibrationResistance = 0.0f;
std::int32_t ButiEngine::OutsideCrystal::m_appearIntervalFrame = 1800;

void ButiEngine::OutsideCrystal::OnUpdate()
{
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
	m_appearIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_appearIntervalFrame);
	SetEnemyParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::OutsideCrystal::Clone()
{
	return ObjectFactory::Create<OutsideCrystal>();
}

void ButiEngine::OutsideCrystal::Appeaer()
{
	if (m_isAppear) { return; }

	m_appearIntervalTimer->Stop();
	m_appearIntervalTimer->Reset();

	m_isAppear = true;
}

void ButiEngine::OutsideCrystal::Disappear()
{
	if (!m_isAppear) { return; }
	m_isAppear = false;
	m_appearIntervalTimer->Start();
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
