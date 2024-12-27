#pragma once

#include <iostream>
#include <string>
#include <vector>

#define M_EPSILON 1e-4
#define M_PI 3.14159265358979323846
#define M_INV_PI 0.31830988618379067154
#define M_INV_TWOPI 0.15915494309189533577
#define M_INV_FOUR_PI 0.07957747154594766788
#define M_SQRT_TWO 1.41421356237309504880
#define M_INV_SQRT_TWO 0.70710678118654752440
#define M_MAX_FLOAT 3.402823466e+38

#define M_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define M_MAX(a, b) (((a) > (b)) ? (a) : (b))

extern std::vector<std::string> tokenize(const std::string &string, const std::string &delim, bool include_empty);

// Convert a string to lower case
extern std::string to_lower(const std::string &value);

// Convert a string into a boolean value
extern bool to_bool(const std::string &str);

// Convert a string into a signed integer value
extern int to_int(const std::string &str);

// Convert a string into an unsigned integer value
extern unsigned int to_uint(const std::string &str);

// Convert a string into a floating point value
extern float to_float(const std::string &str);

// Convert a time value in milliseconds into a human-readable string
extern std::string time_string(double time, bool precise = false);

// Convert radians to degrees
template <typename Scalar> Scalar rad_to_deg(Scalar value) { return value * (180.0 / M_PI); }

// Convert degrees to radians
template <typename Scalar> Scalar deg_to_rad(Scalar value) { return value * (M_PI / 180.0); }

// Linearly interpolate between two values
template <typename Scalar> Scalar interpolate(Scalar t, Scalar v1, Scalar v2) {
    return (static_cast<Scalar>(1) - t) * v1 + t * v2;
}

template <typename Scalar> Scalar sqrt(Scalar value) { return std::sqrt(value); }

template <typename Scalar> Scalar safe_sqrt(Scalar value) {
    value = M_MAX(0, value);
    return std::sqrt(value);
}

template <typename Scalar> static Scalar clamp(Scalar value, Scalar min, Scalar max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

template <typename Scalar> static Scalar pow(Scalar base, Scalar exponent) { return std::pow(base, exponent); }

// Always-positive modulo operation
template <typename Scalar> int mod(Scalar a, Scalar b) {
    Scalar r = a % b;
    return r < 0 ? r + b : r;
}

template <typename Scalar> Scalar cos(Scalar x) { return std::cos(x); }

template <typename Scalar> Scalar sin(Scalar x) { return std::sin(x); }

template <typename Scalar> Scalar tan(Scalar x) { return std::tan(x); }

template <typename Scalar> Scalar atan2(Scalar y, Scalar x) { return std::atan2(y, x); }

template <typename Scalar> Scalar asin(Scalar x) { return std::asin(x); }

template <typename Scalar> Scalar acos(Scalar x) { return std::acos(x); }

template <typename Scalar> Scalar abs(Scalar x) { return x < 0 ? -x : x; }

template <typename Scalar> Scalar floor(Scalar x) { return std::floor(x); }

template <typename Scalar> Scalar ceil(Scalar x) { return std::ceil(x); }

template <typename Scalar> Scalar lerp(Scalar a, Scalar b, Scalar t) { return b * t + a * (Scalar(1) - t); }

template <typename T> T copysign(T value, T sign) { return value >= T(0) == sign >= T(0) ? value : -value; }

template <typename T, typename S> T mulsign(T value, S sign) { return sign >= 0 ? value : -value; }

struct float4 {
    float x, y, z, w;

    float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float4(const float4 &other) = default;

    float4 &operator=(const float4 &other) = default;

    [[nodiscard]] float4 wise_min(const float4 &other) const {
        float4 result;
        result.x = M_MIN(x, other.x);
        result.y = M_MIN(y, other.y);
        result.z = M_MIN(z, other.z);
        result.w = M_MIN(w, other.w);
        return result;
    }

    [[nodiscard]] float4 wise_max(const float4 &other) const {
        float4 result;
        result.x = M_MAX(x, other.x);
        result.y = M_MAX(y, other.y);
        result.z = M_MAX(z, other.z);
        result.w = M_MAX(w, other.w);
        return result;
    }

    [[nodiscard]] float magnitude() const { return sqrt(x * x + y * y + z * z + w * w); }

    [[nodiscard]] float4 normalize() const {
        float magnitude = this->magnitude();
        if (magnitude != 0.0f) {
            return *this / magnitude;
        } else {
            return *this;
        }
    }

    float operator[](int index) const {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                std::cerr << "Error: index out of range" << std::endl;
                exit(1);
        }
    }

    float &operator[](int index) {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                std::cerr << "Error: index out of range" << std::endl;
                exit(1);
        }
    }

    float4 operator-() const { return { -x, -y, -z, -w }; }

    float4 operator+(const float4 &other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }

    void operator+=(const float4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    float4 operator-(const float4 &other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }

    void operator-=(const float4 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    float4 operator*(const float4 &other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }

    void operator*=(const float4 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
    }

    float4 operator/(const float4 &other) const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }

    void operator/=(const float4 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
    }

    float4 operator*(float other) const { return { x * other, y * other, z * other, w * other }; }

    void operator*=(float other) {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
    }

    float4 operator/(float other) const { return { x / other, y / other, z / other, w / other }; }

    void operator/=(float other) {
        x /= other;
        y /= other;
        z /= other;
        w /= other;
    }

    float4 operator+(float other) const { return { x + other, y + other, z + other, w + other }; }

    void operator+=(float other) {
        x += other;
        y += other;
        z += other;
        w += other;
    }

    float4 operator-(float other) const { return { x - other, y - other, z - other, w - other }; }

    void operator-=(float other) {
        x -= other;
        y -= other;
        z -= other;
        w -= other;
    }
};

