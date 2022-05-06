#include "stdafx_u.h"
#include "Player.h"
#include"InputManager.h"
#include "ParticleGenerater.h"
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
	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &m_maxMoveSpeed, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("VibrationForce");
	GUI::DragFloat("##vForce", &m_vibrationForce, 1.0f, 0.0f, 100.0f);
}

void ButiEngine::Player::Start()
{
	m_vlp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_knockBackVelocity = Vector3(0, 0, 0);
	m_life = 3;
	m_level = 1;
	m_addTrajectoryParticleCounter = 0;
	m_addTrajectoryParticleWait = 2;
	m_knockBackFlame = 0;
	m_maxKnockBackFlame = 20;
	m_moveSpeed = 0.0f;
	m_maxMoveSpeed = 0.1f;
	m_vibrationForce = 3.0f;
	m_knockBackFlag = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::KnockBack()
{
	if (m_knockBackFlag) return;

	//ノックバックの初期値セット
	m_knockBackFlag = true;
	m_knockBackFlame = m_maxKnockBackFlame;
	m_knockBackVelocity.x = ButiRandom::GetRandom(-10, 10, 100);
	m_knockBackVelocity.z = ButiRandom::GetRandom(-10, 10, 100);
	m_knockBackVelocity.Normalize();

	m_knockBackVelocity *= 70.0f;
}

void ButiEngine::Player::Move()
{
	//ノックバック中は操作不能
	if (m_knockBackFlag) return;

	//X、Z平面の移動方向を取得
	Vector2 leftStick = InputManager::GetLeftStick();

	m_moveSpeed = 0.0f;
	if (leftStick != 0)
	{
		m_moveSpeed = m_maxMoveSpeed;

		if (m_addTrajectoryParticleCounter == 0)
		{
			auto position = gameObject.lock()->transform->GetWorldPosition();
			m_vwp_particleGenerater.lock()->TrajectoryParticles(position);
		}
	}

	auto cameraTransform = GetManager().lock()->GetScene().lock()->GetCamera("main")->vlp_transform;

	Vector3 velocity = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
	velocity.y = 0;
	velocity.Normalize();
	velocity *= m_moveSpeed * 70;

	m_vlp_rigidBody->GetRigidBody()->SetVelocity(velocity);
	//gameObject.lock()->transform->Translate(velocity);
}

void ButiEngine::Player::MoveKnockBack()
{
	if (!m_knockBackFlag) return;

	//ノックバック中の処理
	m_vlp_rigidBody->GetRigidBody()->SetVelocity(m_knockBackVelocity);

	m_knockBackVelocity *= 0.85f;

	m_knockBackFlame--;

	if (m_knockBackFlame <= 0)
	{
		m_knockBackFlag = false;
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
