#include "stdafx_u.h"
#include "TutorialCrystal.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"
#include "Enemy.h"
#include "WaveManager.h"
#include "WorkerSpawner.h"
#include "Worker.h"
#include "TutorialCrystalSpawner.h"
#include "FlockingLeader.h"
#include "SpawnEffect.h"
#include "GameSettings.h"
#include "SoundPlayerComponent.h"
#include "TutorialUI.h"

std::int32_t ButiEngine::TutorialCrystal::m_progressPoint = 0;
std::int32_t ButiEngine::TutorialCrystal::m_pocketCount = 0;
float ButiEngine::TutorialCrystal::m_createPocketRadius = 3.0f;
float ButiEngine::TutorialCrystal::m_vibrationCapacity = 1500.0f;
float ButiEngine::TutorialCrystal::m_vibrationResistance = 0.0f;

void ButiEngine::TutorialCrystal::OnUpdate()
{
}

void ButiEngine::TutorialCrystal::OnSet()
{
	m_isLast = false;
}

void ButiEngine::TutorialCrystal::OnRemove()
{
}

void ButiEngine::TutorialCrystal::OnShowUI()
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

void ButiEngine::TutorialCrystal::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("TutorialCrystal");
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1000.0f);

	m_vwp_crystalSpawner = GetManager().lock()->GetGameObject("TutorialCrystalSpawner").lock()->GetGameComponent<TutorialCrystalSpawner>();

	SetEnemyParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TutorialCrystal::Clone()
{
	return ObjectFactory::Create<TutorialCrystal>();
}

void ButiEngine::TutorialCrystal::Dead()
{
	if (m_isLast)
	{
		GetManager().lock()->GetGameObject("WorkerSpawner").lock()->GetGameComponent<WorkerSpawner>()->RandomSpawnStart();
		GetManager().lock()->GetGameObject("TutorialVibrationObject25").lock()->GetGameComponent<Worker>()->Dead();
		GetManager().lock()->GetGameObject("TutorialVibrationObject25_1").lock()->GetGameComponent<Worker>()->Dead();
		GetManager().lock()->GetGameObject("TutorialVibrationObject75").lock()->GetGameComponent<Worker>()->Dead();
		GetManager().lock()->GetGameObject("TutorialVibrationObject75_1").lock()->GetGameComponent<Worker>()->Dead();
		GetManager().lock()->GetGameObject("FlockingLeader").lock()->GetGameComponent<FlockingLeader>()->TutorialStart();

		GetManager().lock()->GetGameObject("TutorialUI_Gather").lock()->GetGameComponent<TutorialUI>()->Appear();
	}
	else
	{
		m_vwp_crystalSpawner.lock()->RemoveCrystal();

		if (m_vwp_crystalSpawner.lock()->GetCrystalCount() == 0)
		{
			auto crystal = GetManager().lock()->AddObjectFromCereal("TutorialCrystal");
			crystal.lock()->transform->SetLocalPosition(0.0f);
			crystal.lock()->GetGameComponent<TutorialCrystal>()->SetIsLast(true);

			auto spawnEffect = GetManager().lock()->AddObjectFromCereal("SpawnEffect");
			spawnEffect.lock()->transform->SetLocalPosition(0.0f);
			spawnEffect.lock()->transform->SetLocalScale(10.0f);
			auto spawnEffectComponent = spawnEffect.lock()->GetGameComponent<SpawnEffect>();
			spawnEffectComponent->SetColor(GameSettings::WORKER_COLOR);
			spawnEffectComponent->SetLife(60);

			GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>()->PlaySE(SoundTag("Sound/Beam.wav"), 3.0f);
		}
	}
}

void ButiEngine::TutorialCrystal::SetEnemyParameter()
{
	auto enemy = gameObject.lock()->GetGameComponent<Enemy>();
	enemy->SetProgressPoint(m_progressPoint);
	enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	enemy->SetVibrationCapacity(m_vibrationCapacity);
	enemy->SetVibrationResistance(m_vibrationResistance);
	enemy->SetWeight(10000.0f);
}
