#include "stdafx_u.h"
#include "Flocking.h"
#include "Player.h"

float ButiEngine::Flocking::m_rotationSpeed = 0.05f;
float ButiEngine::Flocking::m_gatherWeight = 1.0f;
float ButiEngine::Flocking::m_cohesionWeight = 1.0f;
float ButiEngine::Flocking::m_alignmentWeight = 1.0f;
float ButiEngine::Flocking::m_separationWeight = 5.0f;

void ButiEngine::Flocking::OnUpdate()
{
	auto vec_workers = GetManager().lock()->GetGameObjects(GameObjectTag("Worker"));

	CalcMoveSpeed(vec_workers);
	CalcGatherDir();
	CalcCohesionVec(vec_workers);
	CalcALignmentVec(vec_workers);
	CalcSeparationVec(vec_workers);

	Move();
}

void ButiEngine::Flocking::OnSet()
{
}

void ButiEngine::Flocking::OnShowUI()
{
	GUI::BulletText("RotationSpeed");
	GUI::DragFloat("##rSpeed", &m_rotationSpeed, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Gather");
	GUI::DragFloat("##gather", &m_gatherWeight, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("Cohesion");
	GUI::DragFloat("##cohesion", &m_cohesionWeight, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("Alignment");
	GUI::DragFloat("##alignment", &m_alignmentWeight, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("Separation");
	GUI::DragFloat("##separation", &m_separationWeight, 0.01f, 0.0f, 10.0f);
}

void ButiEngine::Flocking::Start()
{
	if (m_vwp_player.lock() == nullptr)
	{
		m_vwp_player = GetManager().lock()->GetGameObject("Player");
	}

	m_moveSpeed = 0.1f;
	m_viewRadius = 25.0f;
	m_nearBorder = 25.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Flocking::Clone()
{
	return ObjectFactory::Create<Flocking>();
}

void ButiEngine::Flocking::CalcMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//プレイヤーに近い時プレイヤーの速度に合わせる
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	if ((pos - playerPos).GetLengthSqr() <= 100.0f)
	{
		m_moveSpeed = m_moveSpeed + 0.01f * (m_vwp_player.lock()->GetGameComponent<Player>()->GetMoveSpeed() - m_moveSpeed);
	}
	else
	{
		m_moveSpeed = m_moveSpeed + 0.01f * (0.15f - m_moveSpeed);
	}
}

void ButiEngine::Flocking::CalcGatherDir()
{
	//プレイヤーのいる方向を向く

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_gatherDir = (playerPos - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}

void ButiEngine::Flocking::CalcCohesionVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//群れの中心へ向かう

	m_cohesionVec = Vector3Const::Zero;
	Vector3 averagePos;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = arg_vec_workers.end();
	for (auto itr = arg_vec_workers.begin(); itr != end; ++itr)
	{
		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float sqrDistance = (pos - workerPos).GetLengthSqr();
		if (sqrDistance <= m_viewRadius)
		{
			workerNum++;
			averagePos += workerPos;
		}
	}

	if (workerNum > 1)
	{
		averagePos /= workerNum;
		m_cohesionVec = (averagePos - pos).GetNormalize();
	}
}

void ButiEngine::Flocking::CalcALignmentVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//周りと同じ方向を向く

	m_alignmentVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = arg_vec_workers.end();
	for (auto itr = arg_vec_workers.begin(); itr != end; ++itr)
	{
		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float sqrDistance = (pos - workerPos).GetLengthSqr();
		if (sqrDistance <= m_viewRadius)
		{
			workerNum++;
			m_alignmentVec += (*itr)->transform->GetFront();
		}
	}

	if (workerNum != 0)
	{
		m_alignmentVec /= workerNum;
	}
}

void ButiEngine::Flocking::CalcSeparationVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//近づきすぎたWorkerと離れる方向を向く

	m_separationVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	std::uint8_t nearWorkerNum = 0;

	auto end = arg_vec_workers.end();
	for (auto itr = arg_vec_workers.begin(); itr != end; ++itr)
	{
		if (gameObject.lock() == (*itr)) { continue; }

		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float sqrDistance = (pos - workerPos).GetLengthSqr();
		if (sqrDistance <= m_nearBorder)
		{
			nearWorkerNum++;
			Vector3 diff = pos - workerPos;
			m_separationVec += diff / diff.GetLengthSqr();
		}
	}

	if (nearWorkerNum != 0)
	{
		m_separationVec /= nearWorkerNum;
		m_separationVec.Normalize();
	}
}

void ButiEngine::Flocking::Move()
{
	Vector3 dir = (m_gatherDir * m_gatherWeight);
	dir += (m_cohesionVec * m_cohesionWeight) + (m_alignmentVec * m_alignmentWeight) + (m_separationVec * m_separationWeight);
	dir.y = 0.0f;
	dir.Normalize();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	auto target = gameObject.lock()->transform.Clone();
	target->SetLookAtRotation(pos + dir);
	auto rotation = MathHelper::LearpQuat(gameObject.lock()->transform->GetLocalRotation().ToQuat(), target->GetLocalRotation().ToQuat(), m_rotationSpeed);
	gameObject.lock()->transform->SetLocalRotation(rotation.ToMatrix());

	gameObject.lock()->transform->Translate(gameObject.lock()->transform->GetFront() * m_moveSpeed);
}
