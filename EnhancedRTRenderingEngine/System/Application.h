#pragma once

#include <map>

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include "RenderCore.D3D11/D3D11ImmediateCommands.h"

#include "GUI/HUDCanvas.h"
#include "GUI/NuklearInputMediator.h"
#include "GUI/NuklearWrapper.h"

#include "Render/ForwardRenderer.h"
#include "Render/HUDCanvasRenderer.h"

namespace ERTRESystem {
    class Application : public libspiral::SingletonBase<Application>
    {
    public:
        void Setup(void* runtimeWindowHandle);
        Application();
        virtual ~Application();

        void Update(float dt);
        void BeginInput();
        void EndInput();

        void SetFPS(double fps) {
            _fps = fps;
        }

        double FPS() {
            return _fps;
        }

        void DispatchInputEvent(InputEvent e, InputKey key, boost::optional<Index> pos);
        void DispathDragEvent(InputKey key, Index Delta, Index pos);

        void RegisterPressListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener);
        void RegisterReleaseListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener);
        void RegisterDragListener(int priority, const std::function<bool(Index Delta, Index pos, InputKey key)>& listener);

    protected:
        std::shared_ptr<NuklearWrapper> _nuklear;
        std::shared_ptr<NuklearInputMediator> _nuklearInput;
        std::shared_ptr<HUDCanvas> _canvas;
        std::shared_ptr<HUDCanvasRenderer> _HUDRenderer;
        std::shared_ptr<ForwardRenderer> _forwardRenderer;
        std::shared_ptr<RenderScene> _renderScene;
        std::shared_ptr<GIRenderView> _view;
        std::unique_ptr<D3D11ImmediateCommands> _cmd;
        std::unique_ptr<Scene> _scene;

        std::map<int, std::function<bool(InputKey key, boost::optional<Index>)>, std::greater<int>> pressedKeyListeners;
        std::map<int, std::function<bool(InputKey key, boost::optional<Index>)>, std::greater<int>> releasedKeyListeners;
        std::map<int, std::function<bool(Index Delta, Index pos, InputKey key)>, std::greater<int>> dragListeners;

        double _fps = 0.0f;
    };
}

