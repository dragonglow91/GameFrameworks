#include "Scene.h"

#include <algorithm>

#include "Camera.h"
#include "ObjectManager.h"
namespace meltshine
{
	Scene::Scene()
		: _dirty_cameras(false)
	{
	}

	Scene::~Scene()
	{
	}

	bool Scene::Init(std::shared_ptr<Core> core)
	{
		if (!GameObject::Init(core))
		{
			return false;
		}

		// 기본 카메라 게임 오브젝트를 생성해 자식으로 추가합니다.
		auto default_camera = GetObjectManager()->CreateCamera();
		default_camera->SetName("Default Camera");
		AddChild(default_camera);
		return true;
	}

	void Scene::LateUpdate()
	{
		GameObject::LateUpdate();
		SortCameras();
	}

	void Scene::SortCameras()
	{
		if (!_dirty_cameras)
		{
			return;
		}
		auto greater = [](std::shared_ptr<Camera> src, std::shared_ptr<Camera> dst)->bool
		{
			return src->GetDepth() < dst->GetDepth();
		};
		std::sort(_cameras.begin(), _cameras.end(), greater);
		_dirty_cameras = false;
	}

}