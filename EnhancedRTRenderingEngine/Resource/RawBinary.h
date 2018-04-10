#pragma once

#include <utility>
#include "ResourceEntity.h"

class RawBinary : public ResourceEntity<RawBinary> {
public:
    using ResourceEntity<RawBinary>::ResourceEntity;

    RawBinary() : _ptr(nullptr), _size(0) {}
    RawBinary(const void* ptr, std::size_t size) : _ptr(ptr), _size(size) {}


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
    void* mutableGet() { return const_cast<void*>(_ptr); }
    std::size_t size() const { return _size; }
    bool isValid() const { return _ptr != nullptr; }

    std::size_t _size;
    const void* _ptr;
};
