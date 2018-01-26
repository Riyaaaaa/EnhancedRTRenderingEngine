#pragma once

#include "d3d11.h"

#include <utility>

template<class Derived>
class ResourceHandleImpl
{
public:
	ResourceHandleImpl() : _ptr(nullptr), _size(0) {}
	ResourceHandleImpl(void* ptr, SIZE_T size, bool isOwner = true) : _ptr(ptr), _size(size), _isOwner(isOwner) {}
	ResourceHandleImpl& operator=(ResourceHandleImpl&& other) noexcept {
		if (this != &other) {
			this->_ptr = other.get();
			this->_size = other.size();
			this->_isOwner = other._isOwner;

			other._ptr = nullptr;
		}
		return *this;
	}
	ResourceHandleImpl(ResourceHandleImpl&& other) noexcept {
		*this = std::move(other); 
	}

	Derived CreateWeakHandle() {
		Derived weakHandle = *(dynamic_cast<Derived*>(this));
		weakHandle._isOwner = false;

		return weakHandle;
	}

	virtual ~ResourceHandleImpl() {
		if (_ptr && _isOwner) {
			delete _ptr;
		}
	}

	const void* get() const { return _ptr; }
	SIZE_T size() const { return _size; }
	bool isValid() const { return _ptr != nullptr; }

protected:
	ResourceHandleImpl(const ResourceHandleImpl&) = default;

	SIZE_T _size;
	const void* _ptr;
	bool _isOwner = true;
};

class ResourceHandle : public ResourceHandleImpl<ResourceHandle> {
	using ResourceHandleImpl::ResourceHandleImpl;
};
