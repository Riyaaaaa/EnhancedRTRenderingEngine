#include "Application.h"

#include <boost/timer.hpp>
#include <boost/format.hpp>

#include "Algorithms/SpaceOctree.h"
#include "Common/Defines.h"
#include "GUI/GUIFactory.h"

#include "Render/TextureHUDRenderer.h"
#include "Render/TextureEffects.h"
#include "Render/PostEffectRenderer.h"
#include "Render/SpaceOctreeVisualizer.h"
#include "Render/LineRenderer.h"
#include "Render/UnlitRenderer.h"
#include "Lighting/RayTracer.h"
#include "Lighting/PhotonMapping.h"
#include "Lighting/PrecomputeLightingSystem.h"
#include "Utility/TextureUtils.h"
#include "Utility/SceneUtils.h"
#include "Mesh/Primitive/Primitives.h"
#include "Resource/ResourceLoader.h"
#include "WindowsApp.h"
#include "dxgidebug.h"
#include "UserData/UserConfig.h"
#include "Common/Common.h"

#include "nuklear/nuklear.h"

#include <DirectXMath.h>

using namespace ERTRESystem;

void Application::Setup(void* runtimeWindowHandle)
{
	auto windowSize = Size2Dd(1280, 720);
	_view = std::make_shared<GIRenderView>();
	_cmd = std::make_unique<D3D11ImmediateCommands>();

	ViewportParam param;
	param.cfg.width = windowSize.w;
	param.cfg.height = windowSize.h;

	param.cfg.maxDepth = 1.0f;
	param.cfg.minDepth = 0.0f;
	param.cfg.topLeftX = 0;
	param.cfg.topLeftY = 0;

	param.MSAAQuality = MSAAQualityType::LOW_QUALITY;
	param.runtimeWindowHandle = runtimeWindowHandle;

	_view->Initialize(_cmd.get(), param);

	_scene = std::make_unique<Scene>();
	_scene->CreateGITestScene();

	_nuklear = std::make_shared<NuklearWrapper>(_cmd.get());
	_nuklearInput = std::make_shared<NuklearInputMediator>(_nuklear);
	_canvas = std::make_shared<HUDCanvas>();

	_forwardRenderer = std::make_shared<ForwardRenderer>(_cmd.get());
	_HUDRenderer = std::make_shared<HUDCanvasRenderer>(_cmd.get(), _view.get(), _nuklear);

	_renderScene = std::make_shared<RenderScene>(_scene.get());
	UserConfig::getInstance()->AddObserver(_renderScene.get());

	auto userConfig = UserConfig::getInstance();
	if (userConfig->VisibleIndirectLights()) {
		PrecomputeLightingSystem lightingSystem;
		lightingSystem.Compute(_cmd.get(), _renderScene.get());
	}

	SpaceOctreeVisualizer visualizer;
	visualizer.Initialize(_renderScene->_staticMeshesOctree.get());

	_renderScene->Preprocess(_cmd.get());

	_canvas->AddWindow(GUIFactory::CreateGlobalSettingsWindow());
	_canvas->AddWindow(GUIFactory::CreateApplicationMonitor());

	RegisterPressListener(100, std::bind(&NuklearInputMediator::ProcessInputDown, _nuklearInput.get(), std::placeholders::_1, std::placeholders::_2));
	RegisterReleaseListener(100, std::bind(&NuklearInputMediator::ProcessInputUp, _nuklearInput.get(), std::placeholders::_1, std::placeholders::_2));
	RegisterDragListener(100, std::bind(&NuklearInputMediator::ProcessInputMotion, _nuklearInput.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void Application::Update(float dt)
{
	auto config = UserConfig::getInstance();
	_renderScene->Refresh(_cmd.get());
	_forwardRenderer->render(_cmd.get(), _view.get(), _renderScene.get());

	if (config->IsDisplayRayPaths()) {
		LineRenderer::render(_cmd.get(), _view.get(), _scene->GetMainCamera(), _renderScene->rayPaths);
	}

	if (config->VisibleIndirectLights() && _renderScene->_lightMap) {
		TextureHUDRenderer::render(_cmd.get(), _view.get(), Vector2D(256, 256), Size2D(512, 512), _renderScene->_lightMap, 0);
	}

	//visualizer.RenderOctreeBoxes(cmd.get(), view.get(), scene);
	_HUDRenderer->update(_cmd.get(), _canvas.get(), _nuklear);
	_HUDRenderer->render(_cmd.get(), _view.get(), _nuklear);
	_view->Present();

	SetFPS(1 / dt);
}

void Application::BeginInput()
{
	_nuklearInput->BeginInput();
}

void Application::EndInput()
{
	_nuklearInput->EndInput();
}


void Application::RegisterPressListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener) {
	pressedKeyListeners.insert(std::make_pair(priority, listener));
}

void Application::RegisterReleaseListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener) {
	releasedKeyListeners.insert(std::make_pair(priority, listener));
}

void Application::RegisterDragListener(int priority, const std::function<bool(Index Delta, Index pos, InputKey key)>& listener) {
	dragListeners.insert(std::make_pair(priority, listener));
}

void Application::DispatchInputEvent(InputEvent e, InputKey key, boost::optional<Index> pos) {
	switch (e)
	{
	case InputEvent::PRESS:
		for (auto&& listener : pressedKeyListeners) {
			if (listener.second(key, pos)) {
				break;
			}
		}
		break;
	case InputEvent::RELEASE:
		for (auto&& listener : releasedKeyListeners) {
			if (listener.second(key, pos)) {
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Application::DispathDragEvent(InputKey key, Index Delta, Index pos) {
	for (auto&& listener : dragListeners) {
		if (listener.second(Delta, pos, key)) {
			break;
		}
	}
}

Application::Application()
{
	
}


Application::~Application()
{
}
