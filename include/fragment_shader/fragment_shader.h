#pragma once

#include <core/common.h>
#include <core/gbuffer.h>
#include <memory>

enum Pattern {
    ENormal,
    EDepth,
    ETriangleIndex,
    EBlinnPhong
};

class FragmentShader {
public:
    explicit FragmentShader(Pattern pattern, const matrix4 &transform_matrix, const float3 &view_direction);

    void set_blinn_phong_params(float3 light_position, float3 light_color, float3 ambient_color);

    void apply(const std::shared_ptr<GBuffer>& gbuffer) const;

private:
    Pattern m_pattern;
    float3 m_light_position;
    float3 m_light_color;
    float3 m_ambient_color;
    matrix4 m_transform_matrix;
    matrix4 m_inv_transform_matrix;
    float3 m_view_direction;
};