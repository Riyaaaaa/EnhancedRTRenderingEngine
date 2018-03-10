#pragma once

#include <cstddef>

class ERTREObject
{
public:
    ERTREObject();

    std::size_t GetID() const { return _uid; }

protected:
    std::size_t _uid;
};

