#include "stdafx_u.h"
#include "StageClearManagerComponent.h"
#include "WorldSpeedManager.h"
#include "MoveAnimationComponent.h"
#include "CameraComponent.h"

void ButiEngine::StageClearManagerComponent::OnUpdate()
{
	//スロー効果が終わるまで待つ
	if (!m_vlp_waitTimer->Update_continue()) { return; }

	//スロー効果が終わったらaddTimerのフレーム毎にオブジェクトを右上に追加していく
	if (m_vlp_addTimer->Update())
	{
		AddUI();
	}
}

void ButiEngine::StageClearManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(120);
	m_vlp_addTimer = ObjectFactory::Create<AbsoluteTimer>(7);
}

void ButiEngine::StageClearManagerComponent::OnShowUI()
{
}

void ButiEngine::StageClearManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_addTimer->Start();

	//このコンポーネントが生成された瞬間にスロー効果をつける
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.2f, 120);

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
		if (m_uiCount == 0)
		{
			auto cameraComponent = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraComponent>();
			cameraComponent->SetZoomOperationNum(0);
		}
		std::string objectName = "ClearUI_" + std::to_string(m_uiCount);
		auto clearUI = GetManager().lock()->AddObjectFromCereal(objectName);
		auto position = clearUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = clearUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(-750 + 250 * m_uiCount, 100 + 50 * m_uiCount, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.9f);
		moveAnimationComponent->SetIsShake(true);
	}
	else if (m_uiCount == 8)
	{
		auto backToSelectUI = GetManager().lock()->AddObjectFromCereal("BackToSelectUI");
		auto position = backToSelectUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = backToSelectUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(350, -180, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.9f);
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
