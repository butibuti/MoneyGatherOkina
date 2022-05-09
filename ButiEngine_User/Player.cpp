#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "ParticleGenerater.h"
#include "WaveManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Player::OnUpdate()
{
	TrajectoryParticleWaitCount();
	MoveKnockBack();
	Move();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::OnShowUI()
{
	GUI::Text("Level:%d", m_level);
	GUI::Text("Exp:%d", m_exp);
	GUI::Text("MaxWorker:%d", m_maxWorkerCount);

	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &m_maxMoveSpeed, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("VibrationForce");
	GUI::DragFloat("##vForce", &m_vibrationForce, 1.0f, 0.0f, 100.0f);
}

void ButiEngine::Player::Start()
{
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_knockBackVelocity = Vector3(0, 0, 0);
	m_life = 3;

	m_level = 1;
	m_maxLevel = 10;
	m_maxWorkerCount = 10;
	m_exp = 0;

	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_addTrajectoryParticleCounter = 0;
	m_addTrajectoryParticleWait = 4;
	m_knockBackFrame = 0;
	m_maxKnockBackFrame = 15;
	m_moveSpeed = 0.0f;
	m_maxMoveSpeed = 0.1f;

	m_vibrationForce = 3.0f;
	m_isKnockBack = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::AddExp()
{
	if (m_level == m_maxLevel) { return; }

	m_exp++;
	std::uint16_t requestExp = CalcRequestExp();
	if (m_exp == requestExp)
	{
		LevelUp();
	}
}

void ButiEngine::Player::KnockBack(const Vector3& arg_velocity)
{
	if (m_isKnockBack) return;
	if (m_vwp_waveManager.lock()->IsClearAnimationFlag()) return;

	//ノックバックの初期値セット
	m_isKnockBack = true;
	m_knockBackFrame = m_maxKnockBackFrame;

	m_knockBackVelocity = arg_velocity;
	m_knockBackVelocity.y = 0;
	//m_knockBackVelocity.x = ButiRandom::GetRandom(-10, 10, 100);
	//m_knockBackVelocity.z = ButiRandom::GetRandom(-10, 10, 100);
	m_knockBackVelocity.Normalize();
}

void ButiEngine::Player::Move()
{
	//ノックバック中は操作不能
	if (m_isKnockBack) return;
	//リザルト(クリア演出)中は操作不能
	if (m_vwp_waveManager.lock()->IsClearAnimationFlag()) return;

	//X、Z平面の移動方向を取得
	Vector2 leftStick = InputManager::GetLeftStick();
	
	auto cameraTransform = GetManager().lock()->GetScene().lock()->GetCamera("main")->vlp_transform;

	Vector3 velocity;

	m_moveSpeed = 0.0f;
	if (leftStick != 0)
	{
		m_moveSpeed = m_maxMoveSpeed;

		if (m_addTrajectoryParticleCounter == 0)
		{
			velocity = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
			velocity.y = 0;
			velocity.Normalize();
			velocity *= m_moveSpeed * 10.0f;

			auto position = gameObject.lock()->transform->GetWorldPosition();
			m_vwp_particleGenerater.lock()->TrajectoryParticles(position);
			m_vwp_particleGenerater.lock()->PachiPachiParticles(position - velocity);
		}
	}


	velocity = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
	velocity.y = 0;
	velocity.Normalize();
	velocity *= m_moveSpeed;

	gameObject.lock()->transform->Translate(velocity);
}

void ButiEngine::Player::LevelUp()
{
	m_level++;
	m_maxWorkerCount += 10;
}

void ButiEngine::Player::MoveKnockBack()
{
	if (!m_isKnockBack) return;

	//ノックバック中の処理
	gameObject.lock()->transform->Translate(m_knockBackVelocity);

	m_knockBackVelocity *= 0.7f;

	m_knockBackFrame--;

	if (m_knockBackFrame <= 0)
	{
		m_isKnockBack = false;
	}
}

void ButiEngine::Player::TrajectoryParticleWaitCount()
{
	if (m_addTrajectoryParticleCounter < m_addTrajectoryParticleWait)
	{
		m_addTrajectoryParticleCounter++;
	}
	else
	{
		m_addTrajectoryParticleCounter = 0;
	}
}

std::uint16_t ButiEngine::Player::CalcRequestExp()
{
	return m_level * 10;
}
