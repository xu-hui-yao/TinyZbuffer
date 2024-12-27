#include <zbuffer/naive_zbuffer.h>

NaiveZBuffer::NaiveZBuffer(int width, int height) : ZBuffer(width, height) {}

NaiveZBuffer::~NaiveZBuffer() = default;

void NaiveZBuffer::apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) {
    for (int tri_id = 0; tri_id < model->faces.size(); tri_id++) {
        float4 p0 = model->vertices[model->faces[tri_id].x];
        float4 p1 = model->vertices[model->faces[tri_id].y];
        float4 p2 = model->vertices[model->faces[tri_id].z];

        int min_x = std::max(static_cast<int>(std::floor(std::min({ p0.x, p1.x, p2.x }))), 0);
        int max_x = std::min(static_cast<int>(std::ceil(std::max({ p0.x, p1.x, p2.x }))), m_width - 1);
        int min_y = std::max(static_cast<int>(std::floor(std::min({ p0.y, p1.y, p2.y }))), 0);
        int max_y = std::min(static_cast<int>(std::ceil(std::max({ p0.y, p1.y, p2.y }))), m_height - 1);

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                auto pixel      = float2(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
                auto project_p0 = float2(p0.x, p0.y);
                auto project_p1 = float2(p1.x, p1.y);
                auto project_p2 = float2(p2.x, p2.y);
                auto edge1      = project_p0 - project_p1;
                auto edge2      = project_p1 - project_p2;
                auto edge3      = project_p2 - project_p0;

                auto area  = std::abs(edge1.cross(edge2)) / 2.0f;
                auto gamma = edge1.cross(pixel - project_p1) / 2.0f;
                auto alpha = edge2.cross(pixel - project_p2) / 2.0f;
                auto beta  = edge3.cross(pixel - project_p0) / 2.0f;

                if (alpha * beta > 0.0f && alpha * gamma > 0.0f) {
                    alpha       = std::abs(alpha) / area;
                    beta        = std::abs(beta) / area;
                    gamma       = 1 - alpha - beta;
                    float depth = alpha * p0.z + beta * p1.z + gamma * p2.z;
                    int idx     = gbuffer->index(y, x);

                    if (depth < gbuffer->m_depth_buffer[idx]) {
                        gbuffer->m_depth_buffer[idx]       = depth;
                        gbuffer->m_triangle_id_buffer[idx] = tri_id;
                        gbuffer->m_normal_buffer[idx]      = model->normals[model->faces[tri_id].x] * alpha +
                                                        model->normals[model->faces[tri_id].y] * beta +
                                                        model->normals[model->faces[tri_id].z] * gamma;
                    }
                }
            }
        }
    }
}