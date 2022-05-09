#include "stdafx_u.h"
#include "Enemy_Kiba.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"

void ButiEngine::Enemy_Kiba::OnUpdate()
{
	if (m_vlp_enemy->IsVibrate())
	{
		m_vlp_loiter->MoveStop();
		LookAtPlayer();
	}
	else
	{
		m_vlp_loiter->MoveStart();
	}
}

void ButiEngine::Enemy_Kiba::OnSet()
{
}

void ButiEngine::Enemy_Kiba::OnRemove()
{
	if (m_vwp_damageArea.lock())
	{
		m_vwp_damageArea.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Enemy_Kiba::OnShowUI()
{
}

void ButiEngine::Enemy_Kiba::Start()
{
	CreateDamageArea();
	SetEnemyParameter();
	SetLoiterParameter();
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(100.0f);

	m_rotationSpeed = 0.0f;
	m_rotationAcceleration = 0.03f;
	m_maxRotationSpeed = 1.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Kiba::Clone()
{
	return ObjectFactory::Create<Enemy_Kiba>();
}

void ButiEngine::Enemy_Kiba::LookAtPlayer()
{
	m_rotationSpeed += m_rotationAcceleration;
	m_rotationSpeed = min(m_rotationSpeed, m_maxRotationSpeed);

	float rotationDirection = CalculateRotationDirection();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();
	Vector3 diff = playerPos - pos;
	Vector3 front = gameObject.lock()->transform->GetFront();

	float angle = MathHelper::ToDegree(std::acos(front.Dot(diff) / (front.GetLength() * diff.GetLength())));
	if (angle < m_rotationSpeed)
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(rotationDirection * angle);
		m_rotationSpeed = 0.0f;
		return;
	}

	gameObject.lock()->transform->RollLocalRotationY_Degrees(rotationDirection * m_rotationSpeed);
}

void ButiEngine::Enemy_Kiba::CreateDamageArea()
{
	m_vwp_damageArea = GetManager().lock()->AddObjectFromCereal("DamageArea");
	m_vwp_damageArea.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	//子になってもスケールは変わらないようにする
	Vector3 scale = m_vwp_damageArea.lock()->transform->GetLocalScale() / gameObject.lock()->transform->GetLocalScale();
	m_vwp_damageArea.lock()->transform->SetLocalScale(scale);

	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float damageAreaRadius = m_vwp_damageArea.lock()->transform->GetWorldScale().x * 0.5f;
	m_vwp_damageArea.lock()->transform->SetWorldPosition(gameObject.lock()->transform->GetLocalPosition() + gameObject.lock()->transform->GetFront() * (radius + damageAreaRadius));
}

void ButiEngine::Enemy_Kiba::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(8);
	m_vlp_enemy->RemovePocket(0);
	m_vlp_enemy->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	m_vlp_enemy->SetVibrationCapacity(1000.0f);
	m_vlp_enemy->SetVibrationResistance(3.0f);
}

void ButiEngine::Enemy_Kiba::SetLoiterParameter()
{
	m_vlp_loiter = gameObject.lock()->GetGameComponent<Loiter>();
	m_vlp_loiter->SetMoveRange(30.0f);
	m_vlp_loiter->SetMaxMoveSpeed(0.1f);
	m_vlp_loiter->SetWaitFrame(60);
	m_vlp_loiter->SetAccelFrame(30);
	m_vlp_loiter->SetBrakeFrame(30);
}

float ButiEngine::Enemy_Kiba::CalculateRotationDirection()
{
	float result = 1.0f;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 playerPos = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition();
	Vector3 diff = playerPos - pos;
	Vector3 front = gameObject.lock()->transform->GetFront();

	float cross = diff.x * front.z - diff.z * front.x;
	if (cross < 0)
	{
		result = -1.0f;
	}

	return result;
}

