#include "stdafx.h"
#include "CollectionPhotonVolume.h"

bool CollectionPhotonVolume::Contains(Vector3D pos) {
    auto& mpos = _transform.location;
    return pos.x >= mpos.x - box.w / 2 && pos.x <= mpos.x + box.w / 2 &&
        pos.y >= mpos.y - box.h / 2 && pos.y <= mpos.y + box.h / 2 &&
        pos.z >= mpos.z - box.d / 2 && pos.z <= mpos.z + box.d / 2;
}
