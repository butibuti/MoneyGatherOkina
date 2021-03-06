#include "stdafx_u.h"
#include "OutsideCrystal.h"
#include "Enemy.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"
#include "KnockBack.h"
#include "SpawnEffect.h"
#include "GameSettings.h"
#include "WaveManager.h"

std::int32_t ButiEngine::OutsideCrystal::m_progressPoint = 20;
std::int32_t ButiEngine::OutsideCrystal::m_pocketCount = 16;
float ButiEngine::OutsideCrystal::m_createPocketRadius = 3.0f;
float ButiEngine::OutsideCrystal::m_vibrationCapacity = 25.0f;
float ButiEngine::OutsideCrystal::m_vibrationResistance = 0.0f;
std::int32_t ButiEngine::OutsideCrystal::m_appearIntervalFrame = 300;
std::int32_t ButiEngine::OutsideCrystal::m_spwanStalkerCount = 3;
float ButiEngine::OutsideCrystal::m_stalerKnockBackForce = 0.2f;
float ButiEngine::OutsideCrystal::m_knockBackY = 0.5f;

void ButiEngine::OutsideCrystal::OnUpdate()
{
	gameObject.lock()->transform->RollLocalRotationY_Degrees(1.0f);

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

	GUI::BulletText(u8"壊れた時生成するストーカーの数");
	GUI::DragInt("##spawnStalkerCount", &m_spwanStalkerCount, 1, 0, 100);

	GUI::BulletText(u8"生成したストーカーを飛ばす力");
	GUI::DragFloat("##stalkerKnockBackForce", &m_stalerKnockBackForce, 0.1f, 0.0f, 10.0f);

	GUI::BulletText(u8"飛ばす高さ");
	GUI::DragFloat("##knockBackY", &m_knockBackY, 0.1f, 0.0f, 10.0f);
}

void ButiEngine::OutsideCrystal::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("OutsideCrystal");
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1000.0f);

	m_vlp_appearIntervalTimer = ObjectFactory::Create<RelativeTimer>(0);
	SetEnemyParameter();

	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

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
	if (m_isAppear || m_vwp_waveManager.lock()->IsClearAnimation()) { return; }

	gameObject.lock()->transform->SetLocalScale(m_defaultScale);

	m_isAppear = true;
	gameObject.lock()->GetGameComponent<Collision::ColliderComponent>()->CollisionStart();
	gameObject.lock()->GetGameComponent<Enemy>()->SetIsDead(false);
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform->RollIdentity();

	auto spawnEffect = GetManager().lock()->AddObjectFromCereal("SpawnEffect");
	spawnEffect.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
	spawnEffect.lock()->transform->SetLocalScale(10.0f);
	auto spawnEffectComponent = spawnEffect.lock()->GetGameComponent<SpawnEffect>();
	spawnEffectComponent->SetColor(GameSettings::ENEMY_COLOR);
	spawnEffectComponent->SetLife(60);
}

void ButiEngine::OutsideCrystal::Disappear()
{
	if (!m_isAppear) { return; }

	gameObject.lock()->transform->SetLocalScale(0.0f);

	m_isAppear = false;
	gameObject.lock()->GetGameComponent<Collision::ColliderComponent>()->CollisionStop();

	m_vlp_appearIntervalTimer->Start();
}

void ButiEngine::OutsideCrystal::SpawnStalker()
{
	if (m_vwp_waveManager.lock()->IsClearAnimation()) { return; }

	std::uint16_t spawnStalkerCount = m_spwanStalkerCount;
	if (m_vwp_waveManager.lock()->IsTutorial())
	{
		spawnStalkerCount = 1;
	}

	auto stalkerCenter = gameObject.lock()->transform->Clone();
	stalkerCenter->SetLocalScale(1.0f);

	auto stalkerTransform = ObjectFactory::Create<Transform>();
	stalkerTransform->SetBaseTransform(stalkerCenter);
	
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	stalkerTransform->SetLocalPosition(Vector3(0.0f, 0.0f, radius));

	float rollAngle = 360.0f / spawnStalkerCount;

	for (std::uint8_t i = 0; i < spawnStalkerCount; i++)
	{
		auto stalker = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");

		Vector3 pos = stalkerTransform->GetWorldPosition();
		stalker.lock()->transform->SetWorldPosition(pos);
		stalker.lock()->transform->SetLookAtRotation(stalkerCenter->GetWorldPosition());
		stalker.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

		Vector3 dir = stalkerCenter->GetFront();
		dir.y = m_knockBackY;
		dir.Normalize();

		stalker.lock()->AddGameComponent<KnockBack>(dir, m_stalerKnockBackForce, true, 300);

		stalkerCenter->RollLocalRotationY_Degrees(rollAngle);
	}
}

void ButiEngine::OutsideCrystal::Wait()
{
	if (m_vlp_appearIntervalTimer->Update())
	{
		m_vlp_appearIntervalTimer->Stop();
		m_vlp_appearIntervalTimer->Reset();
		std::uint16_t interval = m_appearIntervalFrame;
		if (m_vwp_waveManager.lock()->IsTutorial())
		{
			interval = 600;
		}

		m_vlp_appearIntervalTimer->ChangeCountFrame(interval);

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
