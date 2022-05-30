#include "stdafx_u.h"
#include "TutorialCrystalSpawner.h"

void ButiEngine::TutorialCrystalSpawner::OnUpdate()
{
}

void ButiEngine::TutorialCrystalSpawner::OnSet()
{
}

void ButiEngine::TutorialCrystalSpawner::OnRemove()
{
}

void ButiEngine::TutorialCrystalSpawner::OnShowUI()
{
}

void ButiEngine::TutorialCrystalSpawner::Start()
{
	m_initCrystalCount = 10;
	m_crystalCount = m_initCrystalCount;

	CreateCrystals();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TutorialCrystalSpawner::Clone()
{
	return ObjectFactory::Create<TutorialCrystalSpawner>();
}

void ButiEngine::TutorialCrystalSpawner::CreateCrystals()
{
	auto crystalCenter = gameObject.lock()->transform->Clone();
	auto crystalTransform = ObjectFactory::Create<Transform>();
	crystalTransform->SetBaseTransform(crystalCenter);
	crystalTransform->SetLocalPosition(Vector3(0.0f, 0.0f, 20.0f));

	float rollAngle = 360.0f / m_initCrystalCount;

	for (std::uint8_t i = 0; i < m_initCrystalCount; i++)
	{
		auto crystal = GetManager().lock()->AddObjectFromCereal("TutorialCrystal");

		Vector3 pos = crystalTransform->GetWorldPosition();
		crystal.lock()->transform->SetWorldPosition(pos);
		Vector3 scale = crystal.lock()->transform->GetLocalScale();
		scale *= 0.5f;
		crystal.lock()->transform->SetLocalScale(scale);

		crystalCenter->RollLocalRotationY_Degrees(rollAngle);
	}
}
