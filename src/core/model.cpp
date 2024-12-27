#include <core/model.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

Model::Model(const std::string &filename, const matrix4 &model_matrix) {
    m_model_matrix  = model_matrix;
    m_normal_matrix = m_model_matrix.inverse().left_top_corner().transpose();

    typedef std::unordered_map<OBJVertex, uint32_t, OBJVertexHash> VertexMap;
    std::ifstream is(filename);
    if (!is) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    std::vector<float3> positions;
    std::vector<float3> normals;
    std::vector<uint32_t> indices;
    std::vector<OBJVertex> vertices;
    VertexMap vertex_map;

    std::string line_str;
    while (std::getline(is, line_str)) {
        std::istringstream line(line_str);

        std::string prefix;
        line >> prefix;

        if (prefix == "v") {
            float3 p;
            line >> p.x >> p.y >> p.z;
            positions.emplace_back(p);
        } else if (prefix == "vn") {
            float3 n;
            line >> n.x >> n.y >> n.z;
            normals.emplace_back(n.normalize());
        } else if (prefix == "f") {
            std::string v1, v2, v3, v4;
            line >> v1 >> v2 >> v3 >> v4;
            OBJVertex vertex[6];
            int n_vertices = 3;

            vertex[0] = OBJVertex(v1);
            vertex[1] = OBJVertex(v2);
            vertex[2] = OBJVertex(v3);

            if (!v4.empty()) {
                vertex[3]  = OBJVertex(v4);
                vertex[4]  = vertex[0];
                vertex[5]  = vertex[2];
                n_vertices = 6;
            }
            for (int i = 0; i < n_vertices; ++i) {
                const OBJVertex &v           = vertex[i];
                VertexMap::const_iterator it = vertex_map.find(v);
                if (it == vertex_map.end()) {
                    vertex_map[v] = static_cast<uint32_t>(vertices.size());
                    indices.push_back(static_cast<uint32_t>(vertices.size()));
                    vertices.push_back(v);
                } else {
                    indices.push_back(it->second);
                }
            }
        }
    }

    this->faces.resize(static_cast<long long>(indices.size() / 3));
    for (uint32_t i = 0; i < faces.size(); ++i) {
        this->faces[i] = int3(static_cast<int>(indices[i * 3]), static_cast<int>(indices[i * 3 + 1]),
                              static_cast<int>(indices[i * 3 + 2]));
    }

    this->vertices.resize(static_cast<long long>(vertices.size()));
    for (uint32_t i = 0; i < vertices.size(); ++i) {
        auto vertex = float4(float3(positions.at(vertices[i].p - 1)).x, float3(positions.at(vertices[i].p - 1)).y,
                             float3(positions.at(vertices[i].p - 1)).z, 1);
        vertex      = m_model_matrix * vertex;
        vertex /= vertex.w;
        this->vertices[i] = vertex;
        bounding_box.expand_by(float3(vertex.x, vertex.y, vertex.z));
    }

    // Calculate vertex normal
    if (!normals.empty()) {
        this->normals.resize(static_cast<long long>(vertices.size()));
        for (uint32_t i = 0; i < vertices.size(); ++i) {
            auto normal      = normals.at(vertices[i].n - 1).normalize();
            normal           = m_normal_matrix * normal;
            this->normals[i] = normal.normalize();
        }
    }

    // Calculate face normal
    face_normals.resize(faces.size());
    for (int i = 0; i < faces.size(); i++) {
        float3 p0(this->vertices[faces[i].x]);
        float3 p1(this->vertices[faces[i].y]);
        float3 p2(this->vertices[faces[i].z]);
        float3 normal = (p2 - p0).cross(p1 - p0).normalize();
        normal = normal.dot(this->normals[faces[i].x]) > 0 ? normal : -normal;
        face_normals[i] = normal;
    }
}

std::shared_ptr<Model> Model::combine(const std::shared_ptr<Model> &model1, const std::shared_ptr<Model> &model2) {
    // Create a new model to hold the combined data
    auto combined_model = std::make_shared<Model>();

    // Combine vertices
    combined_model->vertices = model1->vertices;
    size_t vertex_offset     = model1->vertices.size();
    combined_model->vertices.insert(combined_model->vertices.end(), model2->vertices.begin(), model2->vertices.end());

    // Combine normals
    combined_model->normals = model1->normals;
    combined_model->normals.insert(combined_model->normals.end(), model2->normals.begin(), model2->normals.end());

    // Combine faces with updated indices
    combined_model->faces = model1->faces;
    for (const auto &face : model2->faces) {
        combined_model->faces.emplace_back(face.x + static_cast<int>(vertex_offset),
                                           face.y + static_cast<int>(vertex_offset),
                                           face.z + static_cast<int>(vertex_offset));
    }

    // Combine face normals
    combined_model->face_normals = model1->face_normals;
    combined_model->face_normals.insert(combined_model->face_normals.end(), model2->face_normals.begin(),
                                        model2->face_normals.end());

    // Handle model matrix (combine with respect to the transformation matrices)
    combined_model->m_model_matrix  = matrix4::identity();
    combined_model->m_normal_matrix = matrix3::identity();

    // Handle bounding box
    combined_model->bounding_box = BoundingBox::merge(model1->bounding_box, model2->bounding_box);

    return combined_model;
}

Model Model::copy() const {
    // Create a new model and deep copy the data
    Model new_model = *this;

    // Deep copy of vertices, normals, faces, and face_normals
    new_model.vertices     = this->vertices;
    new_model.normals      = this->normals;
    new_model.faces        = this->faces;
    new_model.face_normals = this->face_normals;

    // Handle deep copy of model and normal matrices
    new_model.m_model_matrix  = this->m_model_matrix;
    new_model.m_normal_matrix = this->m_normal_matrix;

    return new_model;
}

BoundingBox Model::get_bounding_box(uint32_t index) const {
    auto temp = float3(vertices[faces[index].x]);
    BoundingBox result(temp);
    result.expand_by(float3(vertices[faces[index].y]));
    result.expand_by(float3(vertices[faces[index].z]));
    return result;
}

float3 Model::get_centroid(uint32_t index) const {
    return float3(vertices[faces[index].x] + vertices[faces[index].y] + vertices[faces[index].z]) * (1.0f / 3.0f);
}

Model::OBJVertex::OBJVertex(const std::string &string) {
    std::vector<std::string> tokens = tokenize(string, "/", true);

    if (tokens.empty() || tokens.size() > 3)
        throw std::runtime_error("Invalid vertex data: " + string);

    p = to_uint(tokens[0]);

    if (tokens.size() >= 2 && !tokens[1].empty())
        uv = to_uint(tokens[1]);

    if (tokens.size() >= 3 && !tokens[2].empty())
        n = to_uint(tokens[2]);
}

bool Model::OBJVertex::operator==(const OBJVertex &v) const { return v.p == p && v.n == n && v.uv == uv; }

std::size_t Model::OBJVertexHash::operator()(const OBJVertex &v) const {
    size_t hash = std::hash<uint32_t>()(v.p);
    hash        = hash * 37 + std::hash<uint32_t>()(v.uv);
    hash        = hash * 37 + std::hash<uint32_t>()(v.n);
    return hash;
}
