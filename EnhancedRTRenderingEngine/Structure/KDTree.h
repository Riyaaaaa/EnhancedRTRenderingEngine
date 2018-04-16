#pragma once

#include <memory>

#include "Constant/GlobalEnumrations.h"

struct KDNode {
    Axis axis;
    unsigned int index;
    std::shared_ptr<KDNode> left, right;
};
