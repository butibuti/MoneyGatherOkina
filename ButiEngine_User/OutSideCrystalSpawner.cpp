#include "stdafx_u.h"
#include "OutSideCrystalSpawner.h"
#include "OutsideCrystal.h"
#include "WaveManager.h"

std::int32_t ButiEngine::OutsideCrystalSpawner::m_startCrystalCount = 3;
float ButiEngine::OutsideCrystalSpawner::m_startAngle = 0.0f;
float ButiEngine::OutsideCrystalSpawner::m_spawnRadius = 30.0f;
std::int32_t ButiEngine::OutsideCrystalSpawner::m_phaseCount = 3;
std::vector<std::int32_t> ButiEngine::OutsideCrystalSpawner::m_vec_phaseActivePoints = { 0, 1000, 2000 };

void ButiEngine::OutsideCrystalSpawner::OnUpdate()
{
	std::uint8_t size = m_vec_phaseActivePoints.size();
	for (std::uint8_t i = 0; i < size; i++)
	{
		if (m_vec_phaseActivePoints[i] <= m_vwp_waveManager.lock()->GetPoint())
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

	auto end = m_vec_phaseActivePoints.end();
	std::uint8_t phase = 1;
	for (auto itr = m_vec_phaseActivePoints.begin(); itr != end; ++itr)
	{
		GUI::BulletText(u8"フェーズ%dに移行するポイント", phase);
		std::string str = "##phase" + std::to_string(phase);
		GUI::DragInt(str, *itr, 1, 0, 1000000);

		phase++;
	}
}

void ButiEngine::OutsideCrystalSpawner::Start()
{
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_vec_crystals.resize(m_phaseCount);
	m_vec_phaseActivePoints.resize(m_phaseCount);
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
	auto crystalCenter = gameObject.lock()->transform->Clone();
	auto crystalTransform = ObjectFactory::Create<Transform>();
	crystalTransform->SetBaseTransform(crystalCenter);
	crystalTransform->SetLocalPosition(Vector3(0.0f, 0.0f, m_spawnRadius));
	crystalCenter->RollLocalRotationY_Degrees(m_startAngle);

	Vector3 centerScale = crystalCenter->GetLocalScale();

	for (std::uint8_t i = 0; i < m_phaseCount; i++)
	{
		std::uint8_t crystalCount = i * m_startCrystalCount;
		crystalCount = max(crystalCount, m_startCrystalCount);

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
