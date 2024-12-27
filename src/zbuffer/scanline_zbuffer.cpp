#include <algorithm>
#include <zbuffer/scanline_zbuffer.h>

ScanlineZBuffer::ScanlineZBuffer(int width, int height) : ZBuffer(width, height) {
    m_classified_edge_table.resize(height);
    m_classified_polygon_table.resize(height);
}

ScanlineZBuffer::~ScanlineZBuffer() = default;

int ScanlineZBuffer::above_scanline_count(float y) const {
    if (y < M_EPSILON || y >= static_cast<float>(m_height)) {
        return y < M_EPSILON ? 0 : m_height;
    }
    return static_cast<int>(std::round(y - M_EPSILON));
}

void ScanlineZBuffer::apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) {
    initialize(model);
    for (int y = m_height - 1; y >= 0; y--) {
        add_active_table(y);
        update_depth(y, gbuffer, model);
        cull_active_table();
    }
}

void ScanlineZBuffer::initialize(const std::shared_ptr<Model> &model) {
    for (int i = 0; i < model->faces.size(); i++) {
        bool has_intersection = false;
        std::vector<std::pair<int, EdgeClassify>> temp_edge_table;
        float max_y      = -M_MAX_FLOAT;
        float min_y      = M_MAX_FLOAT;
        const auto &face = model->faces[i];

        for (int j = 0; j < 3; j++) {
            float4 p1 = model->vertices[face[j]];
            float4 p2 = model->vertices[face[(j + 1) % 3]];

            if (std::abs(p1.y - p2.y) < M_EPSILON) { // Parallel to the scanline
                continue;
            }

            if (p1.y < p2.y) { // p1 has max y value
                std::swap(p1, p2);
            }

            max_y = M_MAX(max_y, p1.y);
            min_y = M_MIN(min_y, p2.y);

            EdgeClassify edge_classify;
            edge_classify.y  = p1.y;
            edge_classify.x  = p1.x;
            edge_classify.id = i;
            edge_classify.dy = above_scanline_count(p1.y) - above_scanline_count(p2.y);
            edge_classify.dx = (p2.x - p1.x) / (p1.y - p2.y);

            if (edge_classify.dy > 0) {
                // Edge which does not intersect the scanline will not be added
                temp_edge_table.emplace_back(above_scanline_count(p1.y) - 1, edge_classify);
                has_intersection = true;
            }
        }

        if (has_intersection) { // Add triangle which intersects the scanline
            PolygonClassify polygon_classify;
            polygon_classify.id = i;
            polygon_classify.dy = above_scanline_count(max_y) - above_scanline_count(min_y);
            float3 p0(model->vertices[face.x]);
            float3 p1(model->vertices[face.y]);
            float3 p2(model->vertices[face.z]);
            // Cull face which are not in the frustum manually
            // if ((max_y <= 0 && max_y >= static_cast<float>(m_height)) ||
            //     (p0.x <= 0 || p0.x >= static_cast<float>(m_width)) ||
            //     (p1.x <= 0 || p1.x >= static_cast<float>(m_width)) ||
            //     (p2.x <= 0 || p2.x >= static_cast<float>(m_width))) {
            //     continue;
            // }
            // Calculate face normals in screen space instead of world space
            float3 face_normal = (p2 - p0).cross(p1 - p0).normalize();
            polygon_classify.a = face_normal.x;
            polygon_classify.b = face_normal.y;
            polygon_classify.c = face_normal.z;
            polygon_classify.d = -(face_normal.x * p0.x + face_normal.y * p0.y + face_normal.z * p0.z);
            // Ignore those which are perpendicular to xoy plane
            if (std::abs(polygon_classify.c) > M_EPSILON) {
                for (auto &[index, edge_classify] : temp_edge_table) {
                    m_classified_edge_table[index].push_back(edge_classify);
                }
                m_classified_polygon_table[above_scanline_count(max_y) - 1].emplace_back(polygon_classify);
            }
        }
    }
}

