#include "stdafx_u.h"
#include "BeeSoulComponent.h"
#include "BeeSoulPodUIComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::BeeSoulComponent::OnUpdate()
{
    if (!m_vlp_waitTimer->Update_continue())
    {
        //スクリーン座標に変換
        Vector3 screenPosition = GetCamera("main")->WorldToScreen(m_residualPosition);
        screenPosition.z = 2;

        gameObject.lock()->transform->SetLocalPosition(screenPosition);
    }
    else
    {
        Move();
    }

    if (m_vlp_timer->Update())
    {
        Animation();
    }

    if (m_animationCount >= m_maxAnimationCount)
    {
        m_animationCount = 0;
    }
    if (m_vlp_lifeTimer->Update())
    {
        GetManager().lock()->GetGameObject("BeeSoulPod").lock()->GetGameComponent<BeeSoulPodUIComponent>()->AddSoulCount();
        gameObject.lock()->SetIsRemove(true);
    }
}

void ButiEngine::BeeSoulComponent::OnSet()
{
    m_vlp_timer = ObjectFactory::Create<RelativeTimer>();
    m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
    m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::BeeSoulComponent::OnRemove()
{
}

void ButiEngine::BeeSoulComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulComponent::Start()
{
    m_vlp_timer->Start();
    m_vlp_timer->ChangeCountFrame(4);
    m_vlp_waitTimer->Start();
    m_vlp_waitTimer->ChangeCountFrame(30);
    m_vwp_spriteAnimationComponent = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();
    m_speed = 0.02f;
    m_animationCount = 0;
    m_maxAnimationCount = 2;
    m_vlp_lifeTimer->Start();
    m_vlp_lifeTimer->ChangeCountFrame((std::int16_t)(1.0f / m_speed) + 30);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulComponent>();
}

void ButiEngine::BeeSoulComponent::Move()
{
    float speed = m_speed * GameDevice::WorldSpeed;
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetLocalPosition(Vector3(-730, 275, 0.01f));
        anim->SetSpeed(speed);
        anim->SetEaseType(Easing::EasingType::EaseInExpo);
    }
    else
    {
        anim->SetSpeed(speed);
    }
}

void ButiEngine::BeeSoulComponent::Animation()
{
    m_animationCount++;

    m_vwp_spriteAnimationComponent.lock()->UpdateHorizontalAnim(1);
}
