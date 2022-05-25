#include "stdafx_u.h"
#include "Flocking.h"
#include "Player.h"
#include "FlockingLeader.h"
#include "InputManager.h"

std::vector<ButiEngine::Value_ptr<ButiEngine::GameObject>> ButiEngine::Flocking::m_vec_workers;
float ButiEngine::Flocking::m_gatherWeight = 0.0f;
float ButiEngine::Flocking::m_cohesionWeight = 0.1f;
float ButiEngine::Flocking::m_minCohesionWeight = 0.1f;
float ButiEngine::Flocking::m_maxCohesionWeight = 0.1f;
float ButiEngine::Flocking::m_alignmentWeight = 1.0f;
float ButiEngine::Flocking::m_separationWeight = 1.0f;
float ButiEngine::Flocking::m_avoidPlayerWeight = 0.3f;
float ButiEngine::Flocking::m_surroundWeight = 0.5f;
float ButiEngine::Flocking::m_viewRadius = 2.0f;
float ButiEngine::Flocking::m_minViewRadius = 0.1f;
float ButiEngine::Flocking::m_maxViewRadius = 0.1f;
float ButiEngine::Flocking::m_nearBorder = 1.2f;
float ButiEngine::Flocking::m_playerNearBorder = 1.7f;
float ButiEngine::Flocking::m_leaderNearBorder = 1.7f;

void ButiEngine::Flocking::OnUpdate()
{
	SetLookSpeed();
	//SetCohesionWeight();
	//SetViewRadius();
	CalculateAveragePos();
	CalculateMoveSpeed();
	CalculateGatherVec();
	CalculateCohesionVec();
	CalculateALignmentVec();
	CalculateSeparationVec();
	CalculateAvoidPlayerVec();
	CalculateSurroundVec();

	Move();
}

void ButiEngine::Flocking::OnSet()
{
	AddFlocking();
}

void ButiEngine::Flocking::OnRemove()
{
	RemoveFlocking();
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
	GUI::BulletText("PlayerNearBorder");
	GUI::DragFloat("##playerNearBorder", &m_playerNearBorder, 0.01f, 0.0f, 100.0f);
	GUI::BulletText("LeaderNearBorder");
	GUI::DragFloat("##leaderNearBorder", &m_leaderNearBorder, 0.01f, 0.0f, 100.0f);
}

void ButiEngine::Flocking::Start()
{
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	m_vwp_leader = GetManager().lock()->GetGameObject(GameObjectTag("Leader"));
	m_vlp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
	m_vlp_flockingLeader = m_vwp_leader.lock()->GetGameComponent<FlockingLeader>();

	m_maxRotationSpeed = 0.3f;
	m_rotationSpeed = m_maxRotationSpeed;
	m_playerMaxMoveSpeed = m_vlp_playerComponent->GetMaxMoveSpeed();
	m_moveSpeed = m_playerMaxMoveSpeed;
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();

	m_velocity = Vector3Const::Zero;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Flocking::Clone()
{
	return ObjectFactory::Create<Flocking>();
}

void ButiEngine::Flocking::AddFlocking()
{
	auto tag = GameObjectTag("Flocking");
	gameObject.lock()->SetGameObjectTag(tag);

	m_vec_workers.push_back(gameObject);

	SetIsActive(true);
}

void ButiEngine::Flocking::RemoveFlocking()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Flocking"));

	auto find = std::find(m_vec_workers.begin(), m_vec_workers.end(), gameObject.lock());
	if (find != m_vec_workers.end())
	{
		m_vec_workers.erase(find);
	}

	SetIsActive(false);
}

void ButiEngine::Flocking::SetLookSpeed()
{
	if (m_vlp_flockingLeader->IsGather())
	{
		m_vlp_lookAt->SetSpeed(0.5f);
	}
	else
	{
		m_vlp_lookAt->SetSpeed(0.1f);
	}
}

void ButiEngine::Flocking::SetCohesionWeight()
{
	if (InputManager::IsPushGatherKey())
	{
		//m_gatherWeight = m_maxCohesionWeight;
		m_cohesionWeight = m_maxCohesionWeight;
	}
	else
	{
		//m_gatherWeight = m_minCohesionWeight;
		m_cohesionWeight = m_minCohesionWeight;
	}
}

void ButiEngine::Flocking::SetViewRadius()
{
	if (m_vec_workers.size() < 25)
	{
		m_viewRadius = m_minViewRadius;
	}
	else
	{
		m_viewRadius = m_maxViewRadius;
	}
}

void ButiEngine::Flocking::CalculateAveragePos()
{
	//見えている範囲のWorkerとリーダーの平均の位置を計算する

	m_averagePos = Vector3Const::Zero;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = m_vec_workers.end();
	for (auto itr = m_vec_workers.begin(); itr != end; ++itr)
	{
		//見えているWorkerを追加
		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float viewRadiusSqr = m_viewRadius * m_viewRadius;
		float distanceSqr = (pos - workerPos).GetLengthSqr();
		if (distanceSqr <= viewRadiusSqr)
		{
			workerNum++;
			m_averagePos += workerPos;
		}
	}

	//リーダーは見えていなくても追加
	m_averagePos += m_vwp_leader.lock()->transform->GetWorldPosition();
	m_averagePos /= workerNum + 1;
}

