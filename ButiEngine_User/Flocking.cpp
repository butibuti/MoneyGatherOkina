#include "stdafx_u.h"
#include "Flocking.h"
#include "Player.h"
#include "SeparateDrawObject.h"

float ButiEngine::Flocking::m_gatherWeight = 0.5f;
float ButiEngine::Flocking::m_cohesionWeight = 0.5f;
float ButiEngine::Flocking::m_alignmentWeight = 0.5f;
float ButiEngine::Flocking::m_separationWeight = 1.0f;
float ButiEngine::Flocking::m_avoidPlayerWeight = 1.0f;
float ButiEngine::Flocking::m_surroundWeight = 1.0f;
float ButiEngine::Flocking::m_viewRadius = 10.0f;
float ButiEngine::Flocking::m_nearBorder = 2.0f;

void ButiEngine::Flocking::OnUpdate()
{
	auto vec_workers = GetManager().lock()->GetGameObjects(GameObjectTag("Flocking"));

	CalcAveragePos(vec_workers);
	CalcMoveSpeed(vec_workers);
	CalcGatherVec();
	CalcCohesionVec(vec_workers);
	CalcALignmentVec(vec_workers);
	CalcSeparationVec(vec_workers);
	CalcAvoidPlayerVec(vec_workers);
	CalcSurroundVec(vec_workers);

	Move();
}

void ButiEngine::Flocking::OnSet()
{
	auto tag = GameObjectTag("Flocking");
	gameObject.lock()->SetGameObjectTag(tag);
}

void ButiEngine::Flocking::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Flocking"));
}

void ButiEngine::Flocking::OnShowUI()
{
	GUI::BulletText("Gather");
	GUI::DragFloat("##gather", &m_gatherWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Cohesion");
	GUI::DragFloat("##cohesion", &m_cohesionWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Alignment");
	GUI::DragFloat("##alignment", &m_alignmentWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Separation");
	GUI::DragFloat("##separation", &m_separationWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Avoid");
	GUI::DragFloat("##avoid", &m_avoidPlayerWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("Surround");
	GUI::DragFloat("##surround", &m_surroundWeight, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("ViewRadius");
	GUI::DragFloat("##viewRadius", &m_viewRadius, 0.01f, 0.0f, 100.0f);
	GUI::BulletText("NearBorder");
	GUI::DragFloat("##nearBorder", &m_nearBorder, 0.01f, 0.0f, 100.0f);
}

void ButiEngine::Flocking::Start()
{
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));

	m_rotationSpeed = 0.05f;
	m_moveSpeed = 0.1f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Flocking::Clone()
{
	return ObjectFactory::Create<Flocking>();
}

void ButiEngine::Flocking::CalcAveragePos(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	m_averagePos = Vector3Const::Zero;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = arg_vec_workers.end();
	for (auto itr = arg_vec_workers.begin(); itr != end; ++itr)
	{
		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float viewRadiusSqr = m_viewRadius * m_viewRadius;
		float distanceSqr = (pos - workerPos).GetLengthSqr();
		if (distanceSqr <= viewRadiusSqr)
		{
			workerNum++;
			m_averagePos += workerPos;
		}
	}

	if (workerNum > 1)
	{
		m_averagePos /= workerNum;
	}
}

void ButiEngine::Flocking::CalcMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//return;
	//プレイヤーに近い時プレイヤーの速度に合わせる
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();

	float nearBorderSqr = m_nearBorder * m_nearBorder;
	float distanceSqr = (playerPos - m_averagePos).GetLengthSqr();

	if (distanceSqr <= nearBorderSqr)
	{
		m_moveSpeed = MathHelper::Lerp(m_moveSpeed, m_vwp_player.lock()->GetGameComponent<Player>()->GetMoveSpeed(), 0.1f);
	}
	else
	{
		m_moveSpeed = MathHelper::Lerp(m_moveSpeed, 0.1f, 0.3f);
	}

	m_rotationSpeed = MathHelper::Lerp(0.01f, 0.1f, m_moveSpeed * 10);
}

void ButiEngine::Flocking::CalcGatherVec()
{
	//プレイヤーのいる方向を向く
	m_gatherDir = Vector3Const::Zero;

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_gatherDir = (playerPos - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}

void ButiEngine::Flocking::CalcCohesionVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	//群れの中心へ向かう

	m_cohesionVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	m_cohesionVec = (m_averagePos - pos).GetNormalize();
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
		float viewRadiusSqr = m_viewRadius * m_viewRadius;
		float distanceSqr = (pos - workerPos).GetLengthSqr();
		if (distanceSqr <= m_viewRadius)
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
		float nearBorderSqr = m_nearBorder * m_nearBorder;
		float distanceSqr = (pos - workerPos).GetLengthSqr();
		if (distanceSqr <= nearBorderSqr)
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

void ButiEngine::Flocking::CalcAvoidPlayerVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	m_avoidPlayerVec = Vector3Const::Zero;

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float nearBorderSqr = m_nearBorder * m_nearBorder;
	float distanceSqr = (pos - playerPos).GetLengthSqr();
	if (distanceSqr <= nearBorderSqr)
	{
		Vector3 diff = pos - playerPos;
		m_avoidPlayerVec += diff / diff.GetLengthSqr();
	}
}

void ButiEngine::Flocking::CalcSurroundVec(std::vector<Value_ptr<GameObject>> arg_vec_workers)
{
	m_surroundVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_cohesionVec = (playerPos - m_averagePos).GetNormalize();
}

void ButiEngine::Flocking::Move()
{
	Vector3 dir = (m_gatherDir * m_gatherWeight);
	dir += (m_cohesionVec * m_cohesionWeight);
	dir += (m_alignmentVec * m_alignmentWeight);
	dir += (m_separationVec * m_separationWeight);
	dir += (m_avoidPlayerVec * m_avoidPlayerWeight);
	dir += (m_surroundVec * m_surroundWeight);
	dir.y = 0.0f;
	dir.Normalize();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	auto rotationTarget = gameObject.lock()->transform->GetMatrix();
	rotationTarget.SetLookAt(pos + dir);
	auto rotation = MathHelper::LearpQuat(gameObject.lock()->transform->GetLocalRotation().ToQuat(), rotationTarget.ToQuat(), m_rotationSpeed);
	gameObject.lock()->transform->SetLocalRotation(rotation.ToMatrix());

	gameObject.lock()->transform->Translate(Vector3Const::ZAxis * rotation.ToMatrix() * m_moveSpeed);
}
