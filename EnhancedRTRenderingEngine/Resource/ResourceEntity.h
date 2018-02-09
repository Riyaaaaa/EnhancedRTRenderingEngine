#pragma once

struct ResourceHandable {};

template<class ResourceType>
class ResourceEntity {
public:
    typedef ResourceHandable ResourceAttribute;

    void Dispose() { static_cast<ResourceType&>(this)->Dispose(); }
    void Release() { static_cast<ResourceType&>(this)->Release(); }
};
