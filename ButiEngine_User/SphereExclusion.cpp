#include "stdafx_u.h"
#include "SphereExclusion.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

void ButiEngine::SphereExclusion::OnUpdate()
{
}

void ButiEngine::SphereExclusion::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			auto exclusion = arg_vwp_other.lock()->GetGameComponent<SphereExclusion>();
			if (exclusion)
			{
				//‘ŠŽè‚æ‚èŒy‚©‚Á‚½‚ç‰Ÿ‚µ–ß‚³‚ê‚é
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

	Vector3 dir = (pos - otherPos).GetNormalize();

	float radius = transform->GetWorldScale().x * 0.5f;
	float otherRadius = otherTransform->GetWorldScale().x * 0.5f;

	Vector3 newPos = otherPos + dir * (radius + otherRadius + 0.001f);

	transform->SetWorldPosition(newPos);
}
