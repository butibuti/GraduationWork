
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"
#include"Header/Device//ModelFileConverter.h"
#include"ButiFont/ButiFont.h"
//#include"Server/RuntimeServer.h"
#include"ButiEventSystem/ButiTaskSystem/TaskSystem.h"
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

std::int32_t APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR arg, std::int32_t)
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

	auto argments = GetCommandLine();
	auto splitedArgments = StringHelper::Split(argments, " ");
	if (splitedArgments.size() > 1)
		GlobalSettings::SetResourceDirectory(splitedArgments[1]);
#endif

#ifdef _IMGUIEDITOR
	ButiTaskSystem::Start();
	ResourceSystem::Start();
	GameAssetData::ApplicationInitData init;
	InputCereal(init, "Application/ButiEngine.ini");
	OutputCereal(init, "Application/ButiEngine.ini");
	auto vlp_app = CreateApplicationInstance(init);
	GameDevice::Initialize();
	GameDevice::InitializeInput(vlp_app->GetWindow()->GetHandle(),init.isUseVRTracker);
	GameDevice::SetReturnCode(0);
	GameDevice::SetIsEnd(false);
	vlp_app->PreLoadResources();
	vlp_app->InitLoadResources();
	vlp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);

	vlp_app->GetGraphicDevice()->SetClearColor(ButiColor::Blue(ButiColor::ShadeIndex::Shade_9));
	returnCode = vlp_app->Run();
	vlp_app->Exit();

	ButiTaskSystem::Dispose();
	ResourceSystem::End();
#else

	auto server = CreateRuntimeServer();

	returnCode = server->Start();
#endif


#else

	ResourceSystem::Start();
	std::stringstream stream;
	auto size = ResourceSystem::GetResourceSize("Application/ButiEngine.ini");
	auto begin = ResourceSystem::GetResourcePtr("Application/ButiEngine.ini"),end= ResourceSystem::GetResourcePtr("Application/ButiEngine.ini")+size;
	std::copy(begin,end, std::ostream_iterator<char>(stream, ""));

	GameAssetData::ApplicationInitData init;
	InputCereal(init, "Application/ButiEngine.ini");
#ifdef DEBUG
#else
	init.hInstance = hInstance;
#endif // !DEBUG

	
	auto vlp_app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	GameDevice::InitializeInput(vlp_app->GetWindow()->GetHandle(), init.isUseVRTracker);
	vlp_app->PreLoadResources();
	vlp_app->InitLoadResources();
	vlp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);


	vlp_app->GetGraphicDevice()->SetClearColor(ButiColor::DeepOrange());
	returnCode = vlp_app->Run();
	vlp_app->Exit();
	ResourceSystem::End();
#endif // _EDITORBUILD

	return returnCode;
}