void ButiEngine::Flocking::CalculateMoveSpeed()
{
	float leaderSpeed = m_vlp_flockingLeader->GetMoveSpeed();
	m_moveSpeed = MathHelper::Lerp(m_moveSpeed, leaderSpeed * 1.1f, 0.05f * GameDevice::WorldSpeed);
	m_moveSpeed = max(m_moveSpeed, m_playerMaxMoveSpeed);
}

void ButiEngine::Flocking::CalculateGatherVec()
{
	//リーダーへ向かう

	m_gatherVec = Vector3Const::Zero;
	if (m_gatherWeight == 0.0f) { return; }

	Vector3 leaderPos = m_vwp_leader.lock()->transform->GetWorldPosition();
	m_gatherVec = (leaderPos - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
	m_gatherVec *= m_gatherWeight;
}

void ButiEngine::Flocking::CalculateCohesionVec()
{
	//群れの中心へ向かう

	m_cohesionVec = Vector3Const::Zero;
	if (m_cohesionWeight == 0.0f) { return; }

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	m_cohesionVec = (m_averagePos - pos).GetNormalize();
	m_cohesionVec *= m_cohesionWeight;
}

void ButiEngine::Flocking::CalculateALignmentVec()
{
	//周りと同じ方向へ向かう

	m_alignmentVec = Vector3Const::Zero;
	if (m_alignmentWeight == 0.0f) { return; }

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = m_vec_workers.end();
	for (auto itr = m_vec_workers.begin(); itr != end; ++itr)
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

	m_alignmentVec.Normalize();
	m_alignmentVec *= m_alignmentWeight;
}

void ButiEngine::Flocking::CalculateSeparationVec()
{
	//近づきすぎたWorkerと離れる方向へ向かう

	m_separationVec = Vector3Const::Zero;
	if (m_separationWeight == 0.0f) { return; }

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	std::uint8_t nearWorkerNum = 0;

	auto end = m_vec_workers.end();
	for (auto itr = m_vec_workers.begin(); itr != end; ++itr)
	{
		if (gameObject.lock() == (*itr)) { continue; }

		Vector3 workerPos = (*itr)->transform->GetLocalPosition();
		float nearBorderSqr = m_nearBorder * m_nearBorder;
		float distanceSqr = (pos - workerPos).GetLengthSqr();
		if (distanceSqr <= nearBorderSqr)
		{
			nearWorkerNum++;
			if (distanceSqr == 0.0f)
			{
				m_separationVec += Vector3(10.0f, 0.0f, 10.0f);
			}
			else
			{
				Vector3 diff = pos - workerPos;
				m_separationVec += diff / distanceSqr;
			}
		}
	}

	m_separationVec.Normalize();
	m_separationVec *= m_separationWeight;
}

void ButiEngine::Flocking::CalculateAvoidPlayerVec()
{
	//近づきすぎたプレイヤーと離れる方向へ向かう

	m_avoidPlayerVec = Vector3Const::Zero;
	if (m_avoidPlayerWeight == 0.0f) { return; }

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float nearBorderSqr = m_playerNearBorder * m_playerNearBorder;
	float distanceSqr = (pos - playerPos).GetLengthSqr();
	if (distanceSqr <= nearBorderSqr)
	{
		if (distanceSqr == 0.0f)
		{
			m_avoidPlayerVec += Vector3(10.0f, 0.0f, 10.0f);
		}
		else
		{
			Vector3 diff = pos - playerPos;
			m_avoidPlayerVec = diff / distanceSqr;
		}
		m_avoidPlayerVec *= m_avoidPlayerWeight;
	}
}

void ButiEngine::Flocking::CalculateSurroundVec()
{
	//リーダーを囲む方向へ向かう

	m_surroundVec = Vector3Const::Zero;
	if (m_surroundWeight == 0.0f) { return; }

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	Vector3 leaderPos = m_vwp_leader.lock()->transform->GetWorldPosition();
	m_surroundVec = (leaderPos - m_averagePos).GetNormalize();
	m_surroundVec *= m_surroundWeight;
}

void ButiEngine::Flocking::Move()
{
	m_velocity = Vector3Const::Zero;
	m_velocity += m_gatherVec;
	m_velocity += m_cohesionVec;
	m_velocity += m_alignmentVec;
	m_velocity += m_separationVec;
	m_velocity += m_avoidPlayerVec;
	m_velocity += m_surroundVec;
	m_velocity.y = 0.0f;
	m_velocity.Normalize();
	
	//m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + m_velocity * 100.0f);
	//gameObject.lock()->transform->Translate(gameObject.lock()->transform->GetFront() * m_moveSpeed * GameDevice::WorldSpeed);

	if (!GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_LEFT))
	{
		m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + m_velocity * 100.0f);
		gameObject.lock()->transform->Translate(gameObject.lock()->transform->GetFront() * m_moveSpeed * GameDevice::WorldSpeed);
	}
	else
	{
		gameObject.lock()->transform->Translate(m_velocity * m_moveSpeed * GameDevice::WorldSpeed);
		m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + m_velocity);
	}
}

bool ButiEngine::Flocking::IsNearLeader(const Vector3& arg_pos)
{
	Vector3 leaderPos = m_vwp_leader.lock()->transform->GetWorldPosition();

	float nearBorderSqr = m_leaderNearBorder * m_leaderNearBorder;
	float distanceSqr = (leaderPos - arg_pos).GetLengthSqr();

	return distanceSqr <= nearBorderSqr;
}
