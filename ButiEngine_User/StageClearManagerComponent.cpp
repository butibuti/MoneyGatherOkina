#include "stdafx_u.h"
#include "StageClearManagerComponent.h"
#include "WorldSpeedManager.h"
#include "MoveAnimationComponent.h"

void ButiEngine::StageClearManagerComponent::OnUpdate()
{
	//�X���[���ʂ��I���܂ő҂�
	if (!m_vlp_waitTimer->Update_continue()) { return; }

	//�X���[���ʂ��I�������addTimer�̃t���[�����ɃI�u�W�F�N�g���E��ɒǉ����Ă���
	if (m_vlp_addTimer->Update())
	{
		AddUI();
	}

	//�����ōX�V

}

void ButiEngine::StageClearManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(180);
	m_vlp_addTimer = ObjectFactory::Create<AbsoluteTimer>(8);
}

void ButiEngine::StageClearManagerComponent::OnShowUI()
{
}

void ButiEngine::StageClearManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_addTimer->Start();

	//���̃R���|�[�l���g���������ꂽ�u�ԂɃX���[���ʂ�����
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.2f, 180);

	m_uiCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageClearManagerComponent::Clone()
{
	return ObjectFactory::Create<StageClearManagerComponent>();
}

void ButiEngine::StageClearManagerComponent::AddUI()
{
	if (m_uiCount < 5)
	{
		std::string objectName = "ClearUI_" + std::to_string(m_uiCount);
		auto clearUI = GetManager().lock()->AddObjectFromCereal(objectName);
		auto position = clearUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = clearUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(-750 + 250 * m_uiCount, 100 + 60 * m_uiCount, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.75f);
		moveAnimationComponent->SetIsShake(true);
	}
	else if (m_uiCount == 8)
	{
		auto backToSelectUI = GetManager().lock()->AddObjectFromCereal("BackToSelectUI");
		auto position = backToSelectUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = backToSelectUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(350, -180, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.75f);
		moveAnimationComponent->SetIsShake(true);
	}
	else if(m_uiCount == 9)
	{
		m_isNext = true;
	}

	if (m_uiCount < 11)
	{
		m_uiCount++;
	}
}
