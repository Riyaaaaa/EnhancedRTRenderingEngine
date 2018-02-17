#pragma once

#include <utility>

#include "RawBinary.h"
#include "SpiralLibrary/Optional/Optional.hpp"

class RefCounter {
private:
    int refCount;

public:
    RefCounter()
        : refCount(1)
    {}

    virtual ~RefCounter()
    {}

public:
    void addref()
    {
        refCount++;
    }

    void release()
    {
        if (--refCount == 0)
        {
            dispose();
            delete this;
        }
    }

public:
    virtual void dispose() = 0;
};

template < typename T >
class RefCounterImpl
    : public RefCounter
{
private:
    T * ptr;

public:
    RefCounterImpl(T* ptr)
        : ptr(ptr)
    {}

public:
    void dispose()
    {
        delete ptr;
    }
};

template<class T>
class SharedRefCounter {
    RefCounter* counter;

public:
    SharedRefCounter(T* ptr) : counter(new RefCounterImpl<void>(ptr)) {}
    ~SharedRefCounter() {
        if (counter) {
            counter->release();
        }
    }

    SharedRefCounter(const SharedRefCounter& src) : counter(src.counter){
        if (counter) {
            counter->addref();
        }
    }

    SharedRefCounter& operator=(const SharedRefCounter& src)
    {
        auto temp = src.counter;
        if (temp != counter) {
            if (temp) {
                temp->addref();
            }
            if (counter) {
                counter->release();
            }
            counter = temp;
        }
        return *this;
    }
};

template<class ResourceType>
class ResourceHandleBase;

template<class ResourceType>
class ResourceHandleBase {
protected:
    libspiral::Optional<ResourceType> _resource;

public:
    ResourceHandleBase() {}
    ResourceHandleBase(const ResourceType& resource) : _resource(resource) {}
    ResourceType& operator()() { return *_resource; }
    const ResourceType& operator()() const { return *_resource; }

    virtual ~ResourceHandleBase() {};
};

//template<class ResourceType = RawBinary, typename Attribute = typename ResourceType::ResourceAttribute>
//class ResourceHandle;

template<class ResourceType = RawBinary, bool Attribute = std::is_move_constructible_v<ResourceType> && std::is_move_assignable_v<ResourceType>>
class ResourceHandle;

template<class ResourceType>
class ResourceHandle<ResourceType, true> : public ResourceHandleBase<ResourceType> {
public:
    int _uid;
    ResourceHandle() {}
    ResourceHandle(const ResourceType& resource) : ResourceHandleBase<ResourceType>(resource), _isOwner(true) {}
    ResourceHandle(const ResourceHandle& src) {
        this->_resource = src._resource;
        this->_isOwner = false;
    }

    ResourceHandle& operator=(const ResourceHandle& rhs) {
        if (this != &rhs) {
            this->_resource = rhs._resource;
            this->_isOwner = false;
        }
        return *this;
    }

    ResourceHandle& operator=(ResourceHandle&& other) noexcept {
        if (this != &other) {
            this->_isOwner = other._isOwner;
            this->_resource = std::move(other._resource);

            other._resource.reset();
            other._isOwner = false;
        }
        return *this;
    }

    ResourceHandle(ResourceHandle&& other) noexcept {
        *this = std::move(other);
    }

    bool HasResource() const {
        return static_cast<bool> (this->_resource);
    }

    virtual ~ResourceHandle() override {
        if (_isOwner && this->_resource) {
            this->_resource->Release();
            this->_resource.reset();
        }
    }

private:
    bool _isOwner;
};

//
//class ResourceSharedHandle : public ResourceEntity {
//    using ResourceEntity::ResourceEntity;
//
//public:
//    SharedRefCounter<std::remove_pointer_t<decltype(_ptr)>> counter;
//    ResourceSharedHandle(void* ptr, SIZE_T size) : counter(ptr), ResourceEntity(ptr, size) {}
//
//    virtual ~ResourceSharedHandle() override {}
//};
