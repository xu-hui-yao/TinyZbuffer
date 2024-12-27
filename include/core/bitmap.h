#pragma once

#include <core/common.h>

class Bitmap {
public:
    Bitmap(int height, int width);

    void set_data(std::vector<float3> data);

    [[nodiscard]] int index(int row, int col) const;

    void save_exr(const std::string &filename) const;

    void save_png(const std::string &filename) const;

    std::vector<float3> m_data;
    int m_rows, m_cols;
};
