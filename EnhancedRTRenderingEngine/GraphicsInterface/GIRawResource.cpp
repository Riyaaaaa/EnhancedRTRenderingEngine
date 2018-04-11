#include "stdafx.h"
#include "GIRawResource.h"

GIRawResource::GIRawResource(const RawBinary& res, ResourceType type, unsigned int stride) :
_resource(res),
_type(type),
_structureByteStride(stride) {

}

GITextureResource::GITextureResource(const Texture2D& tex) :
    _type(ResourceType::Texture2D) {
    _structureByteStride = sizeof(float);
    _resource.push_back(tex);
}

GITextureResource::GITextureResource(const std::vector<Texture2D>& tex, ResourceType type) :
    _type(type) ,
    _resource(tex) {
    _structureByteStride = sizeof(float);
}
