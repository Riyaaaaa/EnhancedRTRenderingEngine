#include "stdafx.h"
#include "PhotonMapping.h"
#include "Common/Common.h"
#include "SpiralLibrary/Random/Random.hpp"
#include "Mesh/MeshExpander.h"

void PhotonMapping::Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene) {

    auto& pLights = scene->GetPointLights();
    std::vector<Photon> photon_caches;

    for (auto&& pLight : pLights) {
        auto pos = pLight.GetPoint();
        auto intensity = pLight.Intensity();

        constexpr int nEmitPhotons = 100;
        float flux = intensity * 4 * D3DX_PI / nEmitPhotons;

        for (int i = 0; i < nEmitPhotons; i++) {
            auto sita = libspiral::Random<>::getValue(libspiral::Range<float>{0.0f, 2 * D3DX_PI});
            auto phi = libspiral::Random<>::getValue(libspiral::Range<float>{0.0f, 2 * D3DX_PI});

            Ray ray(pos, Vector3D(std::sinf(sita) * std::cosf(phi),
                std::sinf(sita) * std::sinf(phi),
                std::cosf(sita)));

            bool reach_diffuse_surface = false;

            auto result = RayTraceIf(factory, ray, [&](const Material& mat, int trace_count) {
                float rand = libspiral::Random<>::getValue(libspiral::Range<float>{0.0f, 1.0f});
                if (rand > mat.metallic) {
                    reach_diffuse_surface = true;
                    return false;
                }

                if (trace_count < 3) {
                    return true;
                }

                return false;
            });

            if (!result.empty() && reach_diffuse_surface) {
                photon_caches.push_back(Photon(result.back().epos,
                    Vector3D(flux, flux, flux),
                    result.back().epos - result.back().bpos));
            }

            for (auto && seg : result) {
                rayPaths.push_back(Line(seg, Color3B{ 255, 0, 0 }, 0.1f));
            }
        }
    }

    kdtree.build(photon_caches);
    photons = photon_caches;
}
