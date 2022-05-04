#include "stdafx_u.h"
#include "Player.h"
#include"InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Player::OnUpdate()
{
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
	m_life = 3;
	m_level = 1;
	m_moveSpeed = 0.0f;
	m_maxMoveSpeed = 0.1f;
	m_vibrationForce = 3.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Move()
{
	//X�AZ���ʂ̈ړ��������擾
	Vector2 leftStick = InputManager::GetLeftStick();

	m_moveSpeed = 0.0f;
	if (leftStick != 0)
	{
		m_moveSpeed = m_maxMoveSpeed;
	}

	auto cameraTransform = GetManager().lock()->GetScene().lock()->GetCamera("main")->vlp_transform;

	Vector3 velocity = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
	velocity.y = 0;
	velocity.Normalize();
	velocity *= m_moveSpeed * 70;

	m_vlp_rigidBody->GetRigidBody()->SetVelocity(velocity);
	//gameObject.lock()->transform->Translate(velocity);
}
