#include <core/bvh.h>

BVHNode::BVHNode(const BoundingBox &bounding_box_)
    : bounding_box(bounding_box_), left(nullptr), right(nullptr), is_leaf(false) {}

BVHAccel::BVHAccel(int max_depth_, int max_primitives_per_leaf_) {
    max_depth               = max_depth_;
    max_primitives_per_leaf = max_primitives_per_leaf_;
}

void BVHAccel::set_model(const std::shared_ptr<Model> &mesh) {
    model = mesh;
}

void BVHAccel::construct() {
    primitives.reserve(model->faces.size());
    for (uint32_t i = 0; i < model->faces.size(); ++i) {
        primitives.emplace_back(i);
    }
    bounding_box = model->bounding_box;
    root = build_tree(primitives, 0);
}

const BoundingBox &BVHAccel::get_bounding_box() const { return bounding_box; }

std::shared_ptr<BVHNode> BVHAccel::build_tree(std::vector<int> &primitives, int depth) {
    if (primitives.size() <= max_primitives_per_leaf || depth >= max_depth) {
        auto node        = std::make_shared<BVHNode>(BoundingBox());
        node->is_leaf    = true;
        node->primitives = primitives;

        for (const auto &prim : primitives) {
            node->bounding_box.expand_by(model->get_bounding_box(prim));
        }
        return node;
    }

    // Find the largest axis
    BoundingBox bbox;
    for (const auto &prim : primitives) {
        bbox.expand_by(model->get_bounding_box(prim));
    }
    int axis = bbox.get_major_axis();

    std::shared_ptr<Model> temp_model = model;

    // Sort primitives along the chosen axis
    std::sort(primitives.begin(), primitives.end(), [axis, temp_model](const uint32_t a, const uint32_t b) {
        return temp_model->get_centroid(a)(axis) < temp_model->get_centroid(b)(axis);
    });

    // Split primitives into two groups
    size_t mid = primitives.size() / 2;
    std::vector left_primitives(primitives.begin(), primitives.begin() + static_cast<long long>(mid));
    std::vector right_primitives(primitives.begin() + static_cast<long long>(mid), primitives.end());

    auto node   = std::make_shared<BVHNode>(bbox);
    node->left  = build_tree(left_primitives, depth + 1);
    node->right = build_tree(right_primitives, depth + 1);

    return node;
}