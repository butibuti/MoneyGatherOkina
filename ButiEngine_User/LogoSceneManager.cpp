
#include "LogoSceneManager.h"
#include"Header/ApplicationCreater.h"
#include"SceneChangeAnimationComponent.h"
void ButiEngine::LogoSceneManager::OnUpdate()
{
    if (m_vlp_timer) {

        if (m_vlp_timer->Update()) {
            m_vec_timerResetFuncion[m_index]();
        }
        else {
            m_vec_timerUpdatingFunction[m_index]();
        }
    }
    if(m_vwp_sceneChangeAnimation.lock()&& !m_vwp_sceneChangeAnimation.lock()->IsAnimation()){
        auto app = GetManager().lock()->GetApplication().lock();
        if (!app->GetResourceContainer()->IsLoading()) {

            //app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
#ifndef DEBUG
            app->InitLoadResources();
#endif // !DEBUG    
            std::string sceneName = gameObject.lock()->GetApplication().lock()->GetAppInitData()->initSceneName;
            app->GetSceneManager()->RemoveScene(sceneName);
            app->GetSceneManager()->LoadScene(sceneName);
            app->GetSceneManager()->ChangeScene(sceneName);
        }
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LogoSceneManager::Clone()
{
    auto output = ObjectFactory::Create<LogoSceneManager>();
    output->m_logokeepTime = m_logokeepTime;
    output->m_logoApperanceTime = m_logoApperanceTime;
    output->m_logoDisappearanceTime = m_logoDisappearanceTime;
    output->m_sceneChangeWait = m_sceneChangeWait;
    return output;
}

void ButiEngine::LogoSceneManager::OnShowUI()
{
    GUI::DragFloat("LogoAppearance", m_logoApperanceTime);
    GUI::DragFloat("LogoDisappearance", m_logoDisappearanceTime);
    GUI::DragFloat("LogoKeep", m_logokeepTime);
    GUI::DragFloat("Wait", m_sceneChangeWait);
}

void ButiEngine::LogoSceneManager::Start()
{
    m_vlp_logoBuffer = GetManager().lock()->GetGameObject("EngineLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation");
    m_vlp_teamBuffer = GetManager().lock()->GetGameObject("TeamLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation");
    m_vwp_sceneChangeAnimation = gameObject.lock()->GetGameComponent<SceneChangeAnimationComponent>();
    m_vwp_sceneChangeAnimation.lock()->HidePanel();

    m_vec_timerCounts.push_back(m_logoApperanceTime);
    m_vec_timerCounts.push_back(m_logokeepTime);
    m_vec_timerCounts.push_back(m_logoDisappearanceTime);
    m_vec_timerCounts.push_back(m_sceneChangeWait);
    m_vec_timerCounts.push_back(m_logoApperanceTime);
    m_vec_timerCounts.push_back(m_logokeepTime);
    m_vec_timerCounts.push_back(m_logoDisappearanceTime);
    m_vec_timerCounts.push_back(m_sceneChangeWait);

    m_vlp_timer = ObjectFactory::Create<AbsoluteTimer>(m_vec_timerCounts[m_index]);
    m_vlp_timer->Start();
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        m_vlp_logoBuffer->Get().color.w = m_vlp_timer->GetPercent();
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        m_vlp_logoBuffer->Get().color.w = 1.0 - m_vlp_timer->GetPercent();
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        m_vlp_teamBuffer->Get().color.w = m_vlp_timer->GetPercent();
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        m_vlp_teamBuffer->Get().color.w = 1.0 - m_vlp_timer->GetPercent();
        });
    m_vec_timerUpdatingFunction.push_back([this]()->void {
        });

    m_vec_timerResetFuncion.push_back([this]()->void {
        m_vlp_logoBuffer->Get().color.w = 1.0f;
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_vlp_logoBuffer->Get().color.w = 0.0f;
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_vlp_teamBuffer->Get().color.w = 1.0f;
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_vlp_teamBuffer->Get().color.w = 0.0f;
        m_index++;
        m_vlp_timer->ChangeCountFrame(m_vec_timerCounts[m_index]);
        });
    m_vec_timerResetFuncion.push_back([this]()->void {
        m_vwp_sceneChangeAnimation.lock()->SceneEnd();
        m_vlp_timer = nullptr;
        });
}

void ButiEngine::LogoSceneManager::OnSet()
{
}
