#pragma once

#include <core/common.h>
#include <vector>

class GBuffer {
public:
    GBuffer(int height, int width);

    [[nodiscard]] int index(int row, int col) const;

    std::vector<float> m_depth_buffer;
    std::vector<int> m_triangle_id_buffer;
    std::vector<std::pair<float, float>> m_barycentric_buffer;
    std::vector<float3> m_normal_buffer;
    std::vector<float3> m_color_buffer;
    int m_height, m_width;
};
