#pragma once

#include "d3d11.h"

#include <utility>

class ResourceHandle
{
public:
	ResourceHandle() : _ptr(nullptr), _size(0) {}
	ResourceHandle(void* ptr, SIZE_T size) : _ptr(ptr), _size(size) {}
	ResourceHandle(const ResourceHandle&) = delete; // Noncopyable
	ResourceHandle& operator=(ResourceHandle&& other) noexcept {
		if (this != &other) {
			this->_ptr = other.get();
			this->_size = other.size();

			other._ptr = nullptr;
		}
		return *this;
	}
	ResourceHandle(ResourceHandle&& other) noexcept { 
		*this = std::move(other); 
	}

	virtual ~ResourceHandle() {
		if (_ptr) {
			delete _ptr;
		}
	}

	const void* get() const { return _ptr; }
	SIZE_T size() const { return _size; }
	bool isValid() const { return _ptr != nullptr; }

protected:
	SIZE_T _size;
	const void* _ptr;
};

