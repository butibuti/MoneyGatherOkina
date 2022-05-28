#include "stdafx_u.h"
#include "BossState_Crystal.h"
#include "Enemy_Boss.h"
#include "PredictedPoint.h"
#include "SpriteParticleGenerator.h"

void ButiEngine::BossState_Crystal::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::CrystalStatePhase::Charge:
		Charge();
		break;
	case ButiEngine::CrystalStatePhase::Appear:
		Appear();
	case ButiEngine::CrystalStatePhase::Wait:
		Wait();
		break;
	default:
		break;
	}
}

void ButiEngine::BossState_Crystal::OnSet()
{
	auto tag = GameObjectTag("Attack");
	gameObject.lock()->SetGameObjectTag(tag);

	m_vwp_screenMeshDraw = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<MeshDrawComponent>(5);
	m_vwp_spriteParticleGenerator = GetManager().lock()->GetGameObject("SphereParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();

	m_defaultBloomPower = m_vwp_screenMeshDraw.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color;
	m_maxBloomPower = m_defaultBloomPower + Vector4(3.0f, 3.0f, 3.0f, 0.0f);

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
	float progress = m_vlp_chargeTimer->GetPercent();
	progress = min(progress, 1.0f);

	Vector4 bloomPower = MathHelper::LerpPosition(m_defaultBloomPower, m_maxBloomPower, Easing::EaseOutCirc(progress));
	bloomPower.w = 1.0f;
	m_vwp_screenMeshDraw.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = bloomPower;

	m_vwp_spriteParticleGenerator.lock()->GatherParticles(gameObject.lock()->transform, 15.0f, ButiColor::White());
	if (m_vlp_chargeTimer->Update())
	{
		m_vlp_chargeTimer->Stop();
		m_vwp_screenMeshDraw.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = m_maxBloomPower;
		AppearStart();
	}
}

void ButiEngine::BossState_Crystal::ChargeStart()
{
	m_phase = CrystalStatePhase::Charge;
	m_vlp_chargeTimer->Start();
}

void ButiEngine::BossState_Crystal::Appear()
{
	if (m_vlp_appearTimer->Update())
	{
		CreateObjectAround("Crystal");
		WaitStart();
	}
}

void ButiEngine::BossState_Crystal::AppearStart()
{
	m_phase = CrystalStatePhase::Appear;
	CreateObjectAround("PredictedPoint");
	m_vlp_appearTimer->Start();
}

void ButiEngine::BossState_Crystal::Wait()
{
	float progress = m_vlp_waitTimer->GetPercent();
	progress = min(progress, 1.0f);

	Vector4 bloomPower = MathHelper::LerpPosition(m_maxBloomPower, m_defaultBloomPower, Easing::EaseOutCirc(progress));
	bloomPower.w = 1.0f;
	m_vwp_screenMeshDraw.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = bloomPower;

	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		m_vwp_screenMeshDraw.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = m_defaultBloomPower;
		EndState();
	}
}

void ButiEngine::BossState_Crystal::WaitStart()
{
	m_phase = CrystalStatePhase::Wait;
	m_vlp_waitTimer->Start();
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
		objectTransform->SetLocalPosition(Vector3(-(radius + 5.0f + (i * 7.0f)) / gameObject.lock()->transform->GetLocalScale().x, 0.0f, 0.0f));

		float rollAngle = 360.0f / objectCount;

		for (std::uint8_t i = 0; i < objectCount; i++)
		{
			auto object = GetManager().lock()->AddObjectFromCereal(arg_objectName, ObjectFactory::Create<Transform>());

			Vector3 pos = objectTransform->GetWorldPosition();
			object.lock()->transform->SetLocalPosition(pos);

			auto predictedPoint = object.lock()->GetGameComponent<PredictedPoint>();
			if (predictedPoint)
			{
				predictedPoint->SetLife(60);
			}

			objectCenter->RollLocalRotationY_Degrees(rollAngle);
		}
	}
}

void ButiEngine::BossState_Crystal::SetPhaseParameter()
{
	m_vlp_chargeTimer = ObjectFactory::Create<RelativeTimer>(240);
	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(240);
	
	ChargeStart();
}
