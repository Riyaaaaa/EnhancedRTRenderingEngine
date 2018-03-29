#pragma once

#include "GIResource.h"
#include "GIImmediateCommands.h"

class GIRenderView
{
public:
    void SetViewPortSize(GIImmediateCommands* cmd, Size size);
    void SetRasterizerState(GIImmediateCommands* cmd, RasterizerState state);

    Size GetRenderSize() const { return _renderSize; }
    MSAAQualityType GetMSAAQualityType() const { return _type; }
    
protected:
    MSAAQualityType _type;
    Size _renderSize;

    std::shared_ptr<GIOMResource> _OMResource;
    std::shared_ptr<GIRasterizerState> _defaultRState, _doubleSidedRState, _wireframeRState;
};