struct bool3 {
    bool x, y, z;

    bool3() : x(false), y(false), z(false) {}

    bool3(bool x_, bool y_, bool z_) : x(x_), y(y_), z(z_) {}

    [[nodiscard]] bool all() const { return x && y && z; }

    [[nodiscard]] bool any() const { return x || y || z; }
};

struct float3 {
    float x, y, z;

    float3() : x(0.0f), y(0.0f), z(0.0f) {}

    float3(float x, float y, float z) : x(x), y(y), z(z) {}

    float3(const float3 &other) = default;

    explicit float3(const float4 &other) : x(other.x), y(other.y), z(other.z) {}

    float3 &operator=(const float3 &other) = default;

    bool operator==(const float3 &other) const { return x == other.x && y == other.y && z == other.z; }

    bool operator!=(const float3 &other) const { return !(*this == other); }

    bool3 operator<(const float3 &other) const {
        bool3 result;
        result.x = x < other.x;
        result.y = y < other.y;
        result.z = z < other.z;
        return result;
    }

    bool3 operator<=(const float3 &other) const {
        bool3 result;
        result.x = x <= other.x;
        result.y = y <= other.y;
        result.z = z <= other.z;
        return result;
    }

    bool3 operator>(const float3 &other) const {
        bool3 result;
        result.x = x > other.x;
        result.y = y > other.y;
        result.z = z > other.z;
        return result;
    }

    bool3 operator>=(const float3 &other) const {
        bool3 result;
        result.x = x >= other.x;
        result.y = y >= other.y;
        result.z = z >= other.z;
        return result;
    }

    [[nodiscard]] float prod() const { return x * y * z; }

    [[nodiscard]] float3 wise_min(const float3 &other) const {
        float3 result;
        result.x = M_MIN(x, other.x);
        result.y = M_MIN(y, other.y);
        result.z = M_MIN(z, other.z);
        return result;
    }

    [[nodiscard]] float3 wise_max(const float3 &other) const {
        float3 result;
        result.x = M_MAX(x, other.x);
        result.y = M_MAX(y, other.y);
        result.z = M_MAX(z, other.z);
        return result;
    }

    [[nodiscard]] float3 abs() const {
        float3 result;
        result.x = std::abs(x);
        result.y = std::abs(y);
        result.z = std::abs(z);
        return result;
    }

    [[nodiscard]] float dot(const float3 &other) const { return x * other.x + y * other.y + z * other.z; }

    [[nodiscard]] float3 cross(const float3 &other) const {
        float3 result;
        result.x = y * other.z - z * other.y;
        result.y = z * other.x - x * other.z;
        result.z = x * other.y - y * other.x;
        return result;
    }

