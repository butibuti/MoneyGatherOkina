#include "stdafx_u.h"
#include "Enemy_Volcano.h"
#include "VolcanoRock.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"
#include "EnemyScaleAnimationComponent.h"

#include "InputManager.h"

void ButiEngine::Enemy_Volcano::OnUpdate()
{
	//if (InputManager::IsTriggerDecideKey())
	//{
	//	ShotVolcanoRock();
	//}

	if (m_vlp_enemy->IsVibrate())
	{
		//激化時
		m_rockShotRate = 30;
	}
	else
	{
		//通常時
		m_rockShotRate = 90;
	}

	m_vlp_rockShotTimer->ChangeCountFrame(m_rockShotRate);

	if (m_vlp_rockShotTimer->Update())
	{
		ShotVolcanoRock();
		AddPredictedPoint();
		m_previousScale = Vector3(0.5f, 1.3f, 0.5f);
	}

	ScaleAnimation();
}

void ButiEngine::Enemy_Volcano::OnSet()
{
	m_vlp_rockShotTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::Enemy_Volcano::OnRemove()
{
}

void ButiEngine::Enemy_Volcano::OnShowUI()
{
}

void ButiEngine::Enemy_Volcano::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Volcano");
	
	m_vlp_rockShotTimer->Start();

	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_defaultScale = Vector3(1, 1, 1);
	m_currentScale = m_defaultScale;
	m_previousScale = m_currentScale;

	m_rockShotRate = 90;

	m_vlp_rockShotTimer->ChangeCountFrame(m_rockShotRate);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Volcano::Clone()
{
	return ObjectFactory::Create<Enemy_Volcano>();
}

void ButiEngine::Enemy_Volcano::Dead()
{
	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}
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
	m_randomVelocity.y = 2.5f;

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
	randomVelocity *= 5.0f;
	randomVelocity.y = 0;
	position += randomVelocity;
	position.y -= 1.0f;
	auto predictedPoint = GetManager().lock()->AddObjectFromCereal("PredictedPoint");
	predictedPoint.lock()->transform->SetWorldPosition(position);
}

void ButiEngine::Enemy_Volcano::ScaleAnimation()
{
	if (!m_vwp_scaleAnimationComponent.lock())
	{
		m_vwp_scaleAnimationComponent = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<EnemyScaleAnimationComponent>();
		return;
	}

	float lerpScale = 0.05f * GameDevice::WorldSpeed;
	m_previousScale.x = m_previousScale.x * (1.0f - lerpScale) + m_currentScale.x * lerpScale;
	m_previousScale.y = m_previousScale.y * (1.0f - lerpScale) + m_currentScale.y * lerpScale;
	m_previousScale.z = m_previousScale.z * (1.0f - lerpScale) + m_currentScale.z * lerpScale;

	m_vwp_scaleAnimationComponent.lock()->SetAnotherScale(m_previousScale);
}

void ButiEngine::Enemy_Volcano::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(8);
	m_vlp_enemy->SetVibrationCapacity(1000.0f);
	m_vlp_enemy->SetVibrationResistance(3.0f);
	m_vlp_enemy->SetExplosionScale(10.0f);
	m_vlp_enemy->SetWeight(1000.0f);
}
