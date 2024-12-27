#include <core/gbuffer.h>

GBuffer::GBuffer(int height, int width) : m_height(height), m_width(width) {
    m_depth_buffer.resize(width * height);
    m_triangle_id_buffer.resize(width * height);
    m_barycentric_buffer.resize(width * height);
    m_normal_buffer.resize(width * height);
    m_color_buffer.resize(width * height);
    std::fill(m_depth_buffer.begin(), m_depth_buffer.end(), static_cast<float>(M_MAX_FLOAT));
    std::fill(m_triangle_id_buffer.begin(), m_triangle_id_buffer.end(), -1);
    std::fill(m_barycentric_buffer.begin(), m_barycentric_buffer.end(), std::pair(0.0f, 0.0f));
    std::fill(m_normal_buffer.begin(), m_normal_buffer.end(), float3(0, 0, 0));
    std::fill(m_color_buffer.begin(), m_color_buffer.end(), float3(0, 0, 0));
}

int GBuffer::index(int row, int col) const { return row * m_width + col; }
