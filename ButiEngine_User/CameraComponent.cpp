#include "stdafx_u.h"
#include "CameraComponent.h"

void ButiEngine::CameraComponent::OnUpdate()
{
    //gameObject.lock()->transform->SetLookAtRotation(m_vwp_cameraAxis.lock()->transform->GetLocalPosition(), Vector3(0, 1, 0));
}

void ButiEngine::CameraComponent::OnSet()
{
}

void ButiEngine::CameraComponent::Start()
{
    //�J�����̏����ʒu
    m_currentCameraPos = gameObject.lock()->transform->GetLocalPosition();
    m_previousCameraPos = m_currentCameraPos;

    if (m_vwp_cameraAxis.lock() == nullptr)
    {
        //�J������]���p�̐e�I�u�W�F�N�g�̎擾
        m_vwp_cameraAxis = GetManager().lock()->GetGameObject("CameraAxis");

        //cameraAxis��e�Ƃ��ăZ�b�g
        gameObject.lock()->transform->SetBaseTransform(m_vwp_cameraAxis.lock()->transform);

        //�e�I�u�W�F�N�g�Ƃ̑��΋����̎擾
        m_relativePosition = m_currentCameraPos - m_vwp_cameraAxis.lock()->transform->GetLocalPosition();
    }

}

void ButiEngine::CameraComponent::OnShowUI()
{
    GUI::BulletText("RelativePosition");
    GUI::DragFloat3("##relativePosition", m_relativePosition, 0.01f, -100.0f, 100.0f);
}

void ButiEngine::CameraComponent::ZoomIn()
{
    //auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    //if (!anim)
    //{
    //    anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
    //    anim->SetTargetTransform(gameObject.lock()->transform->Clone());
    //    anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, m_initCameraZ));
    //    anim->SetSpeed(1.0f / m_zoomInFrame);
    //    anim->SetEaseType(Easing::EasingType::EaseOutQuart);
    //}
}

void ButiEngine::CameraComponent::ZoomOut()
{
    //auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    //if (!anim)
    //{
    //    anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
    //    anim->SetTargetTransform(gameObject.lock()->transform->Clone());
    //    anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, m_initCameraZ - m_moveLength));
    //    anim->SetSpeed(1.0f / m_zoomOutFrame);
    //    anim->SetEaseType(Easing::EasingType::EaseInQuad);
    //}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraComponent::Clone()
{
	return ObjectFactory::Create<CameraComponent>();
}
