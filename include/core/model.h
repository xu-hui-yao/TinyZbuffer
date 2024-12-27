#pragma once

#include <core/boundingbox.h>
#include <core/common.h>

class Model {
public:
    Model() = default;

    explicit Model(const std::string &filename, const matrix4 &model_matrix = matrix4::identity());

    static std::shared_ptr<Model> combine(const std::shared_ptr<Model> &model1, const std::shared_ptr<Model> &model2);

    [[nodiscard]] Model copy() const;

    [[nodiscard]] BoundingBox get_bounding_box(uint32_t index) const;

    [[nodiscard]] float3 get_centroid(uint32_t index) const;

    std::vector<float4> vertices; // Vertex positions
    std::vector<float3> normals;  // Vertex normals
    std::vector<int3> faces;      // Faces
    std::vector<float3> face_normals;
    BoundingBox bounding_box;

private:
    matrix4 m_model_matrix;
    matrix3 m_normal_matrix;

    struct OBJVertex {
        uint32_t p  = static_cast<uint32_t>(-1);
        uint32_t n  = static_cast<uint32_t>(-1);
        uint32_t uv = static_cast<uint32_t>(-1);

        explicit OBJVertex() = default;

        explicit OBJVertex(const std::string &string);

        bool operator==(const OBJVertex &v) const;
    };

    struct OBJVertexHash {
        std::size_t operator()(const OBJVertex &v) const;
    };
};