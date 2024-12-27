#pragma once

#include <core/common.h>

struct Fragment {
  float3 p0, p1, p2;
  float min_z, min_x, max_x, min_y, max_y;
  int tri_id;

  Fragment(float3 p0_, float3 p1_, float3 p2_, int tri_id_);
};

template <typename T, typename Index> class TQuadTree {
public:
    static_assert(std::is_same_v<Index, int2> || std::is_same_v<Index, float2>,
                  "Error, Index of QuadTree must be int2 or float2");

    TQuadTree(Index min, Index max, T value)
        : m_min(min), m_max(max), m_value(value) {}

    Index m_min, m_max;
    T m_value;
    std::shared_ptr<TQuadTree> m_parent;
    std::vector<std::shared_ptr<TQuadTree>> m_children;
};
