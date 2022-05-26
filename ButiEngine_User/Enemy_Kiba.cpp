#include "stdafx_u.h"
#include "Enemy_Kiba.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"
#include "WarningMark.h"

std::int32_t ButiEngine::Enemy_Kiba::m_pocketCount = 8;
float ButiEngine::Enemy_Kiba::m_createPocketRadius = 3.0f;

void ButiEngine::Enemy_Kiba::OnUpdate()
{
	bool isDetectionPlayer = IsDetectionPlayer();
	if (isDetectionPlayer)
	{
		m_vwp_warningMark.lock()->Appear();
		//m_vlp_loiter->MoveStop();
		//m_vlp_lookAt->SetIsActive(false);
		LookAtPlayer();
	}
	else
	{
		m_vwp_warningMark.lock()->Disappear();
		m_rotationAngle = 0.0f;
		//m_vlp_lookAt->SetIsActive(true);
		//m_vlp_loiter->MoveStart();
	}

	HaneAnimation(isDetectionPlayer);
	FangAnimation(isDetectionPlayer);
}

void ButiEngine::Enemy_Kiba::OnSet()
{
	m_fangAnimationTimer = ObjectFactory::Create<RelativeTimer>(20);
}

void ButiEngine::Enemy_Kiba::OnRemove()
{
}

void ButiEngine::Enemy_Kiba::OnShowUI()
{
	GUI::BulletText("PocketCount");
	GUI::DragInt("##pocketCount", &m_pocketCount, 1, 0, 64);
	GUI::BulletText("PocketRadius");
	GUI::DragFloat("##radius", &m_createPocketRadius, 0.1f, 0.0f, 100.0f);

	GUI::BulletText("MaxRotationAngle");
	GUI::DragFloat("##maxRAngle", &m_maxRotationAngle, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("RotationAcceleration");
	GUI::DragFloat("##rAccel", &m_rotationAcceleration, 0.001f, 0.0f, 1.0f);
}

void ButiEngine::Enemy_Kiba::Start()
{
	auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Kiba");
	m_vwp_fang_R = drawObject.lock()->GetGameComponent<MeshDrawComponent>(1);
	m_vwp_fang_L = drawObject.lock()->GetGameComponent<MeshDrawComponent>(2);
	m_vwp_hane_L = drawObject.lock()->GetGameComponent<MeshDrawComponent>(3);
	m_vwp_hane_R = drawObject.lock()->GetGameComponent<MeshDrawComponent>(4);

	m_defaultHaneRotateY = 45.0f;
	m_currentHaneRotateY_L = 0;
	m_currentHaneRotateY_R = 0;
	m_vwp_hane_L.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentHaneRotateY_L);
	m_vwp_hane_R.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentHaneRotateY_R);

	m_fangAnimationTimer->Start();
	m_defaultFangRotateY = m_vwp_fang_R.lock()->GetTransform()->GetLocalRotation_Euler().GetDegrees().y;
	m_currentFangRotateY_L = m_defaultFangRotateY;
	m_currentFangRotateY_R = m_defaultFangRotateY;
	m_endFangRotateY_L = m_defaultFangRotateY - 30;
	m_endFangRotateY_R = m_defaultFangRotateY + 30;
	
	CreateFang();
	SetEnemyParameter();
	//SetLoiterParameter();
	//SetLookAtParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_vwp_warningMark = GetManager().lock()->AddObjectFromCereal("WarningMark").lock()->GetGameComponent<WarningMark>();
	m_vwp_warningMark.lock()->SetParent(gameObject);

	m_detectionRange = 10.0f;
	m_rotationAngle = 0.0f;
	m_rotationAcceleration = 0.03f;
	m_maxRotationAngle = 1.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Kiba::Clone()
{
	return ObjectFactory::Create<Enemy_Kiba>();
}

void ButiEngine::Enemy_Kiba::Dead()
{
	if (m_vwp_fang.lock())
	{
		m_vwp_fang.lock()->SetIsRemove(true);
	}

	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}

	if (m_vwp_warningMark.lock())
	{
		m_vwp_warningMark.lock()->Dead();
	}
}

void ButiEngine::Enemy_Kiba::LookAtPlayer()
{
	float rotationDirection = CalculateRotationDirection();
	m_rotationAngle += m_rotationAcceleration * rotationDirection * GameDevice::WorldSpeed;
	if (abs(m_rotationAngle) > m_maxRotationAngle)
	{
		if (m_rotationAngle > 0)
		{
			m_rotationAngle = m_maxRotationAngle;
		}
		else if(m_rotationAngle < 0)
		{
			m_rotationAngle = -m_maxRotationAngle;
		}
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();
	Vector3 dir = (playerPos - pos).GetNormalize();
	Vector3 front = gameObject.lock()->transform->GetFront();

	float angle = MathHelper::ToDegree(std::acos(front.Dot(dir)));
	if (angle < abs(m_rotationAngle))
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(rotationDirection * angle * GameDevice::WorldSpeed);
		m_rotationAngle = 0.0f;
		return;
	}

	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_rotationAngle * GameDevice::WorldSpeed);
}

