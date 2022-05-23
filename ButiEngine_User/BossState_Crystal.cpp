#include "stdafx_u.h"
#include "BossState_Crystal.h"
#include "Enemy_Boss.h"
#include "PredictedPoint.h"
#include "SpriteParticleGenerator.h"

void ButiEngine::BossState_Crystal::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::CrystalPhase::Charge:
		Charge();
		break;
	case ButiEngine::CrystalPhase::Appear:
		Appear();
		break;
	default:
		break;
	}
}

void ButiEngine::BossState_Crystal::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_vwp_spriteParticleGenerator = GetManager().lock()->GetGameObject("SphereParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();

	m_isStrengthened = gameObject.lock()->GetGameComponent<Enemy_Boss>()->IsStrengthened();

	SetPhaseParameter();
}

void ButiEngine::BossState_Crystal::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Attack"));
}

void ButiEngine::BossState_Crystal::OnShowUI()
{
}

void ButiEngine::BossState_Crystal::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossState_Crystal::Clone()
{
	return ObjectFactory::Create<BossState_Crystal>();
}

void ButiEngine::BossState_Crystal::EndState()
{
	SetIsRemove(true);
}

void ButiEngine::BossState_Crystal::Charge()
{
	m_vwp_spriteParticleGenerator.lock()->ChargeParticles(gameObject.lock()->transform->GetLocalPosition(), 15.0f, ButiColor::White());
	if (m_vlp_chargeTimer->Update())
	{
		m_vlp_chargeTimer->Stop();
		AppearStart();
	}
}

void ButiEngine::BossState_Crystal::ChargeStart()
{
	m_phase = CrystalPhase::Charge;
	m_vlp_chargeTimer->Start();
}

void ButiEngine::BossState_Crystal::Appear()
{
	CreateObjectAround("Crystal");
	EndState();
}

void ButiEngine::BossState_Crystal::AppearStart()
{
	m_phase = CrystalPhase::Appear;
	CreateObjectAround("PredictedPoint");
}

void ButiEngine::BossState_Crystal::CreateObjectAround(const std::string& arg_objectName)
{
	std::uint8_t layerCount = 1;
	if (m_isStrengthened)
	{
		layerCount = 2;
	}

	for (std::uint8_t i = 0; i < layerCount; i++)
	{

		std::uint8_t objectCount = 8 + (i * 4);

		float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;

		auto objectCenter = gameObject.lock()->transform->Clone();
		auto objectTransform = ObjectFactory::Create<Transform>();
		objectTransform->SetBaseTransform(objectCenter);
		objectTransform->SetLocalPosition(Vector3(-(radius + 4.0f + (i * 5.0f)) / gameObject.lock()->transform->GetLocalScale().x, 0.0f, 0.0f));

		float rollAngle = 360.0f / objectCount;

		for (std::uint8_t i = 0; i < objectCount; i++)
		{
			auto object = GetManager().lock()->AddObjectFromCereal(arg_objectName, ObjectFactory::Create<Transform>());

			Vector3 pos = objectTransform->GetWorldPosition();
			object.lock()->transform->SetLocalPosition(pos);

			auto predictedPoint = object.lock()->GetGameComponent<PredictedPoint>();
			if (predictedPoint)
			{
				predictedPoint->SetLife(180);
			}

			objectCenter->RollLocalRotationY_Degrees(rollAngle);
		}
	}
}

void ButiEngine::BossState_Crystal::SetPhaseParameter()
{
	m_vlp_chargeTimer = ObjectFactory::Create<RelativeTimer>(180);
	
	ChargeStart();
}
