#pragma once

#include <cstddef>

enum class ObjectAttribute {
    Static,
    Dynamic
};

class ERTREObject
{
public:
    ERTREObject();

    std::size_t GetID() const { return _uid; }
    void setAttribute(ObjectAttribute att) {
        _attribute = att;
    }
    ObjectAttribute GetAttribute() {
        return _attribute;
    }

protected:
    ObjectAttribute _attribute = ObjectAttribute::Static;
    std::size_t _uid;
};

