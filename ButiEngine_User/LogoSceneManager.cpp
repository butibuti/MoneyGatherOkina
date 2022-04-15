
#include "LogoSceneManager.h"
#include"Header/ApplicationCreater.h"
void ButiEngine::LogoSceneManager::OnUpdate()
{
   
    auto app = GetManager().lock()->GetApplication().lock();

    if (shp_transTimer->Update()) {
        isTrans = true;
        shp_transTimer->Stop();
        
    }
    if (isTrans&&!app->GetResourceContainer()->IsLoading()) {

        //app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
        app->GetSceneManager()->LoadScene(gameObject.lock()->GetApplication().lock()->GetAppInitData()->initSceneName);
        app->GetSceneManager()->ChangeScene(gameObject.lock()->GetApplication().lock()->GetAppInitData()->initSceneName);
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LogoSceneManager::Clone()
{
    auto output = ObjectFactory::Create<LogoSceneManager>();
    if (shp_timer) {
        output->shp_timer = shp_timer->Clone();
    }if (shp_transTimer) {
        output->shp_transTimer = shp_transTimer->Clone();
    }
    return output;
}

void ButiEngine::LogoSceneManager::OnShowUI()
{
    if (GUI::TreeNode("Timer")) {
        shp_timer->ShowGUI();
        GUI::TreePop();
    }
    if (GUI::TreeNode("TransitionTimer")) {
        shp_transTimer->ShowGUI();
        GUI::TreePop();
    }
    GUI::BulletText("logoSpeed");
    GUI::DragFloat("##logoapp", logoApperanceSpeed, 0.001f);
}

void ButiEngine::LogoSceneManager::Start()
{
    shp_logoBuffer = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ObjectInformation>("ObjectInformation");
    shp_teamBuffer = GetManager().lock()->GetGameObject("TeamLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ObjectInformation>("ObjectInformation");



    GetManager().lock()->AddObjectFromCereal("LoadAnim", ObjectFactory::Create<Transform>(Vector3(0, 0, 0.1), Vector3::Zero, Vector3(0, 0, 1)));

    shp_transTimer->Start();
    auto app = GetManager().lock()->GetApplication().lock();
    app->InitLoadResources_async();
    
}

void ButiEngine::LogoSceneManager::OnSet()
{
    if (!shp_timer) {
        shp_timer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
    if (!shp_transTimer) {
        shp_transTimer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
}
