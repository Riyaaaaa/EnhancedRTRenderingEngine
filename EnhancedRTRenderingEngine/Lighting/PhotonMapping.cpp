#include "stdafx.h"
#include "PhotonMapping.h"
#include "Common/Common.h"
#include "SpiralLibrary/Random/Random.hpp"
#include "Mesh/MeshExpander.h"
#include "Utility/MathUtils.h"

#include "Common/Defines.h"

#ifdef SUPPORT_MULTI_PROCESS_OMP
#include <omp.h>
#endif

using namespace libspiral;

void PhotonMapping::Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene) {
    EmmitPhotons(factory, scene);
}

void PhotonMapping::EmmitPhotons(SpaceOctree::OctreeFactoryBase* factory, Scene* scene) {
    auto& pLights = scene->GetPointLights();
    std::vector<Photon> photon_caches;

    for (auto&& pLight : pLights) {
        auto pos = pLight.GetPoint();
        auto intensity = pLight.Intensity();

        constexpr int nEmitPhotons = 1000;

        float flux_ = intensity * 4 * D3DX_PI / nEmitPhotons;
        Vector3D flux(flux_, flux_, flux_);
#ifdef SUPPORT_MULTI_PROCESS_OMP
#pragma omp parallel for
#endif
        for (int i = 0; i < nEmitPhotons; i++) {
            auto sita = Random<>::getValue(Range<float>{0.0f, 2 * D3DX_PI});
            auto phi = Random<>::getValue(Range<float>{0.0f, 2 * D3DX_PI});

            Ray ray(pos, Vector3D(std::sinf(sita) * std::cosf(phi),
                std::sinf(sita) * std::sinf(phi),
                std::cosf(sita)));

            bool reach_diffuse_surface = false;

            Vector3D now_flux = flux;

            auto result = RayTraceIf(factory, ray, [&](const Material& mat, int trace_count) {
                float rand = Random<>::getValue(Range<float>{0.0f, 1.0f});
                // todo: support texture surface
                float prob = (mat.baseColor.x + mat.baseColor.y + mat.baseColor.z) / 3.0f;
                if (rand > prob && trace_count > 1) {
                    reach_diffuse_surface = true;
                    return false;
                }

                if (trace_count < 3) {
                    // todo: support texture surface
                    now_flux = MathUtils::Multiply(now_flux, mat.baseColor) / prob;
                    return true;
                }

                if (mat.metallic < 1.0f) {
                    // If surface is not perfect specular, cache photon
                    reach_diffuse_surface = true;
                }

                return false;
            });

            if (!result.empty() && reach_diffuse_surface) {
                photon_caches.push_back(Photon(result.back().epos,
                    now_flux,
                    result.back().epos - result.back().bpos));
            }

            for (auto && seg : result) {
                rayPaths.push_back(Line(seg, Color3B{ 255, 0, 0 }, 0.1f));
            }
        }
    }

    kdtree.build(photon_caches);
    photons.swap(photon_caches);
}
