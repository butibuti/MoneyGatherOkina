#include "stdafx_u.h"
#include "TitleManagerComponent.h"
#include"StageSelectManagerComponent.h"
#include"FadeOutComponent.h"
#include "InputManager.h"
#include "SoundPlayerComponent.h"

//bool isInit = false;

void ButiEngine::TitleManagerComponent::OnUpdate()
{
	//return;
	//if (!m_isTitle) {
	//	return;
	//}

	//if ( !m_vlp_timer->IsOn()&&!m_vlp_selectUIApperTimer->IsOn() && !m_vwp_fadeComponent.lock()->IsFadeAnimation()&& InputManager::IsTriggerDecideKey())
	//{
	//	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
	//	m_vlp_timer->Start();
	//}
	//if (m_vlp_timer->IsOn()) {
	//	GetManager().lock()->GetGameObject("TitleLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = Easing::EaseInSin(1.0f - m_vlp_timer->GetPercent());
	//}

	//if (m_vlp_timer->Update()) {
	//	m_vlp_timer->Stop();
	//	m_vlp_selectUIApperTimer->Start();
	//}


	//if (m_vlp_selectUIApperTimer->IsOn()) {
	//	GetManager().lock()->GetGameObject("RemainUI").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = Easing::EaseInSin(m_vlp_selectUIApperTimer->GetPercent());
	//	GetManager().lock()->GetGameObject("StartButtonUI").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = Easing::EaseInSin(m_vlp_selectUIApperTimer->GetPercent());
	//}
	//if (m_vlp_selectUIApperTimer->Update()) {
	//	m_vlp_selectUIApperTimer->Stop();
	//	m_isTitle = false;
	//}
}

void ButiEngine::TitleManagerComponent::OnSet()
{
}

void ButiEngine::TitleManagerComponent::OnShowUI()
{
}

void ButiEngine::TitleManagerComponent::Start()
{
	//if (!m_vwp_fadeComponent.lock()) {
	//	m_vwp_fadeComponent = GetManager().lock()->GetGameObject("FadeOutUI").lock()->GetGameComponent<FadeOutComponent>();
	//	m_vwp_fadeComponent.lock()->SetIsFade(true);
	//}
	//if (!m_vlp_timer) {
	//	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);
	//	m_vlp_selectUIApperTimer = ObjectFactory::Create<RelativeTimer>(60);
	//}
	//m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
}


ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleManagerComponent::Clone()
{
	return ObjectFactory::Create<TitleManagerComponent>();
}
