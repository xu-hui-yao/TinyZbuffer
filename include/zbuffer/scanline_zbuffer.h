#pragma once

#include <core/gbuffer.h>
#include <memory>
#include <core/model.h>
#include <zbuffer/zbuffer.h>

typedef struct PolygonClassify {
    float a, b, c, d; // Equation of the polygon
    int id;           // Triangle id
    int dy;           // Cross scanline number in y direction
} PolygonClassify;

typedef struct EdgeClassify {
    float x;  // x of the top endpoint
    float y;  // y of the top endpoint
    float dx; // - 1 / k
    int dy;   // Cross scanline number in y direction
    int id;   // Triangle id
} EdgeClassify;

typedef struct ActiveEdge {
    float xl;  // x value of the left intersection
    float dxl; // x offset when y -= 1
    int dyl;   // Scanline number left
    float xr;  // x value of the right intersection
    float dxr; // x offset when y -= 1
    int dyr;   // Scanline number left
    float zl;  // z value of the left intersection
    float dzx; // z offset when x += 1, typically -a/c
    float dzy; // z offset when y -= 1, typically b/c
    int id;    // Triangle id
} ActiveEdge;

class ScanlineZBuffer : public ZBuffer {
public:
    ScanlineZBuffer(int width, int height);

    ~ScanlineZBuffer() override;

    void apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) override;

private:
    std::vector<std::vector<PolygonClassify>> m_classified_polygon_table;
    std::vector<std::vector<EdgeClassify>> m_classified_edge_table;
    std::vector<PolygonClassify> m_active_polygon_table;
    std::vector<ActiveEdge> m_active_edge_table;

    void initialize(const std::shared_ptr<Model> &model);

    void add_active_table(int y);

    void update_depth(int y, const std::shared_ptr<GBuffer> &gbuffer, const std::shared_ptr<Model> &model);

    void find_replace_edge(int y, ActiveEdge &active_edge) const;

    void cull_active_table();

    [[nodiscard]] int above_scanline_count(float y) const;
};
