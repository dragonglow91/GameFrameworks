#include "Application.h"
#include "Core.h"
#include "ObjectManager.h"
#include "SceneController.h"
#include "DefaultScene.h"

using namespace meltshine;
int main()
{
	auto app = meltshine::Application::GetInstance();
	if (!app->Init(800, 600, TEXT("HelloWorld")))
	{
		return -1;
	}
	
	auto core = Core::Create();
	if (!core->Init(app->GetWindowHandle(), app->GetInstanceHandle()))
	{
		return -1;
	}

	auto scene = core->GetObjectManager()->CreateSceneAs<DefaultScene>();
	core->GetSceneController()->PushScene(scene);
	app->SetOnRunCallback(std::bind(&Core::Run, core));
	return app->Run();
}