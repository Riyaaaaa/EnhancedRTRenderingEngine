#include "stdafx.h"
#include "CubeReflectionCapture.h" 

#include "GraphicsInterface/GITexture.h"


CubeReflectionCapture::CubeReflectionCapture(float size) {
    box.h = box.w = box.z = size;
    dirty = true;
}

void CubeReflectionCapture::ReflectGraphics(GITexture* tex) {
    //tex->Initialize();
}