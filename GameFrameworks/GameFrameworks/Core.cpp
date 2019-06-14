#include "Core.h"

#include "PathUtil.h"
#include "ConfigUtil.h"
#include "Direct3D.h"
#include "Renderer.h"
#include "ObjectManager.h"

//#ifdef _DEBUG
//#define CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//// #include <new.h>등으로 operator new나 malloc을 
//// Derived해서 정의 한 경우, 사용 할 수 없다.
//#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#endif

namespace meltshine
{
	Core::Core()
		: _window(nullptr)
		, _instance(nullptr)
	{
	}

	Core::~Core()
	{
		ConfigUtil::Write(TEXT("Graphics"), TEXT("Width"), _d3d->_d3dpp.BackBufferWidth);
		ConfigUtil::Write(TEXT("Graphics"), TEXT("Height"), _d3d->_d3dpp.BackBufferHeight);
		ConfigUtil::Write(TEXT("Graphics"), TEXT("Windowed"), _d3d->_d3dpp.Windowed);
		ConfigUtil::Write(TEXT("Graphics"), TEXT("V-Synced"), _d3d->_vsynced);
	}

	std::shared_ptr<Core> Core::Create()
	{
		return std::shared_ptr<Core>(new Core);
	}

	bool Core::Init(HWND window, HINSTANCE instance)
	{
		//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		if (!window || !instance)
		{
			MessageBox(
				0, 
				TEXT("Core를 초기화하는데 실패했습니다. \n: 인자 'window' 또는 'instance'가 유효하지 않습니다."),
				TEXT("MeltShine GameFrameworks Error!"), MB_OK);
			return false;
		}

		// 멤버 설정
		_window = window;
		_instance = instance;

		PathUtil::Init();
		ConfigUtil::Init(PathUtil::GetDirectory());
		
		BOOL windowed = ConfigUtil::ReadInt(TEXT("Graphics"), TEXT("Windowed"));
		BOOL vsynced = ConfigUtil::ReadInt(TEXT("Graphics"), TEXT("V-Synced"));
		_d3d = std::shared_ptr<Direct3D>(new Direct3D);
		
		if (!_d3d || !_d3d->Init(_window, windowed, vsynced))
		{
			MessageBox(
				0,
				TEXT("Core를 초기화하는데 실패했습니다. \n: Direct3D를 초기화하는데 실패했습니다."),
				TEXT("MeltShine GameFrameworks Error!"), MB_OK);
			return false;
		}

		_renderer = std::shared_ptr<Renderer>(new Renderer);
		if (!_renderer || !_renderer->Init(_d3d->_device))
		{
			MessageBox(
				0,
				TEXT("Core를 초기화하는데 실패했습니다. \n: Renderer를 초기화하는데 실패했습니다."),
				TEXT("MeltShine GameFrameworks Error!"), MB_OK);
			return false;
		}

		_obj_mgr = std::shared_ptr<ObjectManager>(new ObjectManager);
		if (!_obj_mgr || !_obj_mgr->Init(shared_from_this()))
		{
			MessageBox(
				0,
				TEXT("Core를 초기화하는데 실패했습니다. \n: ObjectManager를 초기화하는데 실패했습니다."),
				TEXT("MeltShine GameFrameworks Error!"), MB_OK);
			return false;
		}

		return true;
	}

	void Core::Run()
	{
		_renderer->Clear(D3DCOLOR_XRGB(0, 0, 0), D3DCLEAR_TARGET);
		_renderer->DrawSprite(0, 0, 0xFFFFFFFF);
		_renderer->Render();
	}

	void Core::GetWinSize(int& w, int& h) const
	{
		RECT rect = {};
		GetClientRect(_window, &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}

	int Core::GetWinWidth() const
	{
		RECT rect = {};
		GetClientRect(_window, &rect);
		return rect.right - rect.left;
	}

	int Core::GetWinHeight() const
	{
		RECT rect = {};
		GetClientRect(_window, &rect);
		return rect.bottom - rect.top;
	}

}