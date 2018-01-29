#pragma once

#include "d3d11.h"

#include <utility>

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

class ResourceEntity {
public:
	ResourceEntity() : _ptr(nullptr), _size(0) {}
	ResourceEntity(void* ptr, SIZE_T size) : _ptr(ptr), _size(size) {}
	ResourceEntity& operator=(ResourceEntity&& other) noexcept {
		if (this != &other) {
			this->_ptr = other.get();
			this->_size = other.size();

			other._ptr = nullptr;
		}
		return *this;
	}

	ResourceEntity(ResourceEntity&& other) noexcept {
		*this = std::move(other); 
	}

	virtual ~ResourceEntity() {};

	const void* get() const { return _ptr; }
	SIZE_T size() const { return _size; }
	bool isValid() const { return _ptr != nullptr; }

	void release() {
		if (_ptr) {
			delete _ptr;
			_ptr = nullptr;
		}
	}

	ResourceEntity & operator=(const ResourceEntity& src) = default;
	ResourceEntity(const ResourceEntity&) = default;

	SIZE_T _size;
	const void* _ptr;
};

template<class ResourceType>
class ResourceHandleBase {
protected:
	ResourceType _resource;

public:
	ResourceHandleBase() {}
	ResourceHandleBase(const ResourceType& resource) : _resource(resource) {}
	ResourceType& operator()() { return _resource; }
	const ResourceType& operator()() const { return _resource; }

	virtual ~ResourceHandleBase() {};
};

template<class ResourceType = ResourceEntity, bool isResource = std::is_base_of<ResourceEntity, ResourceType>::value>
class ResourceHandle : public ResourceHandleBase<ResourceType> {
public:
	ResourceHandle() {}
	ResourceHandle(const ResourceType& resource) : ResourceHandleBase<ResourceType>(resource), _isOwner(true) {}
	ResourceHandle(const ResourceHandle& src) {
		this->_resource = src._resource;
		this->_isOwner = false;
	}

	ResourceHandle& operator=(const ResourceHandle&) = default;

	ResourceHandle& operator=(ResourceHandle&& other) noexcept {
		if (this != &other) {
			this->_isOwner = other._isOwner;
			this->_resource = std::move(other._resource);
		}
		return *this;
	}

	ResourceHandle(ResourceHandle&& other) noexcept {
		*this = std::move(other);
	}

	virtual ~ResourceHandle() override {
		if (_isOwner) {
			this->_resource.release();
		}
	}

private:
	bool _isOwner;
};

template<class ResourceType>
class ResourceHandle<ResourceType, false>;
//
//class ResourceSharedHandle : public ResourceEntity {
//	using ResourceEntity::ResourceEntity;
//
//public:
//	SharedRefCounter<std::remove_pointer_t<decltype(_ptr)>> counter;
//	ResourceSharedHandle(void* ptr, SIZE_T size) : counter(ptr), ResourceEntity(ptr, size) {}
//
//	virtual ~ResourceSharedHandle() override {}
//};
