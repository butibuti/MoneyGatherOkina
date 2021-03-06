#include "stdafx_u.h"
#include "SphereExclusion.h"
#include "Player.h"

void ButiEngine::SphereExclusion::OnUpdate()
{
}

void ButiEngine::SphereExclusion::OnSet()
{
	auto collisionLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other->GetIsRemove()) { return; }
			auto exclusion = arg_vwp_other->GetGameComponent<SphereExclusion>();
			if (exclusion)
			{
				//相手より軽かったら押し戻される
				float otherWeight = exclusion->GetWeight();
				if (m_weight < otherWeight)
				{
					Exclusion(arg_vwp_other, exclusion);
				}
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionLambda);
	gameObject.lock()->AddCollisionStayReaction(collisionLambda);
}

void ButiEngine::SphereExclusion::Start()
{
}

void ButiEngine::SphereExclusion::OnShowUI()
{
	GUI::DragFloat("##weight", &m_weight, 0.1f, 0.0f, 10000.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SphereExclusion::Clone()
{
	return ObjectFactory::Create<SphereExclusion>();
}

void ButiEngine::SphereExclusion::Exclusion(Value_weak_ptr<GameObject> arg_vwp_other, Value_ptr<SphereExclusion> arg_vlp_exclusion)
{
	auto transform = gameObject.lock()->transform;
	auto otherTransform = arg_vwp_other.lock()->transform;

	Vector3 pos = transform->GetWorldPosition();
	Vector3 otherPos = otherTransform->GetWorldPosition();

	Vector3 dir = pos - otherPos;
	dir.y = 0.0f;
	dir.Normalize();

	float radius = transform->GetWorldScale().x * 0.5f;
	float otherRadius = otherTransform->GetWorldScale().x * 0.5f;

	Vector3 newPos = otherPos + dir * (radius + otherRadius + 0.001f);
	newPos.y = pos.y;

	transform->SetWorldPosition(newPos);

	auto vlp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	if (vlp_playerComponent)
	{
		Vector3 velocity = vlp_playerComponent->GetVelocity();
		velocity = velocity - velocity.Dot(dir) * dir;
		vlp_playerComponent->SetVelocity(velocity);
	}
}
