#include "stdafx_u.h"
#include "Enemy_Fly.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"
#include "ShakeComponent.h"

std::int32_t ButiEngine::Enemy_Fly::m_progressPoint = 1;
std::int32_t ButiEngine::Enemy_Fly::m_pocketCount = 3;
float ButiEngine::Enemy_Fly::m_createPocketRadius = 1.5f;
float ButiEngine::Enemy_Fly::m_vibrationCapacity = 10.0f;
float ButiEngine::Enemy_Fly::m_vibrationResistance = 0.0f;

float ButiEngine::Enemy_Fly::m_knockBackForce = 0.2f;
std::int32_t ButiEngine::Enemy_Fly::m_knockBackFrame = 30;

void ButiEngine::Enemy_Fly::OnUpdate()
{
}

void ButiEngine::Enemy_Fly::OnSet()
{
}

void ButiEngine::Enemy_Fly::OnRemove()
{
}

void ButiEngine::Enemy_Fly::OnShowUI()
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

	GUI::BulletText(u8"ノックバックの初速");
	GUI::DragFloat("##knockBackForce", &m_knockBackForce, 0.1f, 0.0f, 1000.0f);

	GUI::BulletText(u8"ノックバックにかかる時間");
	GUI::DragInt("##knockBackFrame", &m_knockBackFrame, 1, 0, 600);
}

void ButiEngine::Enemy_Fly::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Fly");

	SetEnemyParameter();
	SetLoiterParameter();
	SetLookAtParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Fly::Clone()
{
	return ObjectFactory::Create<Enemy_Fly>();
}

void ButiEngine::Enemy_Fly::Dead()
{
}

void ButiEngine::Enemy_Fly::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->SetProgressPoint(m_progressPoint);
	m_vlp_enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	m_vlp_enemy->SetVibrationCapacity(m_vibrationCapacity);
	m_vlp_enemy->SetVibrationResistance(m_vibrationResistance);
	m_vlp_enemy->SetKnockBackForce(m_knockBackForce);
	m_vlp_enemy->SetKnockBackFrame(m_knockBackFrame);
	m_vlp_enemy->SetWeight(100.0f);
}

void ButiEngine::Enemy_Fly::SetLoiterParameter()
{
	auto loiterComponent = gameObject.lock()->GetGameComponent<Loiter>();
	loiterComponent->SetMoveRange(2.0f);
	loiterComponent->SetMaxMoveSpeed(0.2f);
	loiterComponent->SetWaitFrame(60);
	loiterComponent->SetAccelFrame(30);
	loiterComponent->SetBrakeFrame(30);
}

void ButiEngine::Enemy_Fly::SetLookAtParameter()
{
	auto lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	lookAt->SetSpeed(0.1f);
}
