#pragma once

#include <utility>
#include "ResourceEntity.h"

class RowBinary : public ResourceEntity<RowBinary> {
public:
	using ResourceEntity<RowBinary>::ResourceEntity;

	RowBinary() : _ptr(nullptr), _size(0) {}
	RowBinary(void* ptr, std::size_t size) : _ptr(ptr), _size(size) {}


	void Dispose() {
		this->_ptr = nullptr;
	}

	void Release() {
		if (_ptr) {
			delete _ptr;
			_ptr = nullptr;
		}
	}

	const void* get() const { return _ptr; }
	std::size_t size() const { return _size; }
	bool isValid() const { return _ptr != nullptr; }

	std::size_t _size;
	const void* _ptr;
};
