#pragma once

#include "GIResource.h"
#include "GIImmediateCommands.h"

class GIRenderView
{
public:
    void Initialize(GIImmediateCommands* cmd, const ViewportParam& param);
    void SetViewPortSize(GIImmediateCommands* cmd, Size size);
    void SetRasterizerState(GIImmediateCommands* cmd, RasterizerState state);

    Size GetRenderSize() const { return _renderSize; }
    MSAAQualityType GetMSAAQualityType() const { return _type; }

    void Present();
    
    const std::shared_ptr<GIOMResource>& GetOMResource() const {
        return _OMResource;
    }

    const ViewportCfg& GetViewPortCfg() const {
        return _viewportCfg;
    }

protected:
    MSAAQualityType _type;
    ViewportCfg _viewportCfg;
    Size _renderSize;

    std::shared_ptr<GISwapChain> _swapchain;
    std::shared_ptr<GIOMResource> _OMResource;
    std::shared_ptr<GIRasterizerState> _defaultRState, _doubleSidedRState, _wireframeRState;
};

