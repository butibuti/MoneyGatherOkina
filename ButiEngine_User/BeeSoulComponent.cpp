#include "stdafx_u.h"
#include "BeeSoulComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::BeeSoulComponent::OnUpdate()
{
    if (m_residualFrame > 0)
    {
        m_residualFrame--;

        //スクリーン座標に変換
        Vector3 screenPosition = GetCamera("main")->WorldToScreen(m_residualPosition);
        screenPosition.z = 0;

        gameObject.lock()->transform->SetLocalPosition(screenPosition);
    }
    else
    {
        Move();
    }


    Animation();

    if (m_animationCount >= m_maxAnimationCount)
    {
        m_animationCount = 0;
    }
    if (m_life > 0)
    {
        m_life--;
    }
    else
    {
        gameObject.lock()->SetIsRemove(true);
        return;
    }
}

void ButiEngine::BeeSoulComponent::OnSet()
{
}

void ButiEngine::BeeSoulComponent::OnRemove()
{
}

void ButiEngine::BeeSoulComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulComponent::Start()
{
    m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
    m_speed = 0.02f;
    m_residualFrame = 60;
    m_life = (std::int16_t)(1.0f / m_speed) + m_residualFrame;
    m_animationFrame = 0;
    m_animationRate = 4;
    m_animationCount = 0;
    m_maxAnimationCount = 2;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulComponent>();
}

void ButiEngine::BeeSoulComponent::Move()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(-850, 300, 0));
        anim->SetSpeed(m_speed);
        anim->SetEaseType(Easing::EasingType::EaseInOutQuad);
    }
}

void ButiEngine::BeeSoulComponent::Animation()
{
    m_animationFrame++;

    if (m_animationFrame < m_animationRate) { return; }

    m_animationFrame = 0;

    m_animationCount++;

    m_vwp_spriteAnimationComponent.lock()->UpdateHorizontalAnim(1);
}
