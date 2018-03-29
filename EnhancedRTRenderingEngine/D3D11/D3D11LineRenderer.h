#pragma once

#include "D3D11RenderView.h"
#include "Scene/Scene.h"

#include "Structure/Primitive.h"

class D3D11LineRenderer
{
public:
    void render(const CameraObject& camera, const std::vector<Line>& lines);
    bool Initialize(const std::shared_ptr<D3D11RenderView>& view);

protected:
    std::shared_ptr<D3D11RenderView> _view;
};

