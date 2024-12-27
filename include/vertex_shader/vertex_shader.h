#pragma once

#include <core/model.h>
#include <memory>

class VertexShader {
public:
    VertexShader(const matrix4 &view_matrix, const matrix4 &perspective_matrix, const matrix4 &screen_matrix, int width,
                 int height);

    void apply(const std::shared_ptr<Model> &model) const;

    [[nodiscard]] matrix4 get_transform_matrix() const;

private:
    matrix4 m_view_matrix;
    matrix4 m_perspective_matrix;
    matrix4 m_screen_matrix;
    matrix4 m_transform_matrix;
    int m_width;
    int m_height;
};