    [[nodiscard]] float magnitude() const { return std::sqrtf(x * x + y * y + z * z); }

    [[nodiscard]] float3 normalize() const {
        float magnitude = this->magnitude();
        if (magnitude != 0.0f) {
            return *this / magnitude;
        } else {
            return *this;
        }
    }

    [[nodiscard]] float3 to_srgb() const {
        float3 result;

        float value = x;
        if (value <= 0.0031308f) {
            result.x = 12.92f * value;
        } else {
            result.x = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
        }

        value = y;
        if (value <= 0.0031308f) {
            result.y = 12.92f * value;
        } else {
            result.y = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
        }

        value = z;
        if (value <= 0.0031308f) {
            result.z = 12.92f * value;
        } else {
            result.z = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
        }

        return result;
    }

    float operator[](int index) const {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Error: index out of range" << std::endl;
                exit(1);
        }
    }

    float &operator[](int index) {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Error: index out of range" << std::endl;
                exit(1);
        }
    }

    float operator()(int index) const {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Error: index out of range" << std::endl;
            exit(1);
        }
    }

    float &operator()(int index) {
        switch (index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Error: index out of range" << std::endl;
            exit(1);
        }
    }

    float3 operator-() const { return { -x, -y, -z }; }

    float3 operator+(const float3 &other) const { return { x + other.x, y + other.y, z + other.z }; }

    void operator+=(const float3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    float3 operator-(const float3 &other) const { return { x - other.x, y - other.y, z - other.z }; }

    void operator-=(const float3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    float3 operator*(const float3 &other) const { return { x * other.x, y * other.y, z * other.z }; }

    void operator*=(const float3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    float3 operator/(const float3 &other) const { return { x / other.x, y / other.y, z / other.z }; }

    void operator/=(const float3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    float3 operator*(float other) const { return { x * other, y * other, z * other }; }

    void operator*=(float other) {
        x *= other;
        y *= other;
        z *= other;
    }

    float3 operator/(float other) const { return { x / other, y / other, z / other }; }

    void operator/=(float other) {
        x /= other;
        y /= other;
        z /= other;
    }

    float3 operator+(float other) const { return { x + other, y + other, z + other }; }

    void operator+=(float other) {
        x += other;
        y += other;
        z += other;
    }

    float3 operator-(float other) const { return { x - other, y - other, z - other }; }

    void operator-=(float other) {
        x -= other;
        y -= other;
        z -= other;
    }
};

struct float2 {
    float x, y;

    float2() : x(0.0f), y(0.0f) {}

    float2(float x, float y) : x(x), y(y) {}

    [[nodiscard]] float prod() const { return x * y; }

    float2 operator-() const { return { -x, -y }; }

    float2 operator+(const float2 &other) const { return { x + other.x, y + other.y }; }

    void operator+=(const float2 &other) {
        x += other.x;
        y += other.y;
    }

    float2 operator-(const float2 &other) const { return { x - other.x, y - other.y }; }

    void operator-=(const float2 &other) {
        x -= other.x;
        y -= other.y;
    }

    float2 operator*(const float2 &other) const { return { x * other.x, y * other.y }; }

    void operator*=(const float2 &other) {
        x *= other.x;
        y *= other.y;
    }

    float2 operator/(const float2 &other) const {
        return {
            x / other.x,
            y / other.y,
        };
    }

    void operator/=(const float2 &other) {
        x /= other.x;
        y /= other.y;
    }

    float2 operator*(float other) const { return { x * other, y * other }; }

    void operator*=(float other) {
        x *= other;
        y *= other;
    }

    float2 operator/(float other) const { return { x / other, y / other }; }

    void operator/=(float other) {
        x /= other;
        y /= other;
    }

    float2 operator+(float other) const { return { x + other, y + other }; }

    void operator+=(float other) {
        x += other;
        y += other;
    }

    float2 operator-(float other) const { return { x - other, y - other }; }

    void operator-=(float other) {
        x -= other;
        y -= other;
    }

    [[nodiscard]] float dot(const float2 &other) const { return x * other.x + y * other.y; }

    [[nodiscard]] float cross(const float2 &other) const { return x * other.y - y * other.x; }
};

struct int3 {
    int x, y, z;

    int3() : x(0.0f), y(0.0f), z(0.0f) {}

    int3(int x, int y, int z) : x(x), y(y), z(z) {}

    [[nodiscard]] int prod() const { return x * y * z; }

    int operator[](int i) const {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return 0;
        }
    }

    int3 operator-() const { return { -x, -y, -z }; }

    int3 operator+(const int3 &other) const { return { x + other.x, y + other.y, z + other.z }; }

    void operator+=(const int3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    int3 operator-(const int3 &other) const { return { x - other.x, y - other.y, z - other.z }; }

    void operator-=(const int3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    int3 operator*(const int3 &other) const { return { x * other.x, y * other.y, z * other.z }; }

    void operator*=(const int3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    int3 operator/(const int3 &other) const { return { x / other.x, y / other.y, z / other.z }; }

    void operator/=(const int3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    int3 operator*(int other) const { return { x * other, y * other, z * other }; }

    void operator*=(int other) {
        x *= other;
        y *= other;
        z *= other;
    }

    int3 operator/(int other) const { return { x / other, y / other, z / other }; }

    void operator/=(int other) {
        x /= other;
        y /= other;
        z /= other;
    }

    int3 operator+(int other) const { return { x + other, y + other, z + other }; }

    void operator+=(int other) {
        x += other;
        y += other;
        z += other;
    }

    int3 operator-(int other) const { return { x - other, y - other, z - other }; }

    void operator-=(int other) {
        x -= other;
        y -= other;
        z -= other;
    }
};

struct int2 {
    int x, y;

    int2() : x(0.0f), y(0.0f) {}

    int2(int x, int y) : x(x), y(y) {}

    [[nodiscard]] int prod() const { return x * y; }

    int operator[](int i) const {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            default:
                return 0;
        }
    }

    int2 operator-() const { return { -x, -y }; }

    int2 operator+(const int2 &other) const { return { x + other.x, y + other.y }; }

    void operator+=(const int2 &other) {
        x += other.x;
        y += other.y;
    }

    int2 operator-(const int2 &other) const { return { x - other.x, y - other.y }; }

    void operator-=(const int2 &other) {
        x -= other.x;
        y -= other.y;
    }

    int2 operator*(const int2 &other) const { return { x * other.x, y * other.y }; }

    void operator*=(const int2 &other) {
        x *= other.x;
        y *= other.y;
    }

    int2 operator/(const int2 &other) const { return { x / other.x, y / other.y }; }

    void operator/=(const int2 &other) {
        x /= other.x;
        y /= other.y;
    }

    int2 operator*(int other) const { return { x * other, y * other }; }

    void operator*=(int other) {
        x *= other;
        y *= other;
    }

    int2 operator/(int other) const { return { x / other, y / other }; }

    void operator/=(int other) {
        x /= other;
        y /= other;
    }

    int2 operator+(int other) const { return { x + other, y + other }; }

    void operator+=(int other) {
        x += other;
        y += other;
    }

    int2 operator-(int other) const { return { x - other, y - other }; }

    void operator-=(int other) {
        x -= other;
        y -= other;
    }
};

struct matrix3 {
    float data[3][3]{};

    matrix3() {
        for (auto &i : data) {
            for (float &j : i) {
                j = 0.0f;
            }
        }
    }

    static matrix3 identity() {
        matrix3 m;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == j) {
                    m.data[i][j] = 1.0f;
                } else {
                    m.data[i][j] = 0.0f;
                }
            }
        }
        return m;
    }

    [[nodiscard]] matrix3 transpose() const {
        matrix3 m;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m.data[j][i] = data[i][j];
            }
        }
        return m;
    }

    float operator()(int row, int col) const { return data[row][col]; }

    float &operator()(int row, int col) { return data[row][col]; }

    matrix3 operator*(const matrix3 &m) const {
        matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    result(i, j) += (*this)(i, k) * m(k, j);
                }
            }
        }
        return result;
    }

    float3 operator*(const float3 &v) const {
        float3 result(0, 0, 0);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result[i] += (*this)(i, j) * v[j];
            }
        }
        return result;
    }

    matrix3 operator*(float s) {
        matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result(i, j) = s * this->operator()(i, j);
            }
        }
        return result;
    }

    matrix3 operator+(const matrix3 &m) const {
        matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result(i, j) = this->operator()(i, j) + m(i, j);
            }
        }
        return result;
    }
};

