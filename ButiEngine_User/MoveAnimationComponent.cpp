#include "stdafx_u.h"
#include "MoveAnimationComponent.h"
#include "ShakeComponent.h"

void ButiEngine::MoveAnimationComponent::OnUpdate()
{
	if (!m_isActive) { return; }

	float distance = m_currentPosition.Distance(m_endPosition);
	float difference = std::abs(distance);

	//現在の位置と終点との差がボーダーを下回ったら終点位置にセットしてアニメーション終了
	if (difference < m_borderDistance)
	{
		if (m_isShake)
		{
			auto shakeComponent = gameObject.lock()->GetGameComponent<ShakeComponent>();
			shakeComponent->Shake(0.5f, 6);
			shakeComponent->SetShakeAxis(Vector3(0, 0, 1));
			shakeComponent->SetIsAbsolute(true);
		}
		m_currentPosition = m_endPosition;
		m_isActive = false;
	}
	else
	{
		m_currentPosition.x = MathHelper::Lerp(m_currentPosition.x, m_endPosition.x, m_speed);
		m_currentPosition.y = MathHelper::Lerp(m_currentPosition.y, m_endPosition.y, m_speed);
		m_currentPosition.z = MathHelper::Lerp(m_currentPosition.z, m_endPosition.z, m_speed);
	}

	gameObject.lock()->transform->SetLocalPosition(m_currentPosition);
}

void ButiEngine::MoveAnimationComponent::OnSet()
{
	m_startPosition = gameObject.lock()->transform->GetLocalPosition();
	m_endPosition = Vector3(0, 0, 0);
	m_speed = 0.2f;
	m_isActive = false;
	m_isShake = false;
}

void ButiEngine::MoveAnimationComponent::OnRemove()
{
}

void ButiEngine::MoveAnimationComponent::OnShowUI()
{
}

void ButiEngine::MoveAnimationComponent::Start()
{
	m_currentPosition = m_startPosition;

	//自分の大きさの1/20の距離をボーダーにする
	auto scale = gameObject.lock()->transform->GetLocalScale();
	auto radius = (scale.x + scale.y) * 0.5f;
	m_borderDistance = radius * 0.05f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::MoveAnimationComponent::Clone()
{
	return ObjectFactory::Create<MoveAnimationComponent>();
}
