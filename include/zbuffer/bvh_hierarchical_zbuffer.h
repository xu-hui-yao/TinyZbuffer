#pragma once

#include <core/bvh.h>
#include <core/quadtree.h>
#include <zbuffer/zbuffer.h>

typedef TQuadTree<float, int2> QuadTree;

class BVHHierarchicalZBuffer : public ZBuffer {
public:
    BVHHierarchicalZBuffer(int width, int height);

    ~BVHHierarchicalZBuffer() override;

    void apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) override;

private:
    std::shared_ptr<QuadTree> m_z_pyramid;
    std::vector<std::shared_ptr<QuadTree>> m_z_buffer;
    std::shared_ptr<BVHAccel> m_accel;

    std::shared_ptr<QuadTree> build_pyramid(int2 min, int2 max);

    [[nodiscard]] int get_index(int row, int col) const;

    void pyramid_test(const std::shared_ptr<BVHNode> &bvh_node, const std::shared_ptr<QuadTree> &zbuffer_node,
                      const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer);

    void leaf_node_test(const std::shared_ptr<BVHNode> &bvh_node, const std::shared_ptr<QuadTree> &zbuffer_node,
                        const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer);

    float node_test(const Fragment &fragment, const std::shared_ptr<QuadTree> &node,
                    const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer);

    static void update_z(const std::shared_ptr<QuadTree> &node, float value);
};