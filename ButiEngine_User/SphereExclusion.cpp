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
			GUI::Console("Hit");
			//auto exclusion = arg_vwp_other.lock()->GetGameComponent<SphereExclusion>();
			//if (exclusion)
			//{
			//	//‘ŠŽè‚æ‚èŒy‚©‚Á‚½‚ç‰Ÿ‚µ–ß‚³‚ê‚é
			//	float otherMass = exclusion->GetMass();
			//	if (otherMass <= m_mass)
			//	{
			//		Exclusion(arg_vwp_other, exclusion);
			//	}
			//}
		});

	gameObject.lock()->AddCollisionStayReaction(collisionLambda);
}

void ButiEngine::SphereExclusion::Start()
{
}

void ButiEngine::SphereExclusion::OnShowUI()
{
	GUI::DragFloat("##mass", &m_mass, 0.1f, 0.0f, 10000.0f);
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

	Vector3 newPos = otherPos + dir * (radius + otherRadius);

	transform->SetWorldPosition(newPos);
}
