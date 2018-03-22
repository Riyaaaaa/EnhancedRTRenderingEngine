#pragma once

#include <memory>

#include "D3D11TextureProxy.h"
#include "D3D11RenderView.h"
#include "Scene/Scene.h"

class D3D11UnlitRenderer
{
public:
    template<class VertType>
    void render(const CameraObject& camera, std::vector<MeshObject<VertType>>& meshes);
    bool Initialize(const std::shared_ptr<D3D11RenderView>& view);

protected:
    std::shared_ptr<D3D11RenderView> _view;
};

