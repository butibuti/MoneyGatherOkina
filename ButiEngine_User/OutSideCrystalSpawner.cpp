#include "stdafx_u.h"
#include "OutSideCrystalSpawner.h"
#include "OutsideCrystal.h"
#include "WaveManager.h"

std::int32_t ButiEngine::OutsideCrystalSpawner::m_startCrystalCount = 10;
float ButiEngine::OutsideCrystalSpawner::m_startAngle = 0.0f;
float ButiEngine::OutsideCrystalSpawner::m_spawnRadius = 32.0f;
std::int32_t ButiEngine::OutsideCrystalSpawner::m_phaseCount = 1;
std::vector<float> ButiEngine::OutsideCrystalSpawner::m_vec_phaseActiveProgress = { 0.0f };

void ButiEngine::OutsideCrystalSpawner::OnUpdate()
{
	std::uint8_t size = m_vec_phaseActiveProgress.size();
	for (std::uint8_t i = 0; i < size; i++)
	{
		if (m_vec_phaseActiveProgress[i] <= m_vwp_waveManager.lock()->GetProgress())
		{
			ActivePhase(i);
		}
	}
}

void ButiEngine::OutsideCrystalSpawner::OnSet()
{
}

void ButiEngine::OutsideCrystalSpawner::OnRemove()
{
}

void ButiEngine::OutsideCrystalSpawner::OnShowUI()
{
	GUI::BulletText(u8"開始時に出現させる数");
	GUI::DragInt("##startCount", &m_startCrystalCount, 1, 2, 5);

	GUI::BulletText(u8"起点の角度");
	GUI::DragFloat("##startAngle", &m_startAngle, 0.1f, 0.0f, 360.0f);

	GUI::BulletText(u8"半径");
	GUI::DragFloat("##radius", &m_spawnRadius, 0.1f, 1.0f, 100.0f);

	GUI::BulletText(u8"フェーズの数");
	GUI::DragInt("##phaseCount", &m_phaseCount, 1, 1, 10);

	auto end = m_vec_phaseActiveProgress.end();
	std::uint8_t phase = 1;
	for (auto itr = m_vec_phaseActiveProgress.begin(); itr != end; ++itr)
	{
		GUI::BulletText(u8"フェーズ%dに移行する進捗(0.0~1.0)", phase);
		std::string str = "##phase" + std::to_string(phase);
		GUI::DragFloat(str, *itr, 0.01f, 0.0f, 1.0f);

		phase++;
	}
}

void ButiEngine::OutsideCrystalSpawner::Start()
{
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_vec_crystals.resize(m_phaseCount);
	m_vec_phaseActiveProgress.resize(m_phaseCount);
	CreateCrystals();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::OutsideCrystalSpawner::Clone()
{
	return ObjectFactory::Create<OutsideCrystalSpawner>();
}

void ButiEngine::OutsideCrystalSpawner::ActivePhase(const std::int32_t arg_index)
{
	if (arg_index >= m_vec_crystals.size()) { return; }
	auto vec = m_vec_crystals[arg_index];
	auto end = vec.end();
	for (auto itr = vec.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetIsActive(true);
	}
}

void ButiEngine::OutsideCrystalSpawner::CreateCrystals()
{
	float spawnRadius = m_spawnRadius;
	std::uint16_t startCrystalCount = m_startCrystalCount;
	if (m_vwp_waveManager.lock()->IsTutorial())
	{
		startCrystalCount = 3;
		spawnRadius = 20.0f;
	}

	auto crystalCenter = gameObject.lock()->transform->Clone();
	auto crystalTransform = ObjectFactory::Create<Transform>();
	crystalTransform->SetBaseTransform(crystalCenter);
	crystalTransform->SetLocalPosition(Vector3(0.0f, 0.0f, spawnRadius));
	crystalCenter->RollLocalRotationY_Degrees(m_startAngle);

	Vector3 centerScale = crystalCenter->GetLocalScale();

	for (std::uint8_t i = 0; i < m_phaseCount; i++)
	{
		std::uint8_t crystalCount = i * startCrystalCount;
		crystalCount = max(crystalCount, startCrystalCount);

		float rollAngle = 360.0f / crystalCount;

		for (std::uint8_t j = 0; j < crystalCount; j++)
		{
			auto crystal = GetManager().lock()->AddObjectFromCereal("OutsideCrystal");

			Vector3 pos = crystalTransform->GetWorldPosition();
			crystal.lock()->transform->SetWorldPosition(pos);

			auto crystalComponent = crystal.lock()->GetGameComponent<OutsideCrystal>();
			crystalComponent->SetDefaultScale(crystal.lock()->transform->GetLocalScale());
			m_vec_crystals[i].push_back(crystalComponent);

			crystalCenter->RollLocalRotationY_Degrees(rollAngle);
		}

		if (i == 1)
		{
			centerScale += 0.075f;
			crystalCenter->SetLocalScale(centerScale);
		}

		float rollAngle_ = rollAngle / pow(2.0f, i + 1);
		rollAngle_ = min(rollAngle_, rollAngle / 2.0f);
		crystalCenter->RollLocalRotationY_Degrees(rollAngle_);
	}
}