void ButiEngine::Enemy_Kiba::CreateFang()
{
	m_vwp_fang = GetManager().lock()->AddObjectFromCereal("Kiba_Fang");
	m_vwp_fang.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	//子になってもスケールは変わらないようにする
	Vector3 scale = m_vwp_fang.lock()->transform->GetLocalScale() / gameObject.lock()->transform->GetLocalScale();
	m_vwp_fang.lock()->transform->SetLocalScale(scale);

	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float fangRadius = m_vwp_fang.lock()->transform->GetWorldScale().x * 0.5f;
	m_vwp_fang.lock()->transform->SetWorldPosition(gameObject.lock()->transform->GetLocalPosition() + gameObject.lock()->transform->GetFront() * (radius + fangRadius));
}

void ButiEngine::Enemy_Kiba::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	m_vlp_enemy->SetVibrationCapacity(1000.0f);
	m_vlp_enemy->SetVibrationResistance(0.5f);
	m_vlp_enemy->SetExplosionScale(8.0f);
	m_vlp_enemy->SetWeight(1000.0f);
}

void ButiEngine::Enemy_Kiba::SetLoiterParameter()
{
	m_vlp_loiter = gameObject.lock()->GetGameComponent<Loiter>();
	m_vlp_loiter->SetMoveRange(5.0f);
	m_vlp_loiter->SetMaxMoveSpeed(0.1f);
	m_vlp_loiter->SetWaitFrame(60);
	m_vlp_loiter->SetAccelFrame(30);
	m_vlp_loiter->SetBrakeFrame(30);
}

void ButiEngine::Enemy_Kiba::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(0.1f);
}

void ButiEngine::Enemy_Kiba::HaneAnimation(const bool arg_isDetectionPlayer)
{
	if (arg_isDetectionPlayer)
	{
		m_currentHaneRotateY_L = MathHelper::Lerp(m_currentHaneRotateY_L, 0, 0.2f);
		m_currentHaneRotateY_R = MathHelper::Lerp(m_currentHaneRotateY_R, 0, 0.2f);
	}
	else
	{
		m_currentHaneRotateY_L = MathHelper::Lerp(m_currentHaneRotateY_L, -m_defaultHaneRotateY, 0.05f);
		m_currentHaneRotateY_R = MathHelper::Lerp(m_currentHaneRotateY_R, m_defaultHaneRotateY, 0.05f);
	}

	m_vwp_hane_L.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentHaneRotateY_L);
	m_vwp_hane_R.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentHaneRotateY_R);
}

void ButiEngine::Enemy_Kiba::FangAnimation(const bool arg_isDetectionPlayer)
{
	if (arg_isDetectionPlayer)
	{
		if (m_fangAnimationTimer->Update())
		{
			m_currentFangRotateY_L = m_defaultFangRotateY;
			m_currentFangRotateY_R = m_defaultFangRotateY;
		}
		else
		{
			m_currentFangRotateY_L = MathHelper::Lerp(m_currentFangRotateY_L, m_endFangRotateY_L, 0.15f);
			m_currentFangRotateY_R = MathHelper::Lerp(m_currentFangRotateY_R, m_endFangRotateY_R, 0.15f);
		}
	}
	else
	{
		m_currentFangRotateY_L = m_defaultFangRotateY;
		m_currentFangRotateY_R = m_defaultFangRotateY;
	}

	m_vwp_fang_L.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentFangRotateY_L);
	m_vwp_fang_R.lock()->GetTransform()->SetLocalRotationY_Degrees(m_currentFangRotateY_R);
}

float ButiEngine::Enemy_Kiba::CalculateRotationDirection()
{
	float result = 1.0f;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();
	Vector3 diff = playerPos - pos;
	Vector3 front = gameObject.lock()->transform->GetFront();

	float cross = diff.x * front.z - diff.z * front.x;
	if (cross <= 0)
	{
		result = -1.0f;
	}

	return result;
}

bool ButiEngine::Enemy_Kiba::IsDetectionPlayer()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();

	float rangeSqr = m_detectionRange * m_detectionRange;
	float distanceSqr = (pos - playerPos).GetLengthSqr();

	return distanceSqr <= rangeSqr;
}

