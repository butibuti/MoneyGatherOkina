#include "stdafx_u.h"
#include "StartPopUpComponent.h"
#include "Player.h"

void ButiEngine::StartPopUpComponent::OnUpdate()
{
	CheckHitPlayer();
	ScaleUpdate();
}

void ButiEngine::StartPopUpComponent::OnSet()
{
}

void ButiEngine::StartPopUpComponent::Start()
{
	if (m_vwp_player.lock() == nullptr)
	{
		m_vwp_player = GetManager().lock()->GetGameObject("Player");
	}

	m_defaultScale = Vector3(4, 2, 2);
	m_currentScale = m_defaultScale;
	m_previousScale = m_currentScale;
	m_knockBackCount = 15;
	m_isHitPlayerFlag = false;
	m_scaleAnimationFlag = true;
}

void ButiEngine::StartPopUpComponent::OnShowUI()
{
}

void ButiEngine::StartPopUpComponent::AppearPopUp()
{
	m_currentScale = m_defaultScale;
	m_knockBackCount = 15;
	m_isHitPlayerFlag = false;
	m_scaleAnimationFlag = true;
}

void ButiEngine::StartPopUpComponent::DisappearPopUp()
{
	m_currentScale = Vector3(0, 0, 0);
	m_scaleAnimationFlag = true;
}

bool ButiEngine::StartPopUpComponent::IsHitPlayerFlag()
{
	return m_isHitPlayerFlag;
}

void ButiEngine::StartPopUpComponent::CheckHitPlayer()
{
	//プレイヤーとの当たり判定
	auto position = gameObject.lock()->transform->GetLocalPosition();
	auto playerPosition = m_vwp_player.lock()->transform->GetLocalPosition();

	float hitDistance = 2.5f;
	float distance = playerPosition.Distance(position);

	if (distance <= hitDistance)
	{
		//ヒット
		if (m_knockBackCount > 0)
		{
			//ノックバックさせる
			auto playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
			playerComponent->KnockBack();
		}
		else
		{
			m_isHitPlayerFlag = true;
		}
	}
	else
	{
		m_isHitPlayerFlag = false;
	}

	if (m_knockBackCount > 0)
	{
		m_knockBackCount--;
	}
}

void ButiEngine::StartPopUpComponent::ScaleUpdate()
{
	
	if (m_scaleAnimationFlag)
	{
		m_scaleAnimationFlag = false;
		auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(m_currentScale);
		anim->SetSpeed(0.0175f);
		anim->SetEaseType(Easing::EasingType::EaseOutElastic);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StartPopUpComponent::Clone()
{
	return ObjectFactory::Create<StartPopUpComponent>();
}