void ScanlineZBuffer::add_active_table(int y) {
    m_active_polygon_table.insert(m_active_polygon_table.end(), m_classified_polygon_table[y].begin(),
                                  m_classified_polygon_table[y].end());

    for (const auto &polygon : m_classified_polygon_table[y]) { // Add a polygon to the active polygon table
        // Find related edge of the current polygon
        std::vector<EdgeClassify> related_edge;
        for (const auto &edge : m_classified_edge_table[y]) {
            if (edge.id == polygon.id) {
                related_edge.emplace_back(edge);
            }
        }

        // Sort edge from left to right
        std::sort(related_edge.begin(), related_edge.end(), [](const EdgeClassify &a, const EdgeClassify &b) {
            if (a.x < b.x) {
                return true;
            }
            if (a.x == b.x) {
                return a.dx < b.dx;
            }
            return false;
        });

        // Add active edge pair
        ActiveEdge active_edge;
        active_edge.id  = polygon.id;
        active_edge.dzx = -(polygon.a / polygon.c);
        active_edge.dzy = polygon.b / polygon.c;
        active_edge.xl  = related_edge[0].x +
                         (related_edge[0].y - static_cast<float>(above_scanline_count(related_edge[0].y)) + 0.5f) *
                             related_edge[0].dx;
        active_edge.dxl = related_edge[0].dx;
        active_edge.dyl = related_edge[0].dy;
        active_edge.zl  = -(polygon.d + polygon.a * active_edge.xl +
                           polygon.b * (static_cast<float>(above_scanline_count(related_edge[0].y)) - 0.5f)) /
                         polygon.c;
        active_edge.xr =
            related_edge[1].x + (related_edge[1].y - std::round(related_edge[1].y) + 0.5f) * related_edge[1].dx;
        active_edge.dxr = related_edge[1].dx;
        active_edge.dyr = related_edge[1].dy;
        m_active_edge_table.emplace_back(active_edge);
    }
}

void ScanlineZBuffer::update_depth(int y, const std::shared_ptr<GBuffer> &gbuffer,
                                   const std::shared_ptr<Model> &model) {
    for (auto &active_edge : m_active_edge_table) {
        float zx = active_edge.zl + active_edge.dzx * (std::round(active_edge.xl) + 0.5f - active_edge.xl);
        for (int x = static_cast<int>(std::round(active_edge.xl)); x < static_cast<int>(std::round(active_edge.xr));
             x++) {

            // int idx = gbuffer->index(y, x);
            // if (zx < gbuffer->m_depth_buffer[idx]) {
            //     int tri_id                         = active_edge.id;
            //     gbuffer->m_depth_buffer[idx]       = zx;
            //     gbuffer->m_triangle_id_buffer[idx] = tri_id;
            //     gbuffer->m_normal_buffer[idx]      = model->face_normals[tri_id];
            //     gbuffer->m_barycentric_buffer[idx] = std::make_pair(1.0f / 3.0f, 1.0f / 3.0f);
            // }

            auto pixel      = float2(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
            auto p0         = model->vertices[model->faces[active_edge.id].x];
            auto p1         = model->vertices[model->faces[active_edge.id].y];
            auto p2         = model->vertices[model->faces[active_edge.id].z];
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
                    gbuffer->m_triangle_id_buffer[idx] = active_edge.id;
                    gbuffer->m_normal_buffer[idx]      = model->normals[model->faces[active_edge.id].x] * alpha +
                                                    model->normals[model->faces[active_edge.id].y] * beta +
                                                    model->normals[model->faces[active_edge.id].z] * gamma;
                }
            }

            zx += active_edge.dzx;
        }

        active_edge.dyl--;
        active_edge.dyr--;
        active_edge.xl += active_edge.dxl;
        active_edge.xr += active_edge.dxr;
        active_edge.zl += active_edge.dzx * active_edge.dxl + active_edge.dzy;

        if (active_edge.dyl <= 0 || active_edge.dyr <= 0) {
            find_replace_edge(y - 1, active_edge); // Find edge of the next line, instead of current line
        }
    }
}

void ScanlineZBuffer::find_replace_edge(int y, ActiveEdge &active_edge) const {
    std::vector<EdgeClassify> related_edge;

    for (const auto &edge : m_classified_edge_table[y]) {
        if (edge.id == active_edge.id) {
            related_edge.emplace_back(edge);
        }
    }

    if (related_edge.empty()) {
        return;
    }

    if (active_edge.dyl <= 0) {
        active_edge.xl =
            related_edge[0].x + (related_edge[0].y - std::round(related_edge[0].y) + 0.5f) * related_edge[0].dx;
        active_edge.dxl = related_edge[0].dx;
        active_edge.dyl = related_edge[0].dy;
    } else if (active_edge.dyr <= 0) {
        active_edge.xr =
            related_edge[0].x + (related_edge[0].y - std::round(related_edge[0].y) + 0.5f) * related_edge[0].dx;
        active_edge.dxr = related_edge[0].dx;
        active_edge.dyr = related_edge[0].dy;
    }
}

void ScanlineZBuffer::cull_active_table() {
    for (auto &active_polygon : m_active_polygon_table) {
        active_polygon.dy--;
    }
    m_active_polygon_table.erase(std::remove_if(m_active_polygon_table.begin(), m_active_polygon_table.end(),
                                                [](const auto &active_polygon) { return active_polygon.dy <= 0; }),
                                 m_active_polygon_table.end());

    m_active_edge_table.erase(
        std::remove_if(m_active_edge_table.begin(), m_active_edge_table.end(),
                       [](const auto &active_edge) { return active_edge.dyl <= 0 && active_edge.dyr <= 0; }),
        m_active_edge_table.end());
}
