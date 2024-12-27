#include <core/boundingbox.h>

BoundingBox::BoundingBox()
    : m_min_p(
          float3(static_cast<float>(M_MAX_FLOAT), static_cast<float>(M_MAX_FLOAT), static_cast<float>(M_MAX_FLOAT))),
      m_max_p(float3(-static_cast<float>(M_MAX_FLOAT), -static_cast<float>(M_MAX_FLOAT),
                     -static_cast<float>(M_MAX_FLOAT))) {}

BoundingBox::BoundingBox(float3 p) : m_min_p(p), m_max_p(p) {}

BoundingBox::BoundingBox(float3 min, float3 max) : m_min_p(min), m_max_p(max) {}

// ============================== Function ==============================

bool BoundingBox::operator==(const BoundingBox &bounding_box) const {
    return m_min_p == bounding_box.m_min_p && m_max_p == bounding_box.m_max_p;
}

bool BoundingBox::operator!=(const BoundingBox &bbox) const {
    return m_min_p != bbox.m_min_p || m_max_p != bbox.m_max_p;
}

float BoundingBox::get_volume() const { return (m_max_p - m_min_p).prod(); }

float BoundingBox::get_surface_area() const {
    float3 d    = m_max_p - m_min_p;
    auto result = static_cast<float>(0);
    for (int i = 0; i < 3; ++i) {
        auto term = static_cast<float>(1);
        for (int j = 0; j < 3; ++j) {
            if (i == j)
                continue;
            term *= d[j];
        }
        result += term;
    }
    return static_cast<float>(2) * result;
}

float3 BoundingBox::get_center() const { return (m_max_p + m_min_p) * static_cast<float>(0.5f); }

bool BoundingBox::contains(const float3 &p) const {
    return (p >= m_min_p).all() && (p <= m_max_p).all();
}

bool BoundingBox::contains(const BoundingBox &bbox) const {
    return (bbox.m_min_p >= m_min_p).all() && (bbox.m_max_p <= m_max_p).all();
}

bool BoundingBox::overlaps(const BoundingBox &bbox) const {
    return (bbox.m_min_p <= m_max_p).all() && (bbox.m_max_p >= m_min_p).all();
}

float BoundingBox::squared_distance_to(const float3 &p) const {
    float result = 0;

    for (int i = 0; i < 3; ++i) {
        float value = 0;
        if (p(i) < m_min_p(i))
            value = m_min_p(i) - p(i);
        else if (p(i) > m_max_p(i))
            value = p(i) - m_max_p(i);
        result += value * value;
    }

    return result;
}

float BoundingBox::distance_to(const float3 &p) const { return sqrt(squared_distance_to(p)); }

float BoundingBox::squared_distance_to(const BoundingBox &bbox) const {
    float result = 0;

    for (int i = 0; i < 3; ++i) {
        float value = 0;
        if (bbox.m_max_p(i) < m_min_p(i))
            value = m_min_p(i) - bbox.m_max_p(i);
        else if (bbox.m_min_p(i) > m_max_p(i))
            value = bbox.m_min_p(i) - m_max_p(i);
        result += value * value;
    }

    return result;
}

float BoundingBox::distance_to(const BoundingBox &bbox) const { return sqrt(squared_distance_to(bbox)); }

bool BoundingBox::is_valid() const { return (m_max_p >= m_min_p).all(); }

bool BoundingBox::is_point() const { return m_max_p == m_min_p; }

bool BoundingBox::has_volume() const { return (m_max_p > m_min_p).all(); }

int BoundingBox::get_major_axis() const {
    float3 d    = m_max_p - m_min_p;
    int largest = 0;
    for (int i = 1; i < 3; ++i)
        if (d(i) > d(largest))
            largest = i;
    return largest;
}

int BoundingBox::get_minor_axis() const {
    float3 d     = m_max_p - m_min_p;
    int shortest = 0;
    for (int i = 1; i < 3; ++i)
        if (d(i) < d(shortest))
            shortest = i;
    return shortest;
}

float3 BoundingBox::get_extents() const { return m_max_p - m_min_p; }

void BoundingBox::clip(const BoundingBox &bounding_box) {
    m_min_p = m_min_p.wise_max(bounding_box.m_min_p);
    m_max_p = m_max_p.wise_min(bounding_box.m_max_p);
}

void BoundingBox::expand_by(const float3 &p) {
    m_min_p = m_min_p.wise_min(p);
    m_max_p = m_max_p.wise_max(p);
}

void BoundingBox::expand_by(const BoundingBox &bbox) {
    m_min_p = m_min_p.wise_min(bbox.m_min_p);
    m_max_p = m_max_p.wise_max(bbox.m_max_p);
}

BoundingBox BoundingBox::merge(const BoundingBox &bbox1, const BoundingBox &bbox2) {
    return { bbox1.m_min_p.wise_min(bbox2.m_min_p), bbox1.m_max_p.wise_max(bbox2.m_max_p) };
}

float3 BoundingBox::get_corner(int index) const {
    float3 result;
    for (int i = 0; i < 3; ++i)
        result(i) = index & 1 << i ? m_max_p(i) : m_min_p(i);
    return result;
}