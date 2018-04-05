#pragma once

#include <cstdint>
#include <unordered_map>

#include "Scene/MeshObject.h"
#include "SpiralLibrary/Common/Math.hpp"
#include "Structure/Octree.h"

namespace SpaceOctree {
    constexpr int MaxSpaceSeparateNums = 8;

    uint32_t BitSeparateFor3D(uint8_t n);
    uint32_t Get3DMortonOrder(uint8_t x, uint8_t y, uint8_t z);
    uint32_t Get3DMortonOrder(_Vector3D<uint8_t> coordinate);

    class OctreeFactoryBase {
    public:
        OctreeFactoryBase(AABB RootAABB, int splitLevel);

        AABB CalculateOctreeBoxAABBFromMortonNumber(uint32_t number) const;
        _Vector3D<uint8_t> CalculateGridCoordinate(const Vector3D& pos);
        int CalculateMortonNumber(const AABB& aabb);
        int CalculateMortonNumber(const Vector3D& pos, int splitLevel);
        int CalculateIndexFromPoint(const Vector3D& pos);

        virtual bool BoxExists(uint32_t index) = 0;
        virtual void AddRootBox(uint32_t index) = 0;
        virtual const std::unique_ptr<OctreeBox>& GetOctreeBox(uint32_t index) = 0;
        virtual void IterateEnableBox(std::function<void(const std::pair<uint32_t, OctreeBox*>&)> callback) const = 0;

        bool Register(IMeshObject* object)
        {
            auto aabb = object->GetAABB();
            uint32_t Elem = CalculateMortonNumber(aabb);
            if (Elem < _splitNums) {
                if (!BoxExists(Elem)) {
                    AddRootBox(Elem);
                }
                GetOctreeBox(Elem)->Push(std::make_shared<OctreeNode>(object));
                return true;
            }
            return false;
        }

        void SetBox(uint32_t Elem) {
            while (!BoxExists(Elem))
            {
                AddRootBox(Elem);
                Elem = (Elem - 1) >> 3;
                if (Elem >= _splitNums) break;
            }
        }

        Size3D GetMinBoxSize() const {
            return _minBoxSize;
        }

        AABB GetRootAABB() const {
            return _rootAABB;
        }

        int GetSplitLevel() const {
            return _splitLevel;
        }

    protected:
        uint32_t _splitNums;
        int _splitLevel;
        Size3D _minBoxSize;
        AABB _rootAABB;
    };

    class HashedOctreeFactory : public OctreeFactoryBase {
    public:
        using OctreeFactoryBase::OctreeFactoryBase;

        bool BoxExists(uint32_t index) override {
            return _tree.count(index) == 1;
        }

        void AddRootBox(uint32_t index) override {
            _tree[index] = std::make_unique<OctreeBox>();
        }

        const std::unique_ptr<OctreeBox>& GetOctreeBox(uint32_t index) override {
            return _tree[index];
        }

        void IterateEnableBox(std::function<void(const std::pair<uint32_t, OctreeBox*>&)> callback) const override {
            for (auto && pair : _tree) {
                if (!pair.second->nodes.empty()) {
                    callback(std::make_pair(pair.first, pair.second.get()));
                }
            }
        }

        std::unordered_map<uint32_t, std::unique_ptr<OctreeBox>>& GetTree() {
            return _tree;
        }
        const std::unordered_map<uint32_t, std::unique_ptr<OctreeBox>>& GetTree() const {
            return _tree;
        }
    protected:
        std::unordered_map<uint32_t, std::unique_ptr<OctreeBox>>::const_iterator _it;
        std::unordered_map<uint32_t, std::unique_ptr<OctreeBox>> _tree;
    };

    class LinerOctreeFactory : public OctreeFactoryBase {
    public:
        LinerOctreeFactory(AABB RootAABB, int splitLevel) : 
            OctreeFactoryBase(RootAABB, splitLevel)
        {
            _tree.resize(_splitNums);
        }

        bool BoxExists(uint32_t index) override {
            return static_cast<bool>(_tree[index]);
        }

        void AddRootBox(uint32_t index) override {
            _tree[index] = std::make_unique<OctreeBox>();
        }

        const std::unique_ptr<OctreeBox>& GetOctreeBox(uint32_t index) override {
            return _tree[index];
        }

        void IterateEnableBox(std::function<void(const std::pair<uint32_t, OctreeBox*>&)> callback) const override {
            for (int i = 0; i < GetTree().size(); i++) {
                auto& box = GetTree()[i];
                if (box && !box->nodes.empty()) {
                    callback(std::make_pair(i, box.get()));
                }
            }
        }

        std::vector<std::unique_ptr<OctreeBox>>& GetTree() {
            return _tree;
        }

        const std::vector<std::unique_ptr<OctreeBox>>& GetTree() const {
            return _tree;
        }

    protected:
        std::vector<std::unique_ptr<OctreeBox>> _tree;
    };
}