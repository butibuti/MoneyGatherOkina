#include "stdafx_u.h"
#include "Stick.h"
#include "Pocket.h"

void ButiEngine::Stick::OnUpdate()
{
	if (!m_vwp_pocket.lock()) { return; }
	MoveToPocket();
}

void ButiEngine::Stick::OnSet()
{
	m_vlp_center = ObjectFactory::Create<Transform>();
}

void ButiEngine::Stick::OnShowUI()
{
}

void ButiEngine::Stick::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Stick::Clone()
{
	return ObjectFactory::Create<Stick>();
}

void ButiEngine::Stick::SetPocket(Value_weak_ptr<GameObject> arg_vwp_pocket)
{
	m_vwp_pocket = arg_vwp_pocket;
	auto pocketComponent = m_vwp_pocket.lock()->GetGameComponent<Pocket>();
	pocketComponent->SetWorker(gameObject);

	MoveToEnemy();
	SetCenter();

	m_vlp_rotationTarget = m_vlp_center->Clone();
	m_vlp_rotationTarget->SetLookAtRotation(m_vwp_pocket.lock()->transform->GetWorldPosition());
}

void ButiEngine::Stick::Move()
{
}

void ButiEngine::Stick::MoveToEnemy()
{
	auto pocketComponent = m_vwp_pocket.lock()->GetGameComponent<Pocket>();
	auto enemy = pocketComponent->GetEnemy();

	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float enemyRadius = enemy.lock()->transform->GetLocalScale().x * 0.5f;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 enemyPos = enemy.lock()->transform->GetLocalPosition();
	Vector3 dir = (pos - enemyPos).GetNormalize();
	Vector3 newPos = enemyPos + dir * (radius + enemyRadius);

	gameObject.lock()->transform->SetLocalPosition(newPos);
	
}

void ButiEngine::Stick::MoveToPocket()
{
	constexpr float rotationSpeed = 0.1f;
	auto rotation = MathHelper::LearpQuat(m_vlp_center->GetLocalRotation().ToQuat(), m_vlp_rotationTarget->GetLocalRotation().ToQuat(), rotationSpeed);
	m_vlp_center->SetLocalRotation(rotation.ToMatrix());
}

void ButiEngine::Stick::SetCenter()
{
	auto enemy = m_vwp_pocket.lock()->GetGameComponent<Pocket>()->GetEnemy();

	m_vlp_center->SetBaseTransform(enemy.lock()->transform, true);

	//enemyのスケールを打ち消す
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f) / enemy.lock()->transform->GetLocalScale();
	m_vlp_center->SetLocalScale(scale);

	m_vlp_center->SetLookAtRotation(gameObject.lock()->transform->GetLocalPosition());

	Vector3 a = gameObject.lock()->transform->GetWorldPosition();
	gameObject.lock()->transform->SetBaseTransform(m_vlp_center);
	Vector3 b = gameObject.lock()->transform->GetWorldPosition();
}
