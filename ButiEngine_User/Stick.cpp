#include "stdafx_u.h"
#include "Stick.h"
#include "Pocket.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Stick::OnUpdate()
{
	if (m_vwp_pocket.lock()) { return; }
}

void ButiEngine::Stick::OnSet()
{
}

void ButiEngine::Stick::OnShowUI()
{
}

void ButiEngine::Stick::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Stick::Clone()
{
	return ObjectFactory::Create<Stick>();
}

void ButiEngine::Stick::SetPocket(Value_weak_ptr<GameObject> arg_vwp_pocket)
{
	m_vwp_pocket = arg_vwp_pocket;
	auto pocketComponent = m_vwp_pocket.lock()->GetGameComponent<Pocket>();

	pocketComponent->SetWorker(gameObject);

	gameObject.lock()->transform->SetLocalPosition(m_vwp_pocket.lock()->transform->GetWorldPosition());
	
	auto rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	rigidBody->GetRigidBody()->SetVelocity(Vector3Const::Zero);
	rigidBody->TransformApply();
}
