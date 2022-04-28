#include "stdafx_u.h"
#include "Player.h"
#include"InputManager.h"

void ButiEngine::Player::OnUpdate()
{
	Move();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	m_life = 3;
	m_level = 1;
	m_moveSpeed = 1.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Move()
{
	//XAZ•½–Ê‚ÌˆÚ“®•ûŒü‚ðŽæ“¾
	Vector2 velocityXZ = InputManager::GetLeftStick();
	velocityXZ.Normalize();

	gameObject.lock()->transform->Translate(Vector3(velocityXZ.x, 0.0f, velocityXZ.y));
}
