
#include"Header/ApplicationCreater.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"
#include"Header/Device//ModelFileConverter.h"
#include"ButiFont/ButiFont.h"
#include"ButiEventSystem/ButiEventSystem/TaskSystem.h"
#define _CRTDBG_MAP_ALLOC

using namespace::ButiEngine;

#ifdef DEBUG

std::int32_t main()
{
	std::cout << "debug" << std::endl;
#else
std::int32_t APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR arg, std::int32_t)
{
#endif
	std::int32_t returnCode = 0;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _IMGUIEDITOR
	ButiTaskSystem::Start();
	auto init = InputApplicationInitData();
	auto vlp_app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(vlp_app);
	GameDevice::SetReturnCode(0);
	GameDevice::SetIsEnd(false);
	vlp_app->PreLoadResources();
	vlp_app->InitLoadResources();
	vlp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);
	vlp_app->GetGraphicDevice()->SetClearColor(Vector4(0, 0, 0, 1));
	returnCode = vlp_app->Run();
	vlp_app->Exit();
	ButiTaskSystem::Dispose();


#else
	auto init = InputApplicationInitData();
	auto app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	//GameDevice::GetInput()->SetCursorHide(true);
	GameDevice::GetInput()->Initialize(app);
	app->PreLoadResources();
	app->InitLoadResources();

	app->GetSceneManager()->LoadScene_Init(init.initSceneName);


	app->GetGraphicDevice()->SetClearColor(Vector4(0.5, 0.1, 0.5, 1));
	returnCode = app->Run();
	app->Exit();
#endif // _EDITORBUILD

	return returnCode;
}