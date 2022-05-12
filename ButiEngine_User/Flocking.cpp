#include "stdafx_u.h"
#include "Flocking.h"
#include "Player.h"
#include "SeparateDrawObject.h"

std::vector<ButiEngine::Value_ptr<ButiEngine::GameObject>> ButiEngine::Flocking::m_vec_workers;
float ButiEngine::Flocking::m_gatherWeight = 0.5f;
float ButiEngine::Flocking::m_cohesionWeight = 0.5f;
float ButiEngine::Flocking::m_alignmentWeight = 0.5f;
float ButiEngine::Flocking::m_separationWeight = 1.0f;
float ButiEngine::Flocking::m_avoidPlayerWeight = 1.0f;
float ButiEngine::Flocking::m_surroundWeight = 1.0f;
float ButiEngine::Flocking::m_viewRadius = 10.0f;
float ButiEngine::Flocking::m_nearBorder = 1.2f;

void ButiEngine::Flocking::OnUpdate()
{
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
	auto tag = GameObjectTag("Flocking");
	gameObject.lock()->SetGameObjectTag(tag);

	m_vec_workers.push_back(gameObject);
}

void ButiEngine::Flocking::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Flocking"));

	auto find = std::find(m_vec_workers.begin(), m_vec_workers.end(), gameObject.lock());
	if (find != m_vec_workers.end())
	{
		m_vec_workers.erase(find);
	}
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
	m_vlp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();

	m_rotationSpeed = 0.2f;
	m_maxRotationSpeed = 0.2f;
	m_moveSpeed = m_vlp_playerComponent->GetMaxMoveSpeed() * 1.1f;
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetSpeed(m_rotationSpeed);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Flocking::Clone()
{
	return ObjectFactory::Create<Flocking>();
}

void ButiEngine::Flocking::CalculateAveragePos()
{
	m_averagePos = Vector3Const::Zero;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	std::uint8_t workerNum = 0;

	auto end = m_vec_workers.end();
	for (auto itr = m_vec_workers.begin(); itr != end; ++itr)
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

void ButiEngine::Flocking::CalculateMoveSpeed()
{
	//プレイヤーに近い時プレイヤーの速度に合わせる
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();

	float nearBorderSqr = m_nearBorder * m_nearBorder;
	float distanceSqr = (playerPos - m_averagePos).GetLengthSqr();

	if (distanceSqr <= nearBorderSqr)
	{
		m_moveSpeed = MathHelper::Lerp(m_moveSpeed, m_vlp_playerComponent->GetMoveSpeed(), 0.1f);
	}
	else
	{
		m_moveSpeed = MathHelper::Lerp(m_moveSpeed, m_vlp_playerComponent->GetMaxMoveSpeed() * 1.1f, 0.3f);
	}

	m_rotationSpeed = MathHelper::Lerp(0.0f, m_maxRotationSpeed, m_moveSpeed * 10);
	m_vlp_lookAt->SetSpeed(m_rotationSpeed);
}

void ButiEngine::Flocking::CalculateGatherVec()
{
	//プレイヤーのいる方向を向く
	m_gatherVec = Vector3Const::Zero;

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_gatherVec = (playerPos - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}

void ButiEngine::Flocking::CalculateCohesionVec()
{
	//群れの中心へ向かう

	m_cohesionVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	m_cohesionVec = (m_averagePos - pos).GetNormalize();
}

void ButiEngine::Flocking::CalculateALignmentVec()
{
	//周りと同じ方向を向く

	m_alignmentVec = Vector3Const::Zero;
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
}

void ButiEngine::Flocking::CalculateSeparationVec()
{
	//近づきすぎたWorkerと離れる方向を向く

	m_separationVec = Vector3Const::Zero;
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
			Vector3 diff = pos - workerPos;
			m_separationVec += diff / diff.GetLengthSqr();
		}
	}

	m_separationVec.Normalize();
}

void ButiEngine::Flocking::CalculateAvoidPlayerVec()
{
	//近づきすぎたPlayerと離れる方向を向く

	m_avoidPlayerVec = Vector3Const::Zero;

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float nearBorderSqr = m_nearBorder * m_nearBorder;
	float distanceSqr = (pos - playerPos).GetLengthSqr();
	if (distanceSqr <= nearBorderSqr)
	{
		Vector3 diff = pos - playerPos;
		m_avoidPlayerVec = diff / diff.GetLengthSqr();
	}
}

void ButiEngine::Flocking::CalculateSurroundVec()
{
	//プレイヤーを囲む方向を向く

	m_surroundVec = Vector3Const::Zero;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	m_cohesionVec = (playerPos - m_averagePos).GetNormalize();
}

void ButiEngine::Flocking::Move()
{
	Vector3 velocity = (m_gatherVec * m_gatherWeight);
	velocity += (m_cohesionVec * m_cohesionWeight);
	velocity += (m_alignmentVec * m_alignmentWeight);
	velocity += (m_separationVec * m_separationWeight);
	velocity += (m_avoidPlayerVec * m_avoidPlayerWeight);
	velocity += (m_surroundVec * m_surroundWeight);
	velocity.y = 0.0f;
	velocity.Normalize();


	gameObject.lock()->transform->Translate(velocity * m_moveSpeed);

	auto lookTarget = gameObject.lock()->transform->Clone();
	lookTarget->Translate(velocity);
	m_vlp_lookAt->SetLookTarget(lookTarget);


	//Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	//auto rotationTarget = gameObject.lock()->transform->GetMatrix();
	//rotationTarget.SetLookAt(pos + velocity);
	//auto rotation = MathHelper::LearpQuat(gameObject.lock()->transform->GetLocalRotation().ToQuat(), rotationTarget.ToQuat(), 1.0f);
	//gameObject.lock()->transform->SetLocalRotation(rotation.ToMatrix());

	//gameObject.lock()->transform->Translate(Vector3Const::ZAxis * rotation.ToMatrix() * m_moveSpeed);
}
