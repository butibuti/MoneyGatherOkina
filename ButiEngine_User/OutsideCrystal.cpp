#include "stdafx_u.h"
#include "OutsideCrystal.h"
#include "Enemy.h"

std::int32_t ButiEngine::OutsideCrystal::m_progressPoint = 1000;
std::int32_t ButiEngine::OutsideCrystal::m_pocketCount = 16;
float ButiEngine::OutsideCrystal::m_createPocketRadius = 1.0f;
float ButiEngine::OutsideCrystal::m_vibrationCapacity = 3000.0f;
float ButiEngine::OutsideCrystal::m_vibrationResistance = 0.0f;

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
	GUI::BulletText(u8"�|�C���g");
	GUI::DragInt("##point", &m_progressPoint, 1, 0, 1000);

	GUI::BulletText(u8"�|�P�b�g�̐�");
	GUI::DragInt("##pocketCount", &m_pocketCount, 1, 0, 64);

	GUI::BulletText(u8"�|�P�b�g�������̔��a");
	GUI::DragFloat("##radius", &m_createPocketRadius, 0.1f, 0.0f, 100.0f);

	GUI::BulletText(u8"�U���l�̏��");
	GUI::DragFloat("##capacity", &m_vibrationCapacity, 0.1f, 0.0f, 1000.0f);

	GUI::BulletText(u8"�U���̒�R");
	GUI::DragFloat("##resistance", &m_vibrationResistance, 0.1f, 0.0f, 1000.0f);
}

void ButiEngine::OutsideCrystal::Start()
{
	SetEnemyParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::OutsideCrystal::Clone()
{
	return ObjectFactory::Create<OutsideCrystal>();
}

void ButiEngine::OutsideCrystal::Dead()
{
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
