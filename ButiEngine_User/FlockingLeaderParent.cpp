#include "stdafx_u.h"
#include "FlockingLeaderParent.h"
#include "InputManager.h"

void ButiEngine::FlockingLeaderParent::OnUpdate()
{
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	gameObject.lock()->transform->SetLocalPosition(playerPos);

	Vector2 rightStick = InputManager::GetRightStick();
	rightStick.Normalize();

	Vector3 dir = Vector3(rightStick.x, 0.0f, rightStick.y);

	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(playerPos + m_vwp_player.lock()->transform->GetFront() * 100.0f);
	if (dir != Vector3Const::Zero)
	{
		//m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + dir * 100.0f);
	}
	else
	{
		//m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + gameObject.lock()->transform->GetFront() *100.0f);
	}
}

void ButiEngine::FlockingLeaderParent::OnSet()
{
}

void ButiEngine::FlockingLeaderParent::OnRemove()
{
}

void ButiEngine::FlockingLeaderParent::OnShowUI()
{
}

void ButiEngine::FlockingLeaderParent::Start()
{
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	SetLookAtParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FlockingLeaderParent::Clone()
{
	return ObjectFactory::Create<FlockingLeaderParent>();
}

void ButiEngine::FlockingLeaderParent::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(m_vwp_player.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(m_vwp_player.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(0.1f);
}
