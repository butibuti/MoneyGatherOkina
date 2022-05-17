#include "stdafx_u.h"
#include "AttackFlashSpawner.h"

void ButiEngine::AttackFlashSpawner::OnUpdate()
{
	if (m_vlp_spawnIntervalTimer->Update())
	{
		SpawnAttackFlash();
	}
}

void ButiEngine::AttackFlashSpawner::OnSet()
{
}

void ButiEngine::AttackFlashSpawner::OnShowUI()
{
	GUI::BulletText("Interval");
	if (GUI::DragInt("##interval", &m_spawnIntervalFrame, 1.0f, 1, 10))
	{
		m_vlp_spawnIntervalTimer->ChangeCountFrame(m_spawnIntervalFrame);
		m_vlp_spawnIntervalTimer->Reset();
	}
	GUI::BulletText("SpwanRadius");
	GUI::DragFloat("##radius", &m_spawnRadius, 0.1f, 0.1f, 30.0f);
}

void ButiEngine::AttackFlashSpawner::Start()
{
	m_spawnIntervalFrame = 6;
	m_vlp_spawnIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_spawnIntervalFrame);

	m_spawnRadius = 1.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::AttackFlashSpawner::Clone()
{
	return ObjectFactory::Create<AttackFlashSpawner>();
}

void ButiEngine::AttackFlashSpawner::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::AttackFlashSpawner::SpawnStart(const Vector3& arg_pos)
{
	if (!m_vlp_spawnIntervalTimer) { return; }
	gameObject.lock()->transform->SetLocalPosition(arg_pos);
	m_vlp_spawnIntervalTimer->Start();
}

void ButiEngine::AttackFlashSpawner::SpawnStop()
{
	if (!m_vlp_spawnIntervalTimer) { return; }
	m_vlp_spawnIntervalTimer->Stop();
	m_vlp_spawnIntervalTimer->Reset();
}

void ButiEngine::AttackFlashSpawner::SpawnAttackFlash()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 dir;
	dir.x = ButiRandom::GetInt(-100, 100);
	dir.y = ButiRandom::GetInt(-100, 100);
	dir.z = ButiRandom::GetInt(-100, 100);
	dir.Normalize();
	pos += dir * m_spawnRadius;

	Vector3 rotation;
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = ButiRandom::GetRandom(0.0f, 360.0f);

	Vector3 scale = m_effectScale * ButiRandom::GetRandom(0.3f, 1.0f);

	auto transform = ObjectFactory::Create<Transform>(pos, rotation, scale);
	transform->SetBaseTransform(gameObject.lock()->transform);

	GetManager().lock()->AddObjectFromCereal("AttackFlashEffect", transform);
}
