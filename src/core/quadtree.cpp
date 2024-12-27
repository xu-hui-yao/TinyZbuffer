#include <core/quadtree.h>

Fragment::Fragment(float3 p0_, float3 p1_, float3 p2_, int tri_id_) : p0(p0_), p1(p1_), p2(p2_), tri_id(tri_id_) {
    min_z = std::min({ p0.z, p1.z, p2.z });
    min_x = std::min({ p0.x, p1.x, p2.x });
    max_x = std::max({ p0.x, p1.x, p2.x });
    min_y = std::min({ p0.y, p1.y, p2.y });
    max_y = std::max({ p0.y, p1.y, p2.y });
}