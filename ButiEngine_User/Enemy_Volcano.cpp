#include "stdafx_u.h"
#include "Enemy_Volcano.h"
#include "VolcanoRock.h"
#include "Enemy.h"
#include "Loiter.h"

#include "InputManager.h"

void ButiEngine::Enemy_Volcano::OnUpdate()
{
	//if (InputManager::IsTriggerDecideKey())
	//{
	//	ShotVolcanoRock();
	//}

	if (m_isHeatUp)
	{
		//激化時
		m_rockShotRate = 90;
	}
	else
	{
		//通常時
		m_rockShotRate = 120;
	}

	if (m_rockShotCount < m_rockShotRate)
	{
		m_rockShotCount++;
	}
	else
	{
		m_rockShotCount = 0;
		ShotVolcanoRock();
	}
}

void ButiEngine::Enemy_Volcano::OnSet()
{
}

void ButiEngine::Enemy_Volcano::OnShowUI()
{
}

void ButiEngine::Enemy_Volcano::Start()
{
	auto enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	enemyComponent->CreatePocket(8);
	enemyComponent->SetNearBorder(6.5f);
	enemyComponent->SetVibrationCapacity(1000.0f);
	enemyComponent->SetVibrationResistance(3.0f);

	m_rockShotCount = 0;
	m_rockShotRate = 120;
	m_isHeatUp = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Volcano::Clone()
{
	return ObjectFactory::Create<Enemy_Volcano>();
}

void ButiEngine::Enemy_Volcano::ShotVolcanoRock()
{
	//全体的に要調整

	Vector3 randomVelocity = Vector3(0, 5, 0);
	while (randomVelocity == Vector3(0, 5, 0))
	{
		randomVelocity.x = ButiRandom::GetInt(-5, 5);
		randomVelocity.z = ButiRandom::GetInt(-5, 5);
	}

	Vector3 position = gameObject.lock()->transform->GetLocalPosition();

	//火山岩を出す
	auto volcanoRock = GetManager().lock()->AddObjectFromCereal("VolcanoRock");
	volcanoRock.lock()->transform->SetLocalPosition(position);
	volcanoRock.lock()->GetGameComponent<VolcanoRock>()->SetVelocity(randomVelocity);

	//予測地点を出す
	randomVelocity.Normalize();
	randomVelocity *= 30;
	randomVelocity.y = 0;
	position += randomVelocity;
	auto predictedPoint = GetManager().lock()->AddObjectFromCereal("PredictedPoint");
	volcanoRock.lock()->transform->SetLocalPosition(position);
}
