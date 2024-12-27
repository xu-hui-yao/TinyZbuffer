#pragma once

#include <core/quadtree.h>
#include <zbuffer/zbuffer.h>

typedef TQuadTree<float, int2> QuadTree;

class HierarchicalZBuffer : public ZBuffer {
public:
    HierarchicalZBuffer(int width, int height);

    ~HierarchicalZBuffer() override;

    void apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) override;

private:
    std::shared_ptr<QuadTree> m_z_pyramid;
    std::vector<std::shared_ptr<QuadTree>> m_z_buffer;

    std::shared_ptr<QuadTree> build_pyramid(int2 min, int2 max);

    [[nodiscard]] int get_index(int row, int col) const;

    void pyramid_test(const Fragment &fragment, const std::shared_ptr<QuadTree> &node,
                      const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer);

    float node_test(const Fragment &fragment, const std::shared_ptr<QuadTree> &node,
                    const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer);
};
