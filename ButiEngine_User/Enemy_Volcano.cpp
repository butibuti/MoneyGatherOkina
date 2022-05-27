#include "stdafx_u.h"
#include "Enemy_Volcano.h"
#include "VolcanoRock.h"
#include "Enemy.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"
#include "EnemyScaleAnimationComponent.h"
#include "PredictedPoint.h"
#include "WarningMark.h"
#include "SoundPlayerComponent.h"

#include "InputManager.h"

std::int32_t ButiEngine::Enemy_Volcano::m_progressPoint = 100;
std::int32_t ButiEngine::Enemy_Volcano::m_pocketCount = 8;
float ButiEngine::Enemy_Volcano::m_createPocketRadius = 4.0f;
float ButiEngine::Enemy_Volcano::m_vibrationCapacity = 1000.0f;
float ButiEngine::Enemy_Volcano::m_vibrationResistance = 3.0f;

float ButiEngine::Enemy_Volcano::m_knockBackForce = 0.3f;
std::int32_t ButiEngine::Enemy_Volcano::m_knockBackFrame = 30;

void ButiEngine::Enemy_Volcano::OnUpdate()
{
	//if (InputManager::IsTriggerDecideKey())
	//{
	//	ShotVolcanoRock();
	//}

	if (IsDetectionPlayer())
	{
		m_vwp_warningMark.lock()->Appear();
		//激化時
		m_rockShotRate = 30;
	}
	else
	{
		m_vwp_warningMark.lock()->Disappear();
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

void ButiEngine::Enemy_Volcano::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Volcano");

	m_vwp_warningMark = GetManager().lock()->AddObjectFromCereal("WarningMark").lock()->GetGameComponent<WarningMark>();
	m_vwp_warningMark.lock()->SetParent(gameObject);

	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	
	m_vlp_rockShotTimer->Start();

	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_defaultScale = Vector3(1, 1, 1);
	m_currentScale = m_defaultScale;
	m_previousScale = m_currentScale;
	m_detectionRange = 5.0f;

	m_rockShotRate = 90;

	m_vlp_rockShotTimer->ChangeCountFrame(m_rockShotRate);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Volcano::Clone()
{
	return ObjectFactory::Create<Enemy_Volcano>();
}

void ButiEngine::Enemy_Volcano::Dead()
{
	if (m_vwp_warningMark.lock())
	{
		m_vwp_warningMark.lock()->Dead();
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

	//ショット音再生
	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Kazan_Fire.wav"));
}

void ButiEngine::Enemy_Volcano::AddPredictedPoint()
{
	Vector3 randomVelocity = m_randomVelocity;
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();

	//予測地点を出す
	randomVelocity *= 6.5f;
	randomVelocity.y = 0;
	position += randomVelocity;
	auto predictedPoint = GetManager().lock()->AddObjectFromCereal("PredictedPoint");
	predictedPoint.lock()->transform->SetWorldPosition(position);
	predictedPoint.lock()->GetGameComponent<PredictedPoint>()->SetLife(105);
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
	m_vlp_enemy->SetProgressPoint(m_progressPoint);
	m_vlp_enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	m_vlp_enemy->SetVibrationCapacity(m_vibrationCapacity);
	m_vlp_enemy->SetVibrationResistance(m_vibrationResistance);
	m_vlp_enemy->SetKnockBackForce(m_knockBackForce);
	m_vlp_enemy->SetKnockBackFrame(m_knockBackFrame);
	m_vlp_enemy->SetWeight(1000.0f);
}

bool ButiEngine::Enemy_Volcano::IsDetectionPlayer()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();

	float rangeSqr = m_detectionRange * m_detectionRange;
	float distanceSqr = (pos - playerPos).GetLengthSqr();

	return distanceSqr <= rangeSqr;
}
