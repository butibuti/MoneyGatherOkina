#include "stdafx_u.h"
#include "Stick.h"
#include "Pocket.h"
#include "SeparateDrawObject.h"
#include "ParticleGenerater.h"

void ButiEngine::Stick::OnUpdate()
{
	if (!m_vwp_pocket.lock()) { return; }

	auto parent = gameObject.lock()->transform->GetBaseTransform();
	Vector3 defaultScale = Vector3(0.5f, 0.5f, 0.5f);
	Vector3 scale = defaultScale;
	if (parent)
	{
		scale = scale / parent->GetWorldScale();
	}
	gameObject.lock()->transform->SetLocalScale(scale);

	KeepDistance();
	if (m_vwp_center.lock())
	{
		CheckMoveFinish();
	}
}

void ButiEngine::Stick::OnSet()
{
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("BillBoardParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_isPocketCatch = false;
	m_isMoveToPocket = false;
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

	KeepDistance();
	CreateCenter();

	/*m_vlp_rotationTarget = m_vlp_center->Clone();
	m_vlp_rotationTarget->SetLookAtRotation(m_vwp_pocket.lock()->transform->GetWorldPosition());*/

	m_isMoveToPocket = true;
}

void ButiEngine::Stick::Dead()
{
	if (m_vwp_center.lock())
	{
		m_vwp_center.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Stick::KeepDistance()
{
	auto enemy = m_vwp_pocket.lock()->GetGameComponent<Pocket>()->GetEnemy();
	auto drawObjectEnemy = enemy.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	float enemyRadius = drawObjectEnemy.lock()->transform->GetWorldScale().x;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	Vector3 enemyPos = enemy.lock()->transform->GetLocalPosition();
	Vector3 dir = pos - enemyPos;
	dir.y = 0.0f;
	dir.Normalize();
	Vector3 newPos = enemyPos + dir * (radius + enemyRadius);

	gameObject.lock()->transform->SetWorldPosition(newPos);
	
}

void ButiEngine::Stick::CheckMoveFinish()
{
	Vector3 front = m_vwp_center.lock()->transform->GetFront();
	Vector3 targetDir = (m_vwp_pocket.lock()->transform->GetWorldPosition() - m_vwp_center.lock()->transform->GetWorldPosition()).GetNormalize();
	float dot = front.Dot(targetDir);
	dot = min(dot, 1.0f);
	float angle = MathHelper::ToDegree(std::acos(dot));

	if (angle < 1.0f)
	{
		//gameObject.lock()->transform->SetBaseTransform(m_vwp_pocket.lock()->transform);
		//gameObject.lock()->transform->SetLocalPosition(Vector3Const::Zero);
		gameObject.lock()->transform->SetBaseTransform(m_vwp_pocket.lock()->GetGameComponent<Pocket>()->GetEnemy().lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform);
		//gameObject.lock()->transform->SetBaseTransform(nullptr);
		m_vwp_center.lock()->SetIsRemove(true);
		m_vwp_center = Value_weak_ptr<GameObject>();
	}
	else if (angle < 5.0f)
	{
		if (!m_isPocketCatch)
		{
			m_isPocketCatch = true;
			auto position = gameObject.lock()->transform->GetLocalPosition();
			m_vwp_particleGenerater.lock()->CatchParticles(position, gameObject.lock()->transform);
		}
	}
}

void ButiEngine::Stick::CreateCenter()
{
	m_vwp_center = GetManager().lock()->AddObjectFromCereal("Center");
	m_vwp_center.lock()->SetObjectName(m_vwp_center.lock()->GetGameObjectName() + "_" + gameObject.lock()->GetGameObjectName());

	auto enemy = m_vwp_pocket.lock()->GetGameComponent<Pocket>()->GetEnemy();
	m_vwp_center.lock()->transform->SetBaseTransform(enemy.lock()->transform, true);
	m_vwp_center.lock()->transform->SetLookAtRotation(gameObject.lock()->transform->GetLocalPosition());
	gameObject.lock()->transform->SetBaseTransform(m_vwp_center.lock()->transform);

	auto lookAt = m_vwp_center.lock()->GetGameComponent<LookAtComponent>();
	lookAt->SetLookTarget(m_vwp_pocket.lock()->transform);
	lookAt->SetSpeed(0.1f);
}
