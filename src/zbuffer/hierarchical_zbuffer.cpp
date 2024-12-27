#include <zbuffer/hierarchical_zbuffer.h>

HierarchicalZBuffer::HierarchicalZBuffer(int width, int height) : ZBuffer(width, height) {
    m_z_buffer.resize(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index         = get_index(y, x);
            m_z_buffer[index] = std::make_shared<QuadTree>(int2(x, y), int2(x, y), static_cast<float>(M_MAX_FLOAT));
        }
    }
    m_z_pyramid = build_pyramid(int2(0, 0), int2(m_width - 1, m_height - 1));
}

std::shared_ptr<QuadTree> HierarchicalZBuffer::build_pyramid(int2 min, int2 max) {
    if (min.x == max.x && min.y == max.y) {
        return m_z_buffer[get_index(min.y, min.x)];
    }

    auto node = std::make_shared<QuadTree>(min, max, static_cast<float>(M_MAX_FLOAT));

    int middle_x = (min.x + max.x) / 2;
    int middle_y = (min.y + max.y) / 2;
    auto child   = build_pyramid(int2(min.x, min.y), int2(middle_x, middle_y));
    node->m_children.emplace_back(child);
    child->m_parent = node;
    if (middle_x + 1 <= max.x) {
        child = build_pyramid(int2(middle_x + 1, min.y), int2(max.x, middle_y));
        node->m_children.emplace_back(child);
        child->m_parent = node;
    }
    if (middle_y + 1 <= max.y) {
        child = build_pyramid(int2(min.x, middle_y + 1), int2(middle_x, max.y));
        node->m_children.emplace_back(child);
        child->m_parent = node;
    }
    if (middle_x + 1 <= max.x && middle_y + 1 <= max.y) {
        child = build_pyramid(int2(middle_x + 1, middle_y + 1), int2(max.x, max.y));
        node->m_children.emplace_back(child);
        child->m_parent = node;
    }
    return node;
}

HierarchicalZBuffer::~HierarchicalZBuffer() = default;

void HierarchicalZBuffer::apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) {
    for (int tri_id = 0; tri_id < model->faces.size(); tri_id++) {
        Fragment fragment(float3(model->vertices[model->faces[tri_id].x]),
                          float3(model->vertices[model->faces[tri_id].y]),
                          float3(model->vertices[model->faces[tri_id].z]), tri_id);
        pyramid_test(fragment, m_z_pyramid, model, gbuffer);
    }
}

void HierarchicalZBuffer::pyramid_test(const Fragment &fragment, const std::shared_ptr<QuadTree> &node,
                                       const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) {
    if (fragment.min_z < node->m_value) { // Continue testing
        bool recursive_test = false;
        int idx             = 0;
        for (idx = 0; idx < node->m_children.size(); idx++) {
            if (fragment.min_x >= static_cast<float>(node->m_children[idx]->m_min.x) &&
                fragment.min_y >= static_cast<float>(node->m_children[idx]->m_min.y) &&
                fragment.max_x <= static_cast<float>(node->m_children[idx]->m_max.x + 1) &&
                fragment.max_y <= static_cast<float>(node->m_children[idx]->m_max.y + 1)) {
                recursive_test = true;
                break;
            }
        }
        if (recursive_test) {
            pyramid_test(fragment, node->m_children[idx], model, gbuffer);
            // Update current node z value
            float max_z = -M_MAX_FLOAT;
            for (auto &child : node->m_children) {
                max_z = std::max(max_z, child->m_value);
            }
            node->m_value = max_z;
        } else {
            node_test(fragment, node, model, gbuffer);
        }
    }
}

float HierarchicalZBuffer::node_test(const Fragment &fragment, const std::shared_ptr<QuadTree> &node,
                                     const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) {
    if (static_cast<float>(node->m_max.x + 1) < fragment.min_x || static_cast<float>(node->m_min.x) > fragment.max_x ||
        static_cast<float>(node->m_max.y + 1) < fragment.min_y || static_cast<float>(node->m_min.y) > fragment.max_y) {
        return node->m_value;
    }

    if (node->m_min.x == node->m_max.x && node->m_min.y == node->m_max.y) {
        auto pixel      = float2(static_cast<float>(node->m_min.x) + 0.5f, static_cast<float>(node->m_min.y) + 0.5f);
        auto project_p0 = float2(fragment.p0.x, fragment.p0.y);
        auto project_p1 = float2(fragment.p1.x, fragment.p1.y);
        auto project_p2 = float2(fragment.p2.x, fragment.p2.y);
        auto edge1      = project_p0 - project_p1;
        auto edge2      = project_p1 - project_p2;
        auto edge3      = project_p2 - project_p0;
        auto area       = std::abs(edge1.cross(edge2)) / 2.0f;
        auto gamma      = edge1.cross(pixel - project_p1) / 2.0f;
        auto alpha      = edge2.cross(pixel - project_p2) / 2.0f;
        auto beta       = edge3.cross(pixel - project_p0) / 2.0f;
        if (alpha * beta > 0.0f && alpha * gamma > 0.0f) {
            alpha     = std::abs(alpha) / area;
            beta      = std::abs(beta) / area;
            gamma     = 1 - alpha - beta;
            int index = get_index(node->m_min.y, node->m_min.x);
            float z   = alpha * fragment.p0.z + beta * fragment.p1.z + gamma * fragment.p2.z;
            if (z < m_z_buffer[index]->m_value) {
                int tri_id                           = fragment.tri_id;
                m_z_buffer[index]->m_value           = z;
                gbuffer->m_depth_buffer[index]       = z;
                gbuffer->m_barycentric_buffer[index] = std::make_pair(alpha, beta);
                gbuffer->m_normal_buffer[index]      = model->normals[model->faces[tri_id].x] * alpha +
                                                  model->normals[model->faces[tri_id].y] * beta +
                                                  model->normals[model->faces[tri_id].z] * gamma;
                gbuffer->m_triangle_id_buffer[index] = tri_id;
            }
            return z;
        }
        return node->m_value;
    }

    float max_z = -M_MAX_FLOAT;
    for (const auto &child : node->m_children) {
        float temp_z = node_test(fragment, child, model, gbuffer);
        max_z        = std::max(max_z, temp_z);
    }
    node->m_value = max_z;
    return node->m_value;
}

int HierarchicalZBuffer::get_index(int row, int col) const { return row * m_width + col; }
