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
		m_rockShotRate = 30;
	}
	else
	{
		//通常時
		m_rockShotRate = 90;
	}

	if (m_rockShotCount < m_rockShotRate)
	{
		m_rockShotCount++;
	}
	else
	{
		m_rockShotCount = 0;
		ShotVolcanoRock();
		AddPredictedPoint();
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
	m_rockShotRate = 90;
	m_isHeatUp = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Volcano::Clone()
{
	return ObjectFactory::Create<Enemy_Volcano>();
}

void ButiEngine::Enemy_Volcano::ShotVolcanoRock()
{
	//全体的に要調整

	m_randomVelocity = Vector3(0, 0, 0);
	while (m_randomVelocity == Vector3(0, 0, 0))
	{
		m_randomVelocity.x = ButiRandom::GetInt(-5, 5);
		m_randomVelocity.z = ButiRandom::GetInt(-5, 5);
	}
	m_randomVelocity.Normalize();
	m_randomVelocity.y = 2;

	Vector3 position = gameObject.lock()->transform->GetWorldPosition();

	//火山岩を出す
	auto volcanoRock = GetManager().lock()->AddObjectFromCereal("VolcanoRock");
	volcanoRock.lock()->transform->SetWorldPosition(position);
	volcanoRock.lock()->GetGameComponent<VolcanoRock>()->SetVelocity(m_randomVelocity);
}

void ButiEngine::Enemy_Volcano::AddPredictedPoint()
{
	Vector3 randomVelocity = m_randomVelocity;
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();

	//予測地点を出す
	randomVelocity *= 6.0f;
	randomVelocity.y = 0;
	position += randomVelocity;
	position.y -= 1.0f;
	auto predictedPoint = GetManager().lock()->AddObjectFromCereal("PredictedPoint");
	predictedPoint.lock()->transform->SetWorldPosition(position);
}
