#include "stdafx_u.h"
#include "Flocking.h"

float ButiEngine::Flocking::m_rotationSpeed = 0.05f;
float ButiEngine::Flocking::m_gatherWeight = 3.0f;
float ButiEngine::Flocking::m_cohesionWeight = 1.0f;
float ButiEngine::Flocking::m_alignmentWeight = 1.0f;
float ButiEngine::Flocking::m_separationWeight = 3.0f;

void ButiEngine::Flocking::OnUpdate()
{
	auto vec_workers = GetManager().lock()->GetGameObjects(GameObjectTag("Worker"));

	CalcMoveSpeed(vec_workers);
	CalcGatherDir();
	CalcCohesionDir(vec_workers);
	CalcALignmentDir(vec_workers);
	CalcSeparationDir(vec_workers);

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
	m_nearBorder = 4.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Flocking::Clone()
{
	return ObjectFactory::Create<Flocking>();
}

void ButiEngine::Flocking::CalcMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//プレイヤーとの距離に応じてスピードを変える
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveSpeed = Easing::EaseOutCirc(MathHelper::Clamp((playerPos - pos).GetLengthSqr() - 4.0f, 0.0f, 100.0f) / 100.0f) * 0.1f;
}

void ButiEngine::Flocking::CalcGatherDir()
{
	//プレイヤーのいる方向を向く

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_gatherDir = (playerPos - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}

void ButiEngine::Flocking::CalcCohesionDir(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//群れの中心へ向かう

	m_cohesionDir = Vector3Const::Zero;
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
		m_cohesionDir = (averagePos - pos).GetNormalize();
	}
}

void ButiEngine::Flocking::CalcALignmentDir(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//周りと同じ方向を向く

	m_alignmentDir = Vector3Const::Zero;
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
			m_alignmentDir += (*itr)->transform->GetFront();
		}
	}

	if (workerNum != 0)
	{
		m_alignmentDir /= workerNum;
	}
}

void ButiEngine::Flocking::CalcSeparationDir(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//近づきすぎたWorkerと離れる方向を向く

	m_separationDir = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	auto end = arg_vec_workers.end();
	for (auto itr = arg_vec_workers.begin(); itr != end; ++itr)
	{
		if (gameObject.lock() == (*itr)) { continue; }

		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float sqrDistance = (pos - workerPos).GetLengthSqr();
		if (sqrDistance <= m_nearBorder)
		{
			m_separationDir = (pos - workerPos).GetNormalize();
			break;
		}
	}
}

void ButiEngine::Flocking::Move()
{
	Vector3 dir = (m_gatherDir * m_gatherWeight);
	dir += (m_cohesionDir * m_cohesionWeight) + (m_alignmentDir * m_alignmentWeight) + (m_separationDir * m_separationWeight);
	dir.y = 0.0f;
	dir.Normalize();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	auto target = gameObject.lock()->transform.Clone();
	target->SetLookAtRotation(pos + dir);
	auto rotation = MathHelper::LearpQuat(gameObject.lock()->transform->GetLocalRotation().ToQuat(), target->GetLocalRotation().ToQuat(), m_rotationSpeed);
	gameObject.lock()->transform->SetLocalRotation(rotation.ToMatrix());

	gameObject.lock()->transform->Translate(gameObject.lock()->transform->GetFront() * m_moveSpeed);
}
