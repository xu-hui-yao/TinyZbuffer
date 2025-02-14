#include <vertex_shader/vertex_shader.h>
#include <algorithm>

VertexShader::VertexShader(const matrix4 &view_matrix, const matrix4 &perspective_matrix, const matrix4 &screen_matrix,
                           int width, int height)
    : m_view_matrix(view_matrix), m_perspective_matrix(perspective_matrix), m_screen_matrix(screen_matrix),
      m_width(width), m_height(height) {
    m_transform_matrix = m_screen_matrix * m_perspective_matrix * m_view_matrix;
}

void VertexShader::apply(const std::shared_ptr<Model> &model) const {
    for (auto &vertex : model->vertices) {
        vertex = m_transform_matrix * vertex;
        vertex /= vertex.w;
    }

    // Cull triangles
    auto is_outside_screen = [&](const int3 &face) {
        for (int i = 0; i < 3; i++) {
            const float4 &vertex = model->vertices[face[i]];
            if (vertex.x < 0.0f || vertex.x >= static_cast<float>(m_width) || vertex.y < 0.0f ||
                vertex.y >= static_cast<float>(m_height)) {
                return true; // Should be removed
            }
        }
        return false; // Inside screen
    };
    model->faces.erase(std::remove_if(model->faces.begin(), model->faces.end(), is_outside_screen),
    model->faces.end());
}

matrix4 VertexShader::get_transform_matrix() const { return m_transform_matrix; }
