#include "stdafx.h"
#include "PhotonMapping.h"
#include "Common/Common.h"
#include "SpiralLibrary/Random/Random.hpp"

void PhotonMapping::Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene) {

    auto& pLights = scene->GetPointLights();

    for (auto&& pLight : pLights) {
        auto pos = pLight.GetPoint();

        for (int i = 0; i < 100; i++) {
            auto sita = libspiral::Random<>::getValue(libspiral::Range<float>{0.0f, 2 * D3DX_PI});
            auto phi = libspiral::Random<>::getValue(libspiral::Range<float>{0.0f, 2 * D3DX_PI});

            Ray ray(pos, Vector3D(std::sinf(sita) * std::cosf(phi),
                std::sinf(sita) * std::sinf(phi),
                std::cosf(sita)));

            auto result = RayTrace(factory, ray, 1);

            for (auto && seg : result) {
                rayPaths.push_back(Line(seg, Color3B{ 255, 0, 0 }, 0.1f));
            }
        }
        
    }

}
