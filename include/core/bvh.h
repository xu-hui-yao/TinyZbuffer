#pragma once

#include <algorithm>
#include <core/boundingbox.h>
#include <core/model.h>
#include <memory>
#include <vector>

struct BVHNode {
    BoundingBox bounding_box;             // Bounding box for the node
    std::vector<int> primitives;          // Primitives in leaf nodes
    std::shared_ptr<BVHNode> left, right; // Children nodes
    bool is_leaf;                         // Indicates if the node is a leaf

    explicit BVHNode(const BoundingBox &bounding_box_);
};

class BVHAccel {
public:
    explicit BVHAccel(int max_depth_ = 8, int max_primitives_per_leaf_ = 10);

    void set_model(const std::shared_ptr<Model> &mesh);

    void construct();

    [[nodiscard]] const BoundingBox &get_bounding_box() const;

    std::shared_ptr<BVHNode> root;

private:
    int max_depth;
    int max_primitives_per_leaf;
    BoundingBox bounding_box;
    std::vector<int> primitives;
    std::shared_ptr<Model> model;

    // Build BVH recursively
    std::shared_ptr<BVHNode> build_tree(std::vector<int> &primitives, int depth);
};