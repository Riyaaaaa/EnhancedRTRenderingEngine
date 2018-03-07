#pragma once

#include <d3d11.h>

#include <memory>
#include <vector>

#include "../Structure/Structure.h"
#include "D3DX11RenderView.h"
#include "../Scene/Scene.h"

#include "D3D11Scene.h"
#include "D3D11DepthRenderer.h"
#include "D3D11BasePassRenderer.h"

class D3D11ForwardRenderer
{
public:

    void Initialize(std::shared_ptr<D3DX11RenderView> view);

    D3D11ForwardRenderer();
    ~D3D11ForwardRenderer();

    void render(Scene* scene);

    D3D11Scene& GetScene() { return _scene; }

protected:
    D3D11Scene _scene;
    D3D11DepthRenderer depthRenderer;
    D3D11BasePassRenderer bassPassRenderer;
};

