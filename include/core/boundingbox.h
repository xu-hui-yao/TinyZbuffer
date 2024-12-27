#pragma once

#include <core/common.h>

struct BoundingBox {
    float3 m_min_p, m_max_p;

    BoundingBox();

    explicit BoundingBox(float3 p);

    BoundingBox(float3 min, float3 max);

    // ============================== Function ==============================

    bool operator==(const BoundingBox &bounding_box) const;

    bool operator!=(const BoundingBox &bbox) const;

    [[nodiscard]] float get_volume() const;

    [[nodiscard]] float get_surface_area() const;

    [[nodiscard]] float3 get_center() const;

    [[nodiscard]] bool contains(const float3 &p) const;

    [[nodiscard]] bool contains(const BoundingBox &bbox) const;

    [[nodiscard]] bool overlaps(const BoundingBox &bbox) const;

    [[nodiscard]] float squared_distance_to(const float3 &p) const;

    [[nodiscard]] float distance_to(const float3 &p) const;

    [[nodiscard]] float squared_distance_to(const BoundingBox &bbox) const;

    [[nodiscard]] float distance_to(const BoundingBox &bbox) const;

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] bool is_point() const;

    [[nodiscard]] bool has_volume() const;

    [[nodiscard]] int get_major_axis() const;

    [[nodiscard]] int get_minor_axis() const;

    [[nodiscard]] float3 get_extents() const;

    void clip(const BoundingBox &bounding_box);

    void expand_by(const float3 &p);

    void expand_by(const BoundingBox &bbox);

    static BoundingBox merge(const BoundingBox &bbox1, const BoundingBox &bbox2);

    [[nodiscard]] float3 get_corner(int index) const;
};