struct matrix4 {
    float data[4][4]{};

    matrix4() {
        for (auto &i : data) {
            for (float &j : i) {
                j = 0.0f;
            }
        }
    }

    static matrix4 identity() {
        matrix4 m;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j) {
                    m.data[i][j] = 1.0f;
                } else {
                    m.data[i][j] = 0.0f;
                }
            }
        }
        return m;
    }

    [[nodiscard]] matrix4 transpose() const {
        matrix4 m;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m.data[i][j] = data[j][i];
            }
        }
        return m;
    }

    float operator()(int row, int col) const { return data[row][col]; }

    float &operator()(int row, int col) { return data[row][col]; }

    [[nodiscard]] matrix4 inverse() const {
        matrix4 result;
        // Step 1: LU decomposition
        matrix4 l;
        matrix4 u;

        // Initialize L as an identity matrix
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                l(i, j) = i == j ? static_cast<float>(1) : static_cast<float>(0);
                u(i, j) = 0;
            }
        }

        // Compute LU decomposition (no parallelism due to m_data dependency)
        for (int i = 0; i < 4; ++i) {
            // Step 1: Find the pivot row
            int pivot_row = i;
            for (int j = i + 1; j < 4; ++j) {
                if (abs(u(j, i)) > abs(u(pivot_row, i))) {
                    pivot_row = j;
                }
            }

            // Step 2: Swap rows in U and L if necessary
            if (pivot_row != i) {
                for (int k = 0; k < 4; ++k) {
                    auto temp       = u(i, k);
                    u(i, k)         = u(pivot_row, k);
                    u(pivot_row, k) = temp;
                }
                for (int k = 0; k < i; ++k) {
                    auto temp       = l(i, k);
                    l(i, k)         = l(pivot_row, k);
                    l(pivot_row, k) = temp;
                }
            }

            // Step 3: Compute U (upper triangular matrix)
            for (int j = i; j < 4; ++j) {
                float sum = 0;
                for (int k = 0; k < i; ++k) {
                    sum += l(i, k) * u(k, j);
                }
                u(i, j) = (*this)(i, j) - sum;
            }

            // Step 4: Compute L (lower triangular matrix)
            for (int j = i + 1; j < 4; ++j) {
                float sum = 0;
                for (int k = 0; k < i; ++k) {
                    sum += l(j, k) * u(k, i);
                }
                float denominator = u(i, i);
                l(j, i)           = ((*this)(j, i) - sum) / denominator;
            }
        }

        // Step 2: Compute the inverse of L and U
        matrix4 l_inv;
        matrix4 u_inv;

        // Inverse of L (no parallelism due to dependency)
        for (int i = 0; i < 4; ++i) {
            float denominator = l(i, i);
            l_inv(i, i)       = static_cast<float>(1) / denominator;
            for (int j = i + 1; j < 4; ++j) {
                float sum = 0;
                for (int k = i; k < j; ++k) {
                    sum += l(j, k) * l_inv(k, i);
                }
                denominator = l(j, j);
                l_inv(j, i) = -sum / denominator;
            }
        }

        // Inverse of U (no parallelism due to dependency)
        for (int i = 4 - 1; i >= 0; --i) {
            float denominator = u(i, i);
            u_inv(i, i)       = static_cast<float>(1) / denominator;
            for (int j = i - 1; j >= 0; --j) {
                float sum = 0;
                for (int k = j + 1; k <= i; ++k) {
                    sum += u(j, k) * u_inv(k, i);
                }
                denominator = u(j, j);
                u_inv(j, i) = -sum / denominator;
            }
        }

        // Step 3: Multiply U_inv and L_inv to get the inverse
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                float sum = 0;
                for (int k = 0; k < 4; ++k) {
                    sum += u_inv(i, k) * l_inv(k, j);
                }
                result(i, j) = sum;
            }
        }

        return result;
    }

    matrix4 operator*(const matrix4 &m) const {
        matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    result(i, j) += (*this)(i, k) * m(k, j);
                }
            }
        }
        return result;
    }

    float4 operator*(const float4 &v) const {
        float4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i] += (*this)(i, j) * v[j];
            }
        }
        return result;
    }

    static matrix4 translate(float x, float y, float z) {
        matrix4 m = identity();
        m(0, 3)   = x;
        m(1, 3)   = y;
        m(2, 3)   = z;
        return m;
    }

    static matrix4 scale(float x, float y, float z) {
        matrix4 m = identity();
        m(0, 0)   = x;
        m(1, 1)   = y;
        m(2, 2)   = z;
        return m;
    }

    static matrix4 rotate(float radiance, float3 axis) {
        axis                      = axis.normalize();
        float cos_theta           = cos(radiance);
        float sin_theta           = sin(radiance);
        float one_minus_cos_theta = static_cast<float>(1) - cos_theta;

        float x = axis[0];
        float y = axis[1];
        float z = axis[2];

        matrix3 k;
        k(0, 0) = 0;
        k(0, 1) = -z;
        k(0, 2) = y;
        k(1, 0) = z;
        k(1, 1) = 0;
        k(1, 2) = -x;
        k(2, 0) = -y;
        k(2, 1) = x;
        k(2, 2) = 0;

        matrix3 rotate_matrix = matrix3::identity() + k * sin_theta + k * k * one_minus_cos_theta;

        matrix4 transform_matrix = identity();
        transform_matrix(0, 0)   = rotate_matrix(0, 0);
        transform_matrix(0, 1)   = rotate_matrix(0, 1);
        transform_matrix(0, 2)   = rotate_matrix(0, 2);
        transform_matrix(1, 0)   = rotate_matrix(1, 0);
        transform_matrix(1, 1)   = rotate_matrix(1, 1);
        transform_matrix(1, 2)   = rotate_matrix(1, 2);
        transform_matrix(2, 0)   = rotate_matrix(2, 0);
        transform_matrix(2, 1)   = rotate_matrix(2, 1);
        transform_matrix(2, 2)   = rotate_matrix(2, 2);

        return transform_matrix;
    }

    static matrix4 look_at(float3 origin, float3 target, float3 up) {
        // Calculate the forward direction (camera looks towards -Z by default
        // in right-handed coordinate system)
        float3 forward = (target - origin).normalize();

        // Calculate the left direction
        float3 left = up.normalize().cross(forward).normalize();

        // Recalculate up to ensure orthogonality
        float3 adjusted_up = forward.cross(left).normalize();

        // Construct the look-at matrix
        matrix4 rotate_matrix = identity();
        rotate_matrix(0, 0)   = left[0];
        rotate_matrix(0, 1)   = left[1];
        rotate_matrix(0, 2)   = left[2];
        rotate_matrix(1, 0)   = adjusted_up[0];
        rotate_matrix(1, 1)   = adjusted_up[1];
        rotate_matrix(1, 2)   = adjusted_up[2];
        rotate_matrix(2, 0)   = forward[0];
        rotate_matrix(2, 1)   = forward[1];
        rotate_matrix(2, 2)   = forward[2];

        matrix4 translation_matrix = translate(-origin[0], -origin[1], -origin[2]);

        return rotate_matrix * translation_matrix;
    }

    static matrix4 perspective(float fov, float aspect, float near, float far) {
        float recip = 1.0f / (far - near);
        float cot   = 1.0f / tan(deg_to_rad(fov / 2.0f));
        matrix4 m1;
        m1(0, 0)   = cot;
        m1(1, 1)   = cot;
        m1(2, 2)   = far * recip;
        m1(2, 3)   = -near * far * recip;
        m1(3, 2)   = 1;
        matrix4 m2 = translate(1.0f, 1.0f / aspect, 0.0f);
        matrix4 m3 = scale(0.5f, 0.5f * aspect, 1.0f);

        return m3 * m2 * m1;
    }

    [[nodiscard]] matrix3 left_top_corner() const {
        matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result(i, j) = data[i][j];
            }
        }
        return result;
    }

    [[nodiscard]] std::string to_string() const {
        std::string matrix;
        matrix += "[\n";
        for (auto i : data) {
            matrix += "  [";
            for (int j = 0; j < 3; ++j) {
                matrix += std::to_string(i[j]) + ", ";
            }
            matrix += std::to_string(i[3]) + "]\n";
        }
        matrix += "]";
        return matrix;
    }
};
