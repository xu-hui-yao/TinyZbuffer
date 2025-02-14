#include <fragment_shader/fragment_shader.h>
#include <random>

FragmentShader::FragmentShader(Pattern pattern, const matrix4 &transform_matrix, const float3 &view_direction)
    : m_pattern(pattern), m_transform_matrix(transform_matrix), m_inv_transform_matrix(transform_matrix.inverse()),
      m_view_direction(view_direction) {}

void FragmentShader::set_blinn_phong_params(float3 light_position, float3 light_color, float3 ambient_color) {
    m_light_position = light_position;
    m_light_color    = light_color;
    m_ambient_color  = ambient_color;
}

void FragmentShader::apply(const std::shared_ptr<GBuffer> &gbuffer) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(0.0f, 1.0f);
    float max_depth = -M_MAX_FLOAT;
    float min_depth = M_MAX_FLOAT;

    switch (m_pattern) {
        case ENormal:
            for (int i = 0; i < gbuffer->m_height; i++) {
                for (int j = 0; j < gbuffer->m_width; j++) {
                    int pixel_index = gbuffer->index(i, j);
                    int tri_id      = gbuffer->m_triangle_id_buffer[pixel_index];
                    if (tri_id >= 0) {
                        gbuffer->m_color_buffer[pixel_index] = (gbuffer->m_normal_buffer[pixel_index] + 1.0f) / 2.0f;
                    } else {
                        gbuffer->m_color_buffer[pixel_index] = float3(0.0f, 0.0f, 0.0f);
                    }
                }
            }
            break;

        case EDepth:
            for (int i = 0; i < gbuffer->m_height; i++) {
                for (int j = 0; j < gbuffer->m_width; j++) {
                    int pixel_index = gbuffer->index(i, j);
                    int tri_id      = gbuffer->m_triangle_id_buffer[pixel_index];
                    if (tri_id >= 0) {
                        float depth                          = gbuffer->m_depth_buffer[pixel_index];
                        gbuffer->m_color_buffer[pixel_index] = float3(depth, depth, depth);
                        if (depth > max_depth) {
                            max_depth = depth;
                        }
                        if (depth < min_depth) {
                            min_depth = depth;
                        }
                    } else {
                        gbuffer->m_color_buffer[pixel_index] = float3(1.0f, 1.0f, 1.0f);
                    }
                }
            }
            for (int i = 0; i < gbuffer->m_height; i++) {
                for (int j = 0; j < gbuffer->m_width; j++) {
                    int pixel_index = gbuffer->index(i, j);
                    if (gbuffer->m_color_buffer[pixel_index].x != 1.0f) {
                        gbuffer->m_color_buffer[pixel_index] -= min_depth;
                        gbuffer->m_color_buffer[pixel_index] /= max_depth - min_depth;
                    }
                }
            }
            break;

        case ETriangleIndex:
            for (int i = 0; i < gbuffer->m_height; i++) {
                for (int j = 0; j < gbuffer->m_width; j++) {
                    int pixel_index = gbuffer->index(i, j);
                    int tri_id      = gbuffer->m_triangle_id_buffer[pixel_index];
                    if (tri_id >= 0) {
                        gen.seed(tri_id);
                        float r                              = dis(gen);
                        float g                              = dis(gen);
                        float b                              = dis(gen);
                        gbuffer->m_color_buffer[pixel_index] = float3(r, g, b);
                    } else {
                        gbuffer->m_color_buffer[pixel_index] = float3(0.0f, 0.0f, 0.0f);
                    }
                }
            }
            break;

        case EBlinnPhong:
            for (int i = 0; i < gbuffer->m_height; i++) {
                for (int j = 0; j < gbuffer->m_width; j++) {
                    int pixel_index = gbuffer->index(i, j);
                    int tri_id      = gbuffer->m_triangle_id_buffer[pixel_index];
                    if (tri_id >= 0) {
                        float3 normal = gbuffer->m_normal_buffer[pixel_index];
                        float4 origin_point =
                            m_inv_transform_matrix * float4(static_cast<float>(j), static_cast<float>(i),
                                                            gbuffer->m_depth_buffer[pixel_index], 1.0f);
                        origin_point /= origin_point.w;
                        float3 light_dir =
                            (m_light_position - float3(origin_point.x, origin_point.y, origin_point.z)).normalize();

                        float diffuse = std::max(normal.dot(light_dir), 0.0f);

                        float3 half_dir = (light_dir + m_view_direction).normalize();
                        float specular  = std::pow(std::max(normal.dot(half_dir), 0.0f), 32.0f);

                        float3 ambient        = m_ambient_color;
                        float3 diffuse_color  = m_light_color * diffuse;
                        float3 specular_color = m_light_color * specular;

                        gbuffer->m_color_buffer[pixel_index] = ambient + diffuse_color + specular_color;
                    } else {
                        gbuffer->m_color_buffer[pixel_index] = float3(0.0f, 0.0f, 0.0f);
                    }
                }
            }
            break;

        default:
            std::cerr << "Unsupported pattern: " << m_pattern << std::endl;
            exit(-1);
    }
}