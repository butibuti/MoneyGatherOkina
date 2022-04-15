
#include"Header/ApplicationCreater.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"
#include"Header/Device//ModelFileConverter.h"
#include"ButiFont/ButiFont.h"
#include"ButiEventSystem/ButiEventSystem/TaskSystem.h"
#define _CRTDBG_MAP_ALLOC

using namespace::ButiEngine;

#ifdef DEBUG

#ifdef _EDITORBUILD
std::int32_t main(const std::int32_t argCount, const char* args[])
#else

std::int32_t main() 

#endif
{
	std::cout << "debug" << std::endl;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else

std::int32_t APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR arg, std::int32_t)
{
#endif
	std::int32_t returnCode = 0;

#ifdef _EDITORBUILD

	//引数あり実行ならリソースのディレクトリを変更する
#ifdef DEBUG
	if (argCount > 1) {
		GlobalSettings::SetResourceDirectory(args[1]);
	}
#else

	auto argments = Util::WStringToString(std::wstring(GetCommandLine()));
	auto splitedArgments = StringHelper::Split(argments, " ");
	if (splitedArgments.size() > 1)
		GlobalSettings::SetResourceDirectory(splitedArgments[1]);
#endif

#ifdef _IMGUIEDITOR
	ButiTaskSystem::Start(8);
	auto init = InputApplicationInitData();
	auto shp_app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(shp_app);
	GameDevice::SetReturnCode(0);
	GameDevice::SetIsEnd(false);
	shp_app->PreLoadResources();
	shp_app->InitLoadResources();
	shp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);
	shp_app->GetGraphicDevice()->SetClearColor(Vector4(0, 0, 0, 1));
	returnCode = shp_app->Run();
	shp_app->Exit();

	ButiTaskSystem::Dispose();
#else

	auto server = CreateRuntimeServer();

	returnCode = server->Start();
#endif


#else
	auto init = InputApplicationInitData();
	auto app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	//GameDevice::GetInput()->SetCursorHide(true);
	GameDevice::GetInput()->Initialize(app);
	app->PreLoadResources();
	app->InitLoadResources();

	app->GetSceneManager()->LoadScene_Init("Renewal_test");


	app->GetGraphicDevice()->SetClearColor(Vector4(0.5, 0.1, 0.5, 1));
	returnCode = app->Run();
	app->Exit();
#endif // _EDITORBUILD

	return returnCode;
}