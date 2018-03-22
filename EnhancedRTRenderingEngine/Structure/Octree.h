#pragma once

#include "Scene/MeshObject.h"

struct OctreeBox;

struct OctreeNode {
    OctreeNode(IMeshObject* _object) :
    object(_object),
        next(nullptr),
        prev(nullptr)
    {}
    IMeshObject* object;
    std::shared_ptr<OctreeNode> next, prev;
    OctreeBox* belongs;
};

struct OctreeBox {
    std::shared_ptr<OctreeNode> head = nullptr;
    void Push(const std::shared_ptr<OctreeNode>& node) {
        if (node->belongs) {
            return;
        }

        if (!head) {
            head = node;
        }
        else {
            head->next = node;
            head = node;
        }
        node->belongs = this;
    }
};

