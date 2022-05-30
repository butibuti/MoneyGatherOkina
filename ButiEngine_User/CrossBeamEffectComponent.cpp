#include "stdafx_u.h"
#include "CrossBeamEffectComponent.h"

void ButiEngine::CrossBeamEffectComponent::OnUpdate()
{
	if (m_vlp_animationTimer->Update())
	{
		if (m_animationStep == 0)
		{
			//フラッシュを生成
			m_vwp_crossFlashEffectUI[0] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
			m_vwp_crossFlashEffectUI[1] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
			m_vwp_crossFlashEffectUI[0].lock()->transform->SetLocalPosition(m_position);
			m_vwp_crossFlashEffectUI[1].lock()->transform->SetLocalPosition(m_position);
			m_vwp_crossFlashEffectUI[1].lock()->transform->SetLocalRotationZ_Degrees(90);

			m_animationRotate = 16;

			m_vlp_animationTimer->ChangeCountFrame(8);
		}
		else if (m_animationStep == 1)
		{
			m_vlp_animationTimer->ChangeCountFrame(25);
		}

		if (m_animationStep < 5)
		{
			m_animationStep++;
		}
	}

	//スケールを変える（小→大→小）
	if (m_animationStep == 1)
	{
		m_animationScale = MathHelper::Lerp(m_animationScale, 200.0f, 0.6f);
		m_vwp_crossFlashEffectUI[0].lock()->transform->RollLocalRotationZ_Degrees(-1);
		m_vwp_crossFlashEffectUI[1].lock()->transform->RollLocalRotationZ_Degrees(-1);
	}
	else if (m_animationStep == 2)
	{
		m_animationScale = MathHelper::Lerp(m_animationScale, 0.0f, 0.2f);
		m_animationRotate = MathHelper::Lerp(m_animationRotate, 1.0f, 0.08f);
		m_vwp_crossFlashEffectUI[0].lock()->transform->RollLocalRotationZ_Degrees(-m_animationRotate);
		m_vwp_crossFlashEffectUI[1].lock()->transform->RollLocalRotationZ_Degrees(-m_animationRotate);
	}
	else
	{
		m_animationScale = 0;
	}

	//スケールと回転の更新
	if (m_vwp_crossFlashEffectUI[0].lock())
	{
		auto scale = m_vwp_crossFlashEffectUI[0].lock()->transform->GetLocalScale();
		m_vwp_crossFlashEffectUI[0].lock()->transform->SetLocalScale(Vector3(scale.x, m_animationScale, scale.z));
		m_vwp_crossFlashEffectUI[1].lock()->transform->SetLocalScale(Vector3(scale.x, m_animationScale, scale.z));
	}
}

void ButiEngine::CrossBeamEffectComponent::OnSet()
{
	m_vlp_animationTimer = ObjectFactory::Create<AbsoluteTimer>(10);
	m_position = Vector3(0, 0, 0);
}

void ButiEngine::CrossBeamEffectComponent::OnRemove()
{
}

void ButiEngine::CrossBeamEffectComponent::OnShowUI()
{
}

void ButiEngine::CrossBeamEffectComponent::Start()
{
	m_vlp_animationTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CrossBeamEffectComponent::Clone()
{
	return ObjectFactory::Create<CrossBeamEffectComponent>();
}
