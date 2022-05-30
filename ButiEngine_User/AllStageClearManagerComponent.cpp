#include "stdafx_u.h"
#include "AllStageClearManagerComponent.h"
#include "WorldSpeedManager.h"
#include "MoveAnimationComponent.h"
#include "CameraComponent.h"
#include "SoundPlayerComponent.h"
#include "Enemy.h"
#include "FadeOutComponent.h"
#include "CrossBeamEffectComponent.h"
#include "InputManager.h"

void ButiEngine::AllStageClearManagerComponent::OnUpdate()
{
	//スロー効果が終わるまで待つ
	if (!m_vlp_waitTimer->Update_continue()) { return; }

	if (m_vlp_addTimer->Update())
	{
		AddUI();
	}
}

void ButiEngine::AllStageClearManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(120);
	m_vlp_addTimer = ObjectFactory::Create<AbsoluteTimer>(20);
}

void ButiEngine::AllStageClearManagerComponent::OnShowUI()
{
}

void ButiEngine::AllStageClearManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_addTimer->Start();

	//このコンポーネントが生成された瞬間にスロー効果をつける
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.2f, 120);

	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();

	auto enemys = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));
	for (auto itr = enemys.begin(); itr != enemys.end(); ++itr)
	{
		(*itr)->GetGameComponent<Enemy>()->Dead();
	}

	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Big.wav"));
	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Crystal.wav"));
	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Crystal.wav"));

	m_uiCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::AllStageClearManagerComponent::Clone()
{
	return ObjectFactory::Create<AllStageClearManagerComponent>();
}

void ButiEngine::AllStageClearManagerComponent::AddUI()
{
	if (m_uiCount < 5)
	{
		if (m_uiCount == 0)
		{
			auto fadeUIComponent = GetManager().lock()->AddObjectFromCereal("FadeOutUI").lock()->GetGameComponent<FadeOutComponent>();
			fadeUIComponent->SetMaxAlpha(1.0f);

			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0);
		}
	}
	else if (m_uiCount < 10)
	{
		std::string objectName = "ClearUI_" + std::to_string(m_uiCount - 5);
		auto clearUI = GetManager().lock()->AddObjectFromCereal(objectName);
		auto position = clearUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = clearUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(-600 + 300 * (m_uiCount - 5), 200, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.9f);
		moveAnimationComponent->SetIsShake(true);
		moveAnimationComponent->SetIsActive(true);

		auto crossBeamEffect = GetManager().lock()->AddObjectFromCereal("CrossBeamEffect").lock()->GetGameComponent<CrossBeamEffectComponent>();
		endPosition.z -= (float)m_uiCount * 0.001f;
		crossBeamEffect->SetPosition(endPosition);
	}
	else if (m_uiCount == 13)
	{
		auto backToSelectUI = GetManager().lock()->AddObjectFromCereal("BackToSelectUI");
		auto position = backToSelectUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = backToSelectUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(0, -250, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(0.9f);
		moveAnimationComponent->SetIsShake(true);
		moveAnimationComponent->SetIsActive(true);
	}
	else if (m_uiCount == 14)
	{
		m_isNext = true;
	}

	if (m_uiCount < 15)
	{
		m_uiCount++;
	}
	else
	{

		m_vlp_addTimer->Stop();
	}
